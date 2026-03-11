#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int nums[5] = {0, 1, 2, 3, 4};
    pid_t pid = fork();

    if (pid == 0) { // Child
        for (int i = 0; i < 5; i++) {
            nums[i] *= -i;
            std::cout << "CHILD: " << nums[i] << " "; // LINE X
        }
    } else if (pid > 0) { // Parent
        wait(NULL);
        for (int i = 0; i < 5; i++)
            std::cout << "PARENT: " << nums[i] << " "; // LINE Y
    }
    return 0;
}
