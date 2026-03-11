#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int value = 5; // Global variable [cite: 7]

int main() {
    pid_t pid = fork();

    if (pid == 0) { // Child process [cite: 12]
        value += 15;
        return 0;
    } else if (pid > 0) { // Parent process [cite: 16]
        wait(NULL);
        std::cout << "PARENT: value = " << value << std::endl; // LINE A [cite: 18]
    }
    return 0;
}
