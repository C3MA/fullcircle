#ifndef FULLCIRCLE_SCHEDULER_HPP
#define FULLCIRCLE_SCHEDULER_HPP

#include <libfullcircle/sequence.hpp>
#include <libfullcircle/net/net_server.hpp>

#include <queue>

namespace fullcircle {
	class Scheduler {
		public: 
			typedef boost::shared_ptr<Scheduler> Ptr;

			Scheduler();
			~Scheduler();
			void setInput(std::string dir);
			void setPriority(std::string dir);
			void setFallback(std::string dir);
			void setOutput(std::string dir);
			void setDebug(bool debug);
			void setPort(int port);
			void addConnection(Snip_RequestSnip request);
			void addFrame(Snip_FrameSnip frame);
			Sequence::Ptr getNextSequence();
			Frame::Ptr getNextFrame(Sequence::Ptr sequence);

		private:
			bool _debug;
			std::string _srcdir, _falldir, _priodir, _dstdir;
  		boost::asio::io_service _server_io_service;
			NetServer::Ptr _server;
			ServerProtocolDispatcher::Ptr _dispatcher;
			std::queue< std::pair<BinarySequenceMetadata, ServerProtocolDispatcher::Ptr> > _net_queue;
			BinaryFrame _net_frame;
	};
}

#endif
