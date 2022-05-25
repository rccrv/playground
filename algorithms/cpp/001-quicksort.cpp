#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

// TODO: I mainly converted this from an old C code that I had.
//       I probably should redesign part and quicksort using iterators instead
//       of rellying on indexes and make this code more C++er.
int part(std::vector<int> &a, int start, int end) {
  int pivot = end;
  int j = start;

  for (int i = start; i < end; i++) {
    if (a[i] < a[pivot]) {
      std::swap(a[i], a[j]);
      j++;
    }
  }

  std::swap(a[j], a[pivot]);

  return j;
}

void quicksort(std::vector<int> &a, int start, int end) {
  int p = 0;

  if (start < end) {
    p = part(a, start, end);
    quicksort(a, start, p - 1);
    quicksort(a, p + 1, end);
  }
}

int main(void) {
  std::vector<int> a = {};
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib_size(10, 50);
  std::uniform_int_distribution<int> distrib_elements(1, 100);

  auto print_a = [&]() {
    std::cout << "a: ";
    for (int i = 0; i < a.size(); i++) {
      std::cout << a[i] << " ";
    }
    std::cout << '\n';
  };

  auto generate_a = [&]() {
    int n = distrib_size(gen);
    for (int i = 0; i < n; i++) {
      a.push_back(distrib_elements(gen));
    }
  };

  generate_a();
  print_a();
  quicksort(a, 0, a.size() - 1);
  print_a();

  return 0;
}
