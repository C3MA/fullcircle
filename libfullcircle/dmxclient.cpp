#include "dmxclient.hpp"

#include <ola/Logging.h>

using namespace fullcircle;

DmxClient::DmxClient(fullcircle::Scheduler::Ptr scheduler)
	: _addressMap(NULL)
	, _debug(false)
	, _running(false)
	, _universe(1)
	, _scheduler(scheduler)
{
}

void *DmxClient::play_helper(void* client)
{
	((DmxClient*) client)->play();
	return NULL;
}

uint16_t DmxClient::posMap(uint16_t x, uint16_t y)
{
	// compensate for Ola mapping channel 0 to DMX channel 1
	return (*_addressMap)[x][y] - 1;
}

void DmxClient::play()
{
	// enable logging
	ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);

	//ola::Dmx_buffer _buffer;
	// set all channels to 0
	_buffer.Blackout();

	_olaClient = new ola::StreamingClient();
	// Setup the client, this connects to the server
	if (!_olaClient->Setup()) {
		std::cerr << "Setup failed" << std::endl;
		exit(1);
	}
	_debug && std::cout << "DmxClient started" << std::endl;
	while ( _running )
	{
		_debug && std::cout << "Locking mutex " << &_playing << std::endl;
		_playing.lock();
		Sequence::Ptr seq;
		seq = _scheduler->getNextSequence();
		if ( seq )
		{
			// calculate inter-frame-spacing
			double ifs = 1000/seq->fps();
			// iterate over all frames in the corrent sequence

			if ( seq->size() > 0 )
			{
				playSequence(seq, ifs);
				_debug && std::cout << "Unlocking mutex " << &_playing << std::endl;
				_playing.unlock();
			} else {
				_scheduler->do_on_frame(boost::bind(&DmxClient::playFrame, this, _1));
				_scheduler->do_on_end(boost::bind(&DmxClient::seqFinished, this));
			}
		} else {
			_playing.unlock();
			_debug && std::cout << "Waiting for new sequence" << std::endl;
			sleep(5);
		}
	}
	_olaClient->Stop();

	pthread_exit(NULL);
}

void DmxClient::playSequence(Sequence::Ptr seq, double ifs)
{
	for ( uint32_t frameId = 0; frameId < seq->size(); frameId++ ) {
		_debug && std::cout << frameId;
		fullcircle::Frame::Ptr frame = seq->get_frame(frameId);
		_debug && std::cout << ": " << frame->width() << " x " << frame->height() << std::endl;
		for ( uint16_t y = 0; y < frame->height(); y++ ) {
			for ( uint16_t x = 0; x < frame->width(); x++ ) {
				uint16_t addr = posMap(x, y);
				if ( addr >= 0 )
				{
					fullcircle::RGB_t pixel = frame->get_pixel(x, y);
					_buffer.SetChannel( addr    , pixel.red   % 256 );
					_buffer.SetChannel( addr + 1, pixel.green % 256 );
					_buffer.SetChannel( addr + 2, pixel.blue  % 256 );
				}
			}
		}

		if (!_olaClient->SendDmx(_universe, _buffer)) {
			std::cerr << "Sending data to DMX failed!" << std::endl;
			exit(1);
		}

		// one DMX datagram of 512 slots takes about 23 ms to transmit
		usleep(ifs*1000);
	}
}

void DmxClient::playFrame(Frame::Ptr frame)
{
	_debug && std::cout << ": " << frame->width() << " x " << frame->height() << std::endl;
	for ( uint16_t y = 0; y < frame->height(); y++ ) {
		for ( uint16_t x = 0; x < frame->width(); x++ ) {
			uint16_t addr = posMap(x, y);
			if ( addr >= 0 )
			{
				fullcircle::RGB_t pixel = frame->get_pixel(x, y);
				_buffer.SetChannel( addr    , pixel.red   % 256 );
				_buffer.SetChannel( addr + 1, pixel.green % 256 );
				_buffer.SetChannel( addr + 2, pixel.blue  % 256 );
			}
		}
	}
	if (!_olaClient->SendDmx(_universe, _buffer)) {
		std::cerr << "Sending data to DMX failed!" << std::endl;
		exit(1);
	}
}

void DmxClient::seqFinished()
{
	_scheduler->disconnect_on_frame();
	_scheduler->disconnect_on_end();
	_debug && std::cout << "Sequence finished. Unlocking mutex " << &_playing << std::endl;
	_playing.unlock();
}

void DmxClient::start()
{
	_running = true;
	int rc = pthread_create(&_thread, NULL, play_helper, (void*) this);
	_debug && std::cout << "pthread_create: " << rc;
}

void DmxClient::stop()
{
	_running = false;
	pthread_join(_thread, NULL);
}

void DmxClient::setUniverse(int universe)
{
	_universe = universe;
}

void DmxClient::setMapping(std::map< int, std::map<int, int> > *map)
{
	_addressMap = map;
}

void DmxClient::setDebug(bool debug)
{
	_debug = debug;
}
