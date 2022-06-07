import java.util.Random;
import java.util.concurrent.SynchronousQueue;

public class MainClass {
    static class TC implements Runnable {
        final int num;
        final SynchronousQueue<Integer> q;

        public TC(int n, SynchronousQueue<Integer> queue) {
            this.num = n;
            this.q = queue;
        }

        public void run() {
            var r = new Random();

            System.out.format("Hello from thread %d\n", this.num);

            while (true) {
                try {
                    Thread.sleep(1000 * (r.nextInt(5) + 1));
                    q.put(this.num);
                } catch (InterruptedException e) {}
            }
        }
    }

    public static void main(String[] args) {
        var q = new SynchronousQueue<Integer>();
        var t1 = (new Thread(new TC(1, q)));
        var t2 = (new Thread(new TC(2, q)));
        var t3 = (new Thread(new TC(3, q)));

        System.out.println("Hello from main thread");

        t1.start();
        t2.start();
        t3.start();

        while (true) {
            try {
                System.out.format("Received from thread %d\n", q.take());
            } catch (InterruptedException e) {}
        }
    }
}
