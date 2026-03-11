#include <iostream>
#include <queue>
#include <thread>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 10 // Limit the loop

std::queue<int> buffer;
sem_t mtx, empty, full;

void producer() {
    for (int i = 0; i < TOTAL_ITEMS; ++i) {
        int item = rand() % 100;
        
        sem_wait(&empty);
        sem_wait(&mtx);
        
        buffer.push(item);
        std::cout << "[P] Produced: " << item << " | Size: " << buffer.size() << std::endl;
        
        sem_post(&mtx);
        sem_post(&full);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void consumer() {
    for (int i = 0; i < TOTAL_ITEMS; ++i) {
        sem_wait(&full);
        sem_wait(&mtx);
        
        int item = buffer.front();
        buffer.pop();
        std::cout << "[C] Consumed: " << item << " | Size: " << buffer.size() << std::endl;
        
        sem_post(&mtx);
        sem_post(&empty);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}

int main() {
    sem_init(&mtx, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    std::thread p(producer);
    std::thread c(consumer);

    p.join();
    c.join();

    sem_destroy(&mtx);
    sem_destroy(&empty);
    sem_destroy(&full);

    std::cout << "Done! All items processed." << std::endl;
    return 0;
}
