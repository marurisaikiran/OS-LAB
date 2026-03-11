#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Error occurred
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << ") is exiting..." << std::endl;
        exit(0); 
    } else {
        // Parent process
        std::cout << "Parent process (PID: " << getpid() << ") is sleeping..." << std::endl;
        std::cout << "Child process (PID: " << pid << ") should now be a zombie." << std::endl;

        // Sleep for 20 seconds to allow time to run 'ps -l'
        sleep(20);

        std::cout << "Parent waking up and exiting." << std::endl;
    }

    return 0;
}
