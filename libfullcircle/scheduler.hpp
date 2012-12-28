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
			void seqEnd(ServerProtocolDispatcher::Ptr dispatcher);
			Sequence::Ptr getNextSequence();

			typedef boost::signals2::signal<void (fullcircle::Frame::Ptr)>     on_frame_t;
			typedef on_frame_t::slot_type on_frame_slot_t;
			boost::signals2::connection do_on_frame(const on_frame_slot_t& slot);

			typedef boost::signals2::signal<void (void)>     on_end_t;
			typedef on_end_t::slot_type on_end_slot_t;
			boost::signals2::connection do_on_end(const on_end_slot_t& slot);

		private:
			bool _debug;
			std::string _srcdir, _falldir, _priodir, _dstdir;
			boost::asio::io_service _server_io_service;
			NetServer::Ptr _server;
			ServerProtocolDispatcher::Ptr _dispatcher;
			std::queue< std::pair<BinarySequenceMetadata, ServerProtocolDispatcher::Ptr> > _net_queue;
			int _width, _height;
			on_frame_t _on_frame;
			on_end_t _on_end;
	};
}

#endif
