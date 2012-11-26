#ifndef FULLCIRCLE_DMXCLIENT_HPP
#define FULLCIRCLE_DMXCLIENT_HPP

#include <ola/StreamingClient.h>
#include <ola/DmxBuffer.h>

#include <boost/multi_array.hpp>

#include "scheduler.hpp"

namespace fullcircle {
	class DmxClient {
		public:
			DmxClient(fullcircle::Scheduler::Ptr scheduler);
			void setUniverse(int universe);
			void setMapping(std::map< int, std::map<int, int> > *map);
			void setDebug(bool debug);
			void start();
			void stop();
		private:
			void play();
			static void *play_helper(void *client);
			uint16_t posMap(uint16_t x, uint16_t y);
			std::map< int, std::map<int, int> > *_addressMap;
			bool _debug;
			bool _running;
			pthread_t _thread;
			int _universe;
			fullcircle::Scheduler::Ptr _scheduler;
	};
}

#endif
