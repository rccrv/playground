# Python has no support for parallel threads so I am using multiprocessing
import multiprocessing
import random
import time


def f(num: int, q: multiprocessing.Queue):
    print(f"Hello from process {num}")

    while True:
        time.sleep(random.randint(1, 6))
        q.put(num)


def main():
    print("Hello from main")

    q = multiprocessing.Queue()
    p1 = multiprocessing.Process(target=f, args=(1, q))
    p2 = multiprocessing.Process(target=f, args=(2, q))
    p3 = multiprocessing.Process(target=f, args=(3, q))

    p1.start()
    p2.start()
    p3.start()

    while True:
        print(f"Received from process {q.get(block=True, timeout=None)}")


if __name__ == "__main__":
    main()
