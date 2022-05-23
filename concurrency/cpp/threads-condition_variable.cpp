#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

void f(int n, std::queue<int> &q, std::condition_variable &cv) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(1, 6);

  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(distrib(gen)));

    q.push(n);
    cv.notify_all();
  }
}

int main(void) {
  std::mutex m;
  std::condition_variable cv;
  std::unique_lock<std::mutex> lk = std::unique_lock<std::mutex>(m);
  std::queue<int> q;
  int v = 0;

  std::cout << "This is main()" << '\n';

  std::cout << "Starting thread f1()" << '\n';
  auto t1 = std::thread(f, 1, ref(q), ref(cv));
  std::cout << "Starting thread f2()" << '\n';
  auto t2 = std::thread(f, 2, ref(q), ref(cv));
  std::cout << "Starting thread f3()" << '\n';
  auto t3 = std::thread(f, 3, ref(q), ref(cv));

  t1.detach();
  t2.detach();
  t3.detach();

  while (1) {
    cv.wait(lk);

    while (q.size() > 0) {
      v = q.front();
      q.pop();

      std::cout << "Received from f" << v << "()" << '\n';
    }
  }

  return 0;
}
