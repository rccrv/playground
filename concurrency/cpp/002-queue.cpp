#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <thread>

struct queue {
  std::queue<int> queue;
  std::atomic_flag has_element = ATOMIC_FLAG_INIT;
  std::mutex m;

  void push(int n) {
    m.lock();
    queue.push(n);
    m.unlock();

    has_element.test_and_set();
    has_element.notify_all();
  }

  void pop(int &r) {
    has_element.wait(false);

    m.lock();
    r = queue.front();
    queue.pop();
    m.unlock();

    if (queue.size() == 0) {
      has_element.clear();
    }
  }
};

void f(int n, queue &q) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution distrib(1, 6);

  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(distrib(gen)));

    q.push(n);
  }
}

int main(void) {
  queue q;
  int v = 0;

  std::cout << "This is main()" << '\n';

  std::cout << "Starting thread 1" << '\n';
  auto t1 = std::thread(f, 1, std::ref(q));
  std::cout << "Starting thread 2" << '\n';
  auto t2 = std::thread(f, 2, std::ref(q));
  std::cout << "Starting thread 3" << '\n';
  auto t3 = std::thread(f, 3, std::ref(q));

  t1.detach();
  t2.detach();
  t3.detach();

  while (1) {
    q.pop(v);
    std::cout << "Received from " << v << '\n';
  }

  return 0;
}
