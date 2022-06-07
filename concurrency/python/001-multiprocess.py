# Python has no support for parallel threads so I am using multiprocessing
import multiprocessing
import multiprocessing.synchronize
import random
import time


def f(num: int, e: multiprocessing.synchronize.Event, q: multiprocessing.Queue):
    print(f"Hello from process {num}")
    while True:
        time.sleep(random.randint(1, 6))
        e.set()
        q.put(num)


def main():
    print("Hello from main")
    e = multiprocessing.Event()
    q = multiprocessing.Queue()
    p1 = multiprocessing.Process(target=f, args=(1, e, q))
    p2 = multiprocessing.Process(target=f, args=(2, e, q))
    p3 = multiprocessing.Process(target=f, args=(3, e, q))
    p1.start()
    p2.start()
    p3.start()
    while True:
        e.wait()
        e.clear()
        while not q.empty():
            print(f"Received from process {q.get()}")


if __name__ == "__main__":
    main()
