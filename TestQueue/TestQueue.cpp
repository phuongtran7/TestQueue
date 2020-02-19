// TestQueue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "readerwriterqueue/readerwriterqueue.h"
#include "fmt/format.h"
#include <vector>
#include <thread>

auto main() -> int {
	std::cout << "Hello World!\n";

	moodycamel::ReaderWriterQueue<int> q(1);

	std::thread t1([&] {
		auto count = 0;
		while (1) {
			q.try_enqueue(count);
			count++;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		});

	std::thread t2([&] {
		while (1) {
			int result{};
			if (q.try_dequeue(result)) {
				fmt::print("{}\n", result);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		});

	t1.join();
	t2.join();
	return 0;
}