#include "scheduler.hpp"

#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>

namespace bfs=boost::filesystem;

using namespace fullcircle;

Scheduler* Scheduler::instance = 0;
std::string Scheduler::srcdir = "";
std::string Scheduler::priodir = "";
std::string Scheduler::falldir = "";
std::string Scheduler::dstdir = "";
bool Scheduler::_debug = false;

Scheduler& Scheduler::getInstance()
{
	if ( !instance )
		instance = new Scheduler();

	return *instance;
}

void Scheduler::setInput(std::string dir)
{
	srcdir = dir;
}

void Scheduler::setPriority(std::string dir)
{
	priodir = dir;
}

void Scheduler::setFallback(std::string dir)
{
	falldir = dir;
}

void Scheduler::setOutput(std::string dir)
{
	dstdir = dir;
}

void Scheduler::setDebug(bool debug)
{
	_debug = debug;
}

Scheduler::Scheduler()
{
}

Sequence::Ptr Scheduler::getNextSequence()
{
	try {
		bfs::path source(srcdir);
		bfs::path priosrc;
		bfs::path fallsrc;
		bfs::path destination(dstdir);
		bfs::path srcfile;

		bool move = true;

		if ( !priodir.empty() )
			priosrc = bfs::path(priodir);

		if ( !falldir.empty() )
			fallsrc = bfs::path(falldir);

		try {
			_debug && std::cout << "Loading sequence files from " << srcdir << std::endl;

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
