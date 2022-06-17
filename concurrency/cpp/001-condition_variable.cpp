#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>

void f(int n, std::condition_variable &cv) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution distrib(1, 6);

  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(distrib(gen)));

    cv.notify_all();
  }
}

int main(void) {
  std::mutex m;
  std::condition_variable cv;
  std::unique_lock<std::mutex> lk = std::unique_lock<std::mutex>(m);
  int v = 0;

  std::cout << "This is main()" << '\n';

  std::cout << "Starting thread" << '\n';
  auto t = std::thread(f, 1, ref(cv));

  t.detach();

  while (1) {
    cv.wait(lk);

    std::cout << "Received from thread" << '\n';
  }

  return 0;
}
