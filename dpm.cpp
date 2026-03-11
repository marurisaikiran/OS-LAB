#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

enum State { THINKING, HUNGRY, EATING };

class DiningMonitor {
private:
    int N;
    vector<State> state;
    vector<int> eatCount;
    mutex mtx;                  // The "Monitor" lock
    condition_variable cv;      // For wait() and notifyAll()

    void test(int i) {
        // Condition: Hungry AND neighbors are not eating
        if (state[i] == HUNGRY &&
            state[(i + N - 1) % N] != EATING &&
            state[(i + 1) % N] != EATING) {
            
            state[i] = EATING;
            cout << "--> Philosopher " << i << " starts EATING." << endl;
            // In C++, notifyAll() wakes up everyone to re-check their condition
            cv.notify_all();
        }
    }

public:
    DiningMonitor(int numPhilosophers) : N(numPhilosophers) {
        state.resize(N, THINKING);
        eatCount.resize(N, 0);
    }

    void pickUp(int id) {
        unique_lock<mutex> lock(mtx); // Entry to Monitor
        
        state[id] = HUNGRY;
        cout << "Philosopher " << id << " is HUNGRY (Waiting for forks)." << endl;

        test(id);

        // Wait until the state changes to EATING
        cv.wait(lock, [this, id] { return state[id] == EATING; });
    }

    void putDown(int id) {
        unique_lock<mutex> lock(mtx);
        
        state[id] = THINKING;
        eatCount[id]++;
        cout << "Philosopher " << id << " is THINKING. (Total meals: " << eatCount[id] << ")" << endl;

        // Check neighbors
        test((id + N - 1) % N);
        test((id + 1) % N);
    }
};

void philosopher(int id, DiningMonitor& monitor) {
    int mealLimit = 1;
    // Setup random number generation for sleep
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(500, 1500);

    for (int i = 0; i < mealLimit; ++i) {
        // THINKING
        cout << "Philosopher " << id << " is thinking..." << endl;
        this_thread::sleep_for(chrono::milliseconds(dis(gen)));

        // PICK UP
        monitor.pickUp(id);

        // EATING
        this_thread::sleep_for(chrono::milliseconds(dis(gen)));

        // PUT DOWN
        monitor.putDown(id);
    }
    cout << "!!! Philosopher " << id << " IS FULL AND LEAVING !!!" << endl;
}

int main() {
    int N = 5;
    DiningMonitor monitor(N);
    vector<thread> philosophers;

    cout << "--- Dinner Party Started (C++ Monitor Version) ---" << endl;

    for (int i = 0; i < N; ++i) {
        philosophers.push_back(thread(philosopher, i, ref(monitor)));
    }

    for (auto& t : philosophers) {
        t.join(); // Wait for all threads to finish
    }

    return 0;
}
