#include <boost/lockfree/policies.hpp>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <boost/lockfree/queue.hpp>

void f(int n, boost::lockfree::queue<int> &q) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(1, 6);

  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(distrib(gen)));

    q.push(n);
  }
}

int main(void) {
  boost::lockfree::queue<int> q{50};
  int v = 0;

  std::cout << "This is main()" << '\n';

  std::cout << "Starting thread f1()" << '\n';
  auto t1 = std::thread(f, 1, std::ref(q));
  std::cout << "Starting thread f2()" << '\n';
  auto t2 = std::thread(f, 2, std::ref(q));
  std::cout << "Starting thread f3()" << '\n';
  auto t3 = std::thread(f, 3, std::ref(q));

  t1.detach();
  t2.detach();
  t3.detach();

  while (1) {
    while (q.pop(v)) {
      std::cout << "Received from f" << v << "()" << '\n';
    }
  }

  return 0;
}
