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

    if (queue.empty()) {
      has_element.clear();
    }
  }
};

void producer(int n, std::atomic_flag &sender, std::atomic_flag &end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution distrib(1, 6);
  std::uniform_real_distribution frequency;

  while (1) {
    if (end.test()) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::seconds(distrib(gen)));

    if (frequency(gen) > 0.7) {
      std::cout << "producer " << n << " ping\n";
      sender.test_and_set();
      sender.notify_one();
    }
  }
}

void consumer(int n, queue &q, std::atomic_flag &receiver,
              std::atomic_flag &end) {
  while (1) {
    receiver.wait(false);

    std::cout << "consumer " << n << " pong\n";

    q.push(n);

    receiver.clear();
    if (end.test()) {
      break;
    }
  }
}

int main(void) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(5, 16);
  std::atomic_flag end = ATOMIC_FLAG_INIT;
  int r;
  int counter;
  int limit = 10;
  queue q;
  int n;

  n = distrib(gen);
  std::vector<std::atomic_flag> flags(n);

  for (int i = 0; i < n; i++) {
    flags[i].clear();
    auto p = std::thread(producer, i + 1, std::ref(flags[i]), std::ref(end));
    auto c = std::thread(consumer, i + 1, std::ref(q), std::ref(flags[i]),
                         std::ref(end));
    p.detach();
    c.detach();
  }

  for (counter = 0; counter < limit; counter++) {
    q.pop(r);
  }

  end.test_and_set();
  end.notify_all();
}
