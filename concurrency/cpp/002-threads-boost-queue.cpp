#include <boost/lockfree/policies.hpp>
#include <boost/lockfree/queue.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

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
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib_size(10, 32);
  std::vector<std::thread> t;
  int v = 0;

  std::cout << "This is main()" << '\n';
  v = distrib_size(gen);

  for (int i = 0; i < v; i++) {
    std::cout << "Starting thread f" << i << "()" << '\n';
    t.push_back(std::thread(f, i + 1, std::ref(q)));
    t.back().detach();
  }

  while (1) {
    while (q.pop(v)) {
      std::cout << "Received from f" << v << "()" << '\n';
    }
  }

  return 0;
}
