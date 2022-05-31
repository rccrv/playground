#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <span>
#include <tuple>
#include <vector>

std::tuple<std::span<int>, std::span<int>> part(std::span<int> &a) {
  int pivot = a.size() - 1;
  int j = 0;

  for (int i = 0; i < a.size(); i++) {
    if (a[i] < a[pivot]) {
      std::swap(a[i], a[j]);
      j++;
    }
  }

  std::swap(a[j], a[pivot]);
  std::span<int> n1{a.data(), a.data() + j};
  std::span<int> n2{a.data() + j + 1, a.data() + a.size()};

  return std::make_tuple(n1, n2);
}

void quicksort(std::span<int> &a) {
  if (a.size() > 1) {
    auto [p1, p2] = part(a);
    quicksort(p1);
    quicksort(p2);
  }
}

int main(void) {
  std::vector<int> a = {};
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib_size(10, 50);
  std::uniform_int_distribution<int> distrib_elements(1, 100);

  auto print_a = [&]() {
    std::cout << "[";
    for (int i = 0; i < a.size(); i++) {
      std::cout << a[i] << (i + 1 < a.size() ? ", " : "");
    }
    std::cout << "]\n";
  };

  auto generate_a = [&]() {
    int n = distrib_size(gen);
    for (int i = 0; i < n; i++) {
      a.push_back(distrib_elements(gen));
    }
  };

  generate_a();
  print_a();
  std::span<int> s{a.data(), a.size()};
  quicksort(s);
  print_a();

  return 0;
}
