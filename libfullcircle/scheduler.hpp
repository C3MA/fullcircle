#ifndef FULLCIRCLE_SCHEDULER_HPP
#define FULLCIRCLE_SCHEDULER_HPP

#include <libfullcircle/sequence.hpp>

namespace fullcircle {
	class Scheduler { //Singleton
		public: 
			static Scheduler& getInstance();
			static void setInput(std::string dir);
			static void setPriority(std::string dir);
			static void setFallback(std::string dir);
			static void setOutput(std::string dir);
			static void setDebug(bool debug);
			Sequence::Ptr getNextSequence();

		private:
			Scheduler();
			Scheduler(const Scheduler&);
			~Scheduler();

			static Scheduler *instance;
			static bool _debug;
			static std::string srcdir, falldir, priodir, dstdir;
	};
}

#endif
