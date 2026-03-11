#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <vector>
#include <chrono>

using namespace std;

// Shared Resources
int shared_data = 0;
int reader_count = 0;

mutex count_mutex;      // Protects the reader_count variable
mutex resource_mutex;   // Protects the shared resource (Writer Lock)

void reader(int id) {
    for (int i = 0; i < 3; i++) {
        // --- Entry Section ---
        count_mutex.lock();
        reader_count++;
        if (reader_count == 1) {
            resource_mutex.lock(); // First reader locks the resource for all readers
        }
        count_mutex.unlock();

        // --- Critical Section (Reading) ---
        cout << "Reader " << id << " read data: " << shared_data << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));

        // --- Exit Section ---
        count_mutex.lock();
        reader_count--;
        if (reader_count == 0) {
            resource_mutex.unlock(); // Last reader unlocks the resource for writers
        }
        count_mutex.unlock();
        
        this_thread::sleep_for(chrono::milliseconds(1500));
    }
}

void writer(int id) {
    for (int i = 0; i < 2; i++) {
        // --- Entry Section ---
        resource_mutex.lock(); // Writers need exclusive access

        // --- Critical Section (Writing) ---
        shared_data += 10;
        cout << "Writer " << id << " modified data to: " << shared_data << endl;
        this_thread::sleep_for(chrono::milliseconds(2000));

        // --- Exit Section ---
        resource_mutex.unlock();
        
        this_thread::sleep_for(chrono::milliseconds(2500));
    }
}

int main() {
    vector<thread> threads;

    // Create 3 Readers and 2 Writers
    for (int i = 1; i <= 3; i++) threads.push_back(thread(reader, i));
    for (int i = 1; i <= 2; i++) threads.push_back(thread(writer, i));

    for (auto& t : threads) t.join();

    cout << "Simulation finished." << endl;
    return 0;
}
