import random
from typing import List


def quicksort(v: List[int]) -> List[int]:
    if len(v) > 1:
        p1 = [i for i in v[:-1] if i < v[-1]]
        p2 = [i for i in v[:-1] if i >= v[-1]]
        return quicksort(p1) + [v[-1]] + quicksort(p2)
    return v


def __main__():
    v = [random.randint(1, 101) for _ in range(random.randint(10, 51))]
    print(v)
    v = quicksort(v)
    print(v)


if __name__ == "__main__":
    __main__()
