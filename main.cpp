#include <iostream>

#include "thread.h"
#include "fiber.h"



// A thread declaration child of the parent class 'thread'...
class main_thread : public thread
{
	public:
		void start() override
		{
			// Here is the virtual function 'start' overridden, it's executed right after the thread creation !
		}

		void update() override
		{
			// This one is updated every CPU tick ! (Not per frame)

			// In this case this thread is running the fibers pool, which means, if this function 'do_run' is not executed fibers don't do anything
			// this is why fiber are cool ! If the run function doesn't execute, fibers don't run !
			// It's really cool when you want to synchronize a fiber with a specific threads. (For example:
			// Let's say you want to run fibers in the "game render thread", you could just hook the "game render" function and run the fibers inside !
			// This way you have perfectly sinced fibers with the game rendering thread !
			fibers_pool::get()->do_run();
		}

		void stop() override
		{
			// ... and finally executed when the thread just stop !
		}
};



// A fiber declaration child of the parent class 'fiber'...
class main_fiber : public fiber
{
	public:
		void start(bool& _allow_update) override
		{
			// Uncomment this line if you don't want the update loop to be ran.
			// _allow_update = false;

			// Same thing as the thread class but for the fiber.

			std::cout << "Fiber just started !" << std::endl;
		}

		void update() override
		{
			// Here it's a little bit different, we need to use 'wait' (a custom "Sleep" function to wait a fiber)

			std::cout << "Fiber is running..." << std::endl;

			wait(100); // This one is not necessarily required, it's for the example, so we can see the ouput in the console better !
		}

		// There is no 'stop' function for a fiber, simply because when we stop a fiber it is simply erased (removed) from the fiber list.
		// Since the fiber is instantly cutted from running when removed, we could wait a bit to give time for the fiber to run a stop function
		// but it's useless anyway.
};



int main()
{
	// Create a thread to run fibers inside (To simulate fibers running in a game thread for example...)
	threads_pool::get()->emplace("main_thread", new main_thread());

	// Create a test fiber...
	fibers_pool::get()->emplace("main_fiber", new main_fiber());

	// Pause the program, until we press a key...
	system("pause");

	// Clear everything.
	fibers_pool::get()->clear();
	threads_pool::get()->clear();
}