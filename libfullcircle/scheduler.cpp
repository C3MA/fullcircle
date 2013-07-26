#include "scheduler.hpp"

#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>

namespace bfs=boost::filesystem;

using namespace fullcircle;

void Scheduler::setInput(std::string dir)
{
	_srcdir = dir;
}

void Scheduler::setPriority(std::string dir)
{
	_priodir = dir;
}

void Scheduler::setFallback(std::string dir)
{
	_falldir = dir;
}

void Scheduler::setOutput(std::string dir)
{
	_dstdir = dir;
}

void Scheduler::setDebug(bool debug)
{
	_debug = debug;
}

void Scheduler::setPort(int port, boost::filesystem::path config)
{
	boost::asio::ip::tcp::endpoint server_endpoint(
			boost::asio::ip::tcp::v4(), port);
	_server = fullcircle::NetServer::create(
			_server_io_service, server_endpoint);
	 std::cout << "Found config=" << config << std::endl;	
	_server->run();

	_dispatcher = _server->getDispatcher();
	_dispatcher->setConfig(config);
	if ( _dispatcher != NULL )
	{
		_dispatcher->do_on_request(boost::bind(&fullcircle::Scheduler::addConnection, this, _1));
		//_dispatcher->do_on_frame(boost::bind(&fullcircle::Scheduler::addFrame, this, _1));
	}
}

Scheduler::Scheduler()
	: _debug(false)
	, _srcdir("")
	, _falldir("")
	, _priodir("")
	, _dstdir("")
	, _dispatcher()
	, _net_queue()
{
}

Scheduler::~Scheduler()
{
}

Sequence::Ptr Scheduler::getNextSequence()
{
	try {
		bfs::path source(_srcdir);
		bfs::path priosrc;
		bfs::path fallsrc;
		bfs::path destination(_dstdir);
		bfs::path srcfile;

		bool move = true;

		if ( !_priodir.empty() )
			priosrc = bfs::path(_priodir);

		if ( !_falldir.empty() )
			fallsrc = bfs::path(_falldir);

		try {
			_debug && std::cout << "Loading sequence files from " << _srcdir << std::endl;

			typedef std::vector<bfs::path> vec;             // store paths,
			vec v, vp;                                // so we can sort them later

			if ( priosrc != bfs::path() )
			{
				copy(bfs::directory_iterator(priosrc), bfs::directory_iterator(), back_inserter(vp));

				sort(vp.begin(), vp.end());             // sort, since directory iteration
				// is not ordered on some file systems

				_debug && std::cout << vp.size() << std::endl;
			}

			if ( vp.size() > 0 )
			{
				srcfile = *vp.begin();
			} else {
				//TODO: check network queue
				ServerProtocolDispatcher::Ptr dispatcher;
				while ( _net_queue.size() > 0 )
				{
					std::pair<BinarySequenceMetadata, ServerProtocolDispatcher::Ptr> pair = _net_queue.front();
					dispatcher = pair.second;
					_net_queue.pop();

					if ( dispatcher->is_active() )
					{
						dispatcher->do_on_frame(boost::bind(&fullcircle::Scheduler::addFrame, this, _1));

						dispatcher->send_start();
						//return pair.first;
						BinarySequenceMetadata meta = pair.first;
						return Sequence::Ptr(new Sequence(meta.frames_per_second(), meta.width(), meta.height()));
					}
				}

				if ( dispatcher == NULL ) {
					copy(bfs::directory_iterator(source), bfs::directory_iterator(), back_inserter(v));

					sort(v.begin(), v.end());             // sort, since directory iteration
					// is not ordered on some file systems

					_debug && std::cout << v.size() << std::endl;

					if ( v.size() > 0 )
						srcfile = *v.begin();
					else {
						if ( fallsrc != bfs::path() )
						{
							copy(bfs::directory_iterator(fallsrc), bfs::directory_iterator(), back_inserter(v));

							sort(v.begin(), v.end());             // sort, since directory iteration
							// is not ordered on some file systems

							_debug && std::cout << v.size() << std::endl;

							if ( v.size() > 0 )
							{
								vec::iterator it = v.begin();
								it += std::rand() % v.size();
								srcfile = *it;
								move = false;
							}
						}
					}
				}
			}

			if ( srcfile != bfs::path() )
			{
				_debug && std::cout << "File " << srcfile.c_str() << " chosen." << std::endl;
				std::fstream input(srcfile.c_str(), std::ios::in | std::ios::binary);
				fullcircle::Sequence::Ptr loaded_seq(new fullcircle::Sequence(input));
				input.close();

				// check whether frame fits into one dmx universe
				if ( ( loaded_seq->width() * loaded_seq->height() ) > 512 ) {
					std::cerr << "Frames to large!" << std::endl;
					exit(1);
				}

				if ( move )
				{
					std::string dstfile = srcfile.filename().string();
					int tmp = 0;
					while ( bfs::exists(destination / dstfile ) )
					{
						_debug && std::cout << destination / dstfile << " exists!" << std::endl;
						std::stringstream file(dstfile);
						file << srcfile.filename().string() << "-" << (tmp++);
						file >> dstfile;
					}

					bfs::copy_file(srcfile, destination / dstfile);
					bfs::remove(srcfile);
				}

				return loaded_seq;
			}
		} catch (fullcircle::GenericException& ex) {
			std::cerr << "[Scheduler] Caught fullcircle exception: " << ex.what() << std::endl;
			//exit(1);
		}
	} catch (bfs::filesystem_error& ex) {
		std::cerr << "[Scheduler] Caught filesystem exception: " << ex.what() << std::endl;
		//exit(1);
	}

	return Sequence::Ptr();
}

