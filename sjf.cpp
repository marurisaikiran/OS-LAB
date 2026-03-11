#include <iostream>
#include <vector>
#include <iomanip>
#include <climits>

using namespace std;

struct Process {
    string id;
    int bt, at, ct, tat, wt;
    bool isCompleted = false;
};

int main() {
    // Hardcoded data from your table
    vector<Process> p = {
        {"P1", 10, 0}, {"P2", 5, 2}, {"P3", 8, 4}, {"P4", 6, 6}, {"P5", 10, 10},
        {"P6", 13, 1}, {"P7", 7, 9}, {"P8", 9, 7}, {"P9", 2, 3}, {"P10", 8, 12}
    };

    int n = 10, completed = 0, currentTime = 0;
    float totalWT = 0, totalTAT = 0;

    cout << "--- SJF (Non-Preemptive) Scheduling ---" << endl;

    while (completed < n) {
        int idx = -1;
        int minBT = INT_MAX;

        // 1. Search for the shortest job that has already arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && !p[i].isCompleted) {
                if (p[i].bt < minBT) {
                    minBT = p[i].bt;
                    idx = i;
                }
            }
        }

        // 2. If a process is found, execute it
        if (idx != -1) {
            currentTime += p[idx].bt;
            p[idx].ct = currentTime;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].isCompleted = true;
            completed++;

            totalWT += p[idx].wt;
            totalTAT += p[idx].tat;
        } else {
            // If no process has arrived yet, move time forward
            currentTime++;
        }
    }

    // 3. Print Results
    cout << "ID\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << p[i].id << "\t" << p[i].at << "\t" << p[i].bt << "\t"
             << p[i].ct << "\t" << p[i].tat << "\t" << p[i].wt << endl;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << totalWT / n << " ms";
    cout << "\nAverage Turnaround Time: " << totalTAT / n << " ms" << endl;

    return 0;
}
