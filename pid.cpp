#include <iostream>
#include <vector>

#define MIN_PID 300
#define MAX_PID 5000

// Global bitmap
std::vector<bool> pid_map;

// Creates and initializes the map
int allocate_map() {
    pid_map.assign(MAX_PID + 1, false); // Initialize all to 0 (available)
    return 1;
}

// Allocates and returns a pid
int allocate_pid() {
    for (int i = MIN_PID; i <= MAX_PID; i++) {
        if (!pid_map[i]) {
            pid_map[i] = true; // Mark as in use
            return i;
        }
    }
    return -1; // All pids in use
}

// Releases a pid
void release_pid(int pid) {
    if (pid >= MIN_PID && pid <= MAX_PID) {
        pid_map[pid] = false; // Release pid
        std::cout << "Successfully released PID: " << pid << std::endl;
    }
}

int main() {
    // 1. Initialize the map
    if (allocate_map() == 1) {
        std::cout << "PID Manager initialized." << std::endl;
    }

    // 2. Allocate three PIDs
    int p1 = allocate_pid();
    int p2 = allocate_pid();
    int p3 = allocate_pid();

    std::cout << "First PID: " << p1 << std::endl;
    std::cout << "Second PID: " << p2 << std::endl;
    std::cout << "Third PID: " << p3 << std::endl;

    // 3. Release the middle PID
    release_pid(p2);

    // 4. Allocate again - it should reuse the released PID
    int p4 = allocate_pid();
    std::cout << "Next allocated PID (should be " << p2 << "): " << p4 << std::endl;

    return 0;
}
