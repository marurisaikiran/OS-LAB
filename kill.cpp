#include <iostream>
#include <signal.h>
using namespace std;

int main() {
    int pid;
    cout << "Enter PID: ";
    cin >> pid;

    kill(pid, SIGKILL);
    cout << "Process terminated successfully\n";
    return 0;
}
