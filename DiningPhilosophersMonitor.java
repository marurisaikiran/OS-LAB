import java.util.concurrent.TimeUnit;

class DiningMonitor {
    private enum State { THINKING, HUNGRY, EATING }
    private final State[] state;
    private final int N;
    private final int[] eatCount; // Track how many times each eats

    public DiningMonitor(int numPhilosophers) {
        this.N = numPhilosophers;
        this.state = new State[N];
        this.eatCount = new int[N];
        for (int i = 0; i < N; i++) {
            state[i] = State.THINKING;
            eatCount[i] = 0;
        }
    }

    public synchronized void pickUp(int id) throws InterruptedException {
        state[id] = State.HUNGRY;
        System.out.println("Philosopher " + id + " is HUNGRY (Waiting for forks).");

        test(id);

        // If neighbors are eating, wait
        while (state[id] != State.EATING) {
            wait();
        }
    }

    public synchronized void putDown(int id) {
        state[id] = State.THINKING;
        eatCount[id]++; // Increment count after finishing
        System.out.println("Philosopher " + id + " is THINKING. (Total meals: " + eatCount[id] + ")");

        // Signal neighbors that forks are free
        test((id + N - 1) % N); // Left
        test((id + 1) % N);     // Right
        
        notifyAll();
    }

    private void test(int i) {
        if (state[i] == State.HUNGRY &&
            state[(i + N - 1) % N] != State.EATING &&
            state[(i + 1) % N] != State.EATING) {
            
            state[i] = State.EATING;
            System.out.println("--> Philosopher " + i + " starts EATING.");
        }
    }
}

class Philosopher implements Runnable {
    private final int id;
    private final DiningMonitor monitor;
    private final int mealLimit = 3; // Stops the "infinite loop" after 3 meals

    public Philosopher(int id, DiningMonitor monitor) {
        this.id = id;
        this.monitor = monitor;
    }

    @Override
    public void run() {
        try {
            for (int i = 1; i <= mealLimit; i++) {
                // THINKING
                System.out.println("Philosopher " + id + " is thinking...");
                Thread.sleep((long) (Math.random() * 2000)); 
                
                // PICK UP FORKS
                monitor.pickUp(id);
                
                // EATING
                Thread.sleep((long) (Math.random() * 2000));
                
                // PUT DOWN FORKS
                monitor.putDown(id);
            }
            System.out.println("!!! Philosopher " + id + " IS FULL AND LEAVING THE TABLE !!!");
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

public class DiningPhilosophersMonitor {
    public static void main(String[] args) {
        int N = 5;
        DiningMonitor monitor = new DiningMonitor(N);
        Thread[] philosophers = new Thread[N];

        System.out.println("--- Dinner Party Started (Limit: 3 meals each) ---");

        for (int i = 0; i < N; i++) {
            philosophers[i] = new Thread(new Philosopher(i, monitor));
            philosophers[i].start();
        }
    }
}
