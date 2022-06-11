#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

void merge(std::vector<int> &a, std::vector<int> &b, int low, int middle, int high) {
  int il = low;
  int ir = middle + 1;
  int bi = low;

  while (il <= middle && ir <= high) {
    if (a[il] <= a[ir]) {
      b[bi++] = a[il++];
    } else {
      b[bi++] = a[ir++];
    }
  }

  while (il <= middle) {
    b[bi++] = a[il++];
  }

  while (ir <= high) {
    b[bi++] = a[ir++];
  }

  for (bi = low; bi <= high; bi++) {
    a[bi] = b[bi];
  }
}

void mergesort(std::vector<int> &a, std::vector<int> &b, int low, int high) {
  if (low < high) {
    int middle = (low + high) / 2;
    mergesort(a, b, low, middle);
    mergesort(a, b, middle + 1, high);
    merge(a, b, low, middle, high);
  }
}

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib_size(10, 50);
  std::uniform_int_distribution<int> distrib_elements(1, 100);
  std::vector<int> a;
  std::vector<int> b;

  auto generate_a = [&]() {
    int n = distrib_size(gen);
    for (int i = 0; i < n; i++) {
      a.push_back(distrib_elements(gen));
    }
  };
  auto print_a = [&]() {
    std::cout << "[";
    for (int i = 0; i < a.size(); i++) {
      std::cout << a[i] << (i + 1 < a.size() ? ", " : "");
    }
    std::cout << "]\n";
  };

  generate_a();
  b.resize(a.size());

  print_a();
  mergesort(a, b, 0, a.size() - 1);
  print_a();

  return 0;
}
