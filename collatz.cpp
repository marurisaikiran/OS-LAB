#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc < 2 || std::stoi(argv[1]) <= 0) {
        std::cerr << "Error: Enter a positive integer." << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);
    pid_t pid = fork();

    if (pid == 0) { // Child
        std::cout << n;
        while (n != 1) {
            if (n % 2 == 0) n /= 2;
            else n = 3 * n + 1;
            std::cout << ", " << n;
        }
        std::cout << std::endl;
    } else {
        wait(NULL); // Parent waits
    }
    return 0;
}