/* addConnection handles a REQUEST message, extracting metadata, queuing the client in the net_queue
 * and preparing the new session to handle the next REQUEST
 */
void Scheduler::addConnection(Snip_RequestSnip request)
{
	if ( _net_queue.size() < 10 )
	{
		const BinarySequenceMetadata meta = request.meta();
		_net_queue.push(std::pair<BinarySequenceMetadata, ServerProtocolDispatcher::Ptr>(meta, _dispatcher));
		_width = meta.width();
		_height = meta.height();
		_dispatcher->do_on_eos(boost::bind(&Scheduler::seqEnd, this, _dispatcher));
		_dispatcher->do_on_timeout_snip(boost::bind(&Scheduler::seqEnd, this, _dispatcher));
		_dispatcher->do_on_abort(boost::bind(&Scheduler::seqEnd, this, _dispatcher));
		_dispatcher->do_on_timeout(boost::bind(&Scheduler::seqEnd, this, _dispatcher));
		_dispatcher->send_ack();
		std::cout << "addConnection for Sequence " << request.seqid() << " received!" << std::endl;
	}

	// prepare for new connections (the NetServer just created a new session with the old dispatcher and a new dispatcher for new connections. We want to handle requests on that new dispatcher)
	_dispatcher = _server->getDispatcher();
	if ( _dispatcher != NULL )
	{
		_dispatcher->do_on_request(boost::bind(&fullcircle::Scheduler::addConnection, this, _1));
	}
}

/* Frame from the client received. We have to extract it from the message and pass it to the DMXClient
 */
void Scheduler::addFrame(Snip_FrameSnip frame)
{
	std::cout << "addFrame!" << std::endl;
	BinaryFrame binframe(frame.frame());
	Frame::Ptr framep(new Frame(_width, _height));
	for ( int pixelID = 0; pixelID < binframe.pixel_size(); pixelID++ )
	{
		RGBValue pixel = binframe.pixel(pixelID);
		framep->set_pixel(pixel.x(), pixel.y(),
				pixel.red(), pixel.green(), pixel.blue());
	}
	_on_frame(framep);
}

/* For some reason we want to end the current sequence. Therefore we trigger cleanup and stop the session, which closes the socket
 */
void Scheduler::seqEnd(ServerProtocolDispatcher::Ptr dispatcher)
{
	std::cout << "seqEnd: session count: " << dispatcher->getSession().use_count() << std::endl;
	_on_end();
	_on_frame.disconnect(&fullcircle::Scheduler::addFrame);
	dispatcher->getSession()->stop();
}

boost::signals2::connection Scheduler::do_on_frame(
		const on_frame_slot_t& slot)
{
	boost::signals2::connection c = _on_frame.connect(slot);
	_on_frame_connections.push_back(c);
	return c;
}

boost::signals2::connection Scheduler::do_on_end(
		const on_end_slot_t& slot)
{
	boost::signals2::connection c = _on_end.connect(slot);
	_on_end_connections.push_back(c);
	return c;
}

void Scheduler::disconnect_on_frame()
{
	for(std::list<boost::signals2::connection>::iterator it = _on_frame_connections.begin(); it != _on_frame_connections.end(); it++)
	{
		_debug && std::cout << "disconnect: " << &(*it) << std::endl;
		it->disconnect();
	}
	_on_frame_connections.clear();
}

void Scheduler::disconnect_on_end()
{
	for(std::list<boost::signals2::connection>::iterator it = _on_end_connections.begin(); it != _on_end_connections.end(); it++)
	{
		_debug && std::cout << "disconnect: " << &(*it) << std::endl;
		it->disconnect();
	}
	_on_end_connections.clear();
}
