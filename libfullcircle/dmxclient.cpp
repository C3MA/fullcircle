#include "dmxclient.hpp"

#include <ola/Logging.h>

#include "scheduler.hpp"

using namespace fullcircle;

DmxClient::DmxClient()
{
	_universe = 1;
	_debug = false;
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

	ola::DmxBuffer buffer;
	// set all channels to 0
	buffer.Blackout();

	ola::StreamingClient olaClient;
	// Setup the client, this connects to the server
	if (!olaClient.Setup()) {
		std::cerr << "Setup failed" << std::endl;
		exit(1);
	}
	_debug && std::cout << "DmxClient started" << std::endl;
	while ( _running )
	{
		Sequence::Ptr seq;
		seq = Scheduler::getInstance().getNextSequence();
		if ( seq )
		{
			// calculate inter-frame-spacing
			double ifs = 1000/seq->fps();
			// iterate over all frames in the corrent sequence

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
							buffer.SetChannel( addr    , pixel.red   % 256 );
							buffer.SetChannel( addr + 1, pixel.green % 256 );
							buffer.SetChannel( addr + 2, pixel.blue  % 256 );
						}
					}
				}

				if (!olaClient.SendDmx(_universe, buffer)) {
					std::cerr << "Sending data to DMX failed!" << std::endl;
					exit(1);
				}

				// one DMX datagram of 512 slots takes about 23 ms to transmit
				usleep(ifs*1000);
			}

			/*buffer.Blackout();

			if (!olaClient.SendDmx(_universe, buffer)) {
				std::cerr << "Sending data to DMX failed!" << std::endl;
				exit(1);
			}

			sleep(1);*/
		} else {
			_debug && std::cout << "Waiting for new sequence" << std::endl;
			sleep(5);
		}
	}
	olaClient.Stop();

	pthread_exit(NULL);
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
