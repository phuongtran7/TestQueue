#include "readerwriterqueue/readerwriterqueue.h"
#include "fmt/format.h"
#include <thread>
#include <atomic>

auto main() -> int {
	moodycamel::ReaderWriterQueue<int> q(1);

	std::atomic_bool running { true };

	std::thread t1([&] {
		auto count = 0;
		while (running) {
			q.try_enqueue(count);
			count++;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		});

	std::thread t2([&] {
		while (running) {
			int result{};
			if (q.try_dequeue(result)) {
				fmt::print("{}\n", result);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		});

	std::getchar();
	running.store(false);
	t1.join();
	t2.join();
	return 0;
}