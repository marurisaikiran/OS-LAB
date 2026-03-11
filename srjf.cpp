#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process {
    string id;
    int burstTime;
    int arrivalTime;
    int remainingTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    bool isCompleted = false;
};

int main() {
    // Hardcoded Data from User
    vector<Process> p = {
        {"P1", 10, 0}, {"P2", 5, 2}, {"P3", 8, 4}, {"P4", 6, 6},
        {"P5", 10, 10}, {"P6", 13, 1}, {"P7", 7, 9}, {"P8", 9, 7},
        {"P9", 2, 3}, {"P10", 8, 12}
    };

    int n = p.size();
    for (int i = 0; i < n; i++) p[i].remainingTime = p[i].burstTime;

    int completed = 0, currentTime = 0, prevProcess = -1;
    vector<pair<string, int>> ganttChart;

    while (completed != n) {
        int idx = -1;
        int minRemaining = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && !p[i].isCompleted) {
                if (p[i].remainingTime < minRemaining) {
                    minRemaining = p[i].remainingTime;
                    idx = i;
                }
                if (p[i].remainingTime == minRemaining) {
                    if (idx == -1 || p[i].arrivalTime < p[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            if (prevProcess != idx) {
                ganttChart.push_back({p[idx].id, currentTime});
            }
            p[idx].remainingTime--;
            currentTime++;
            prevProcess = idx;

            if (p[idx].remainingTime == 0) {
                p[idx].completionTime = currentTime;
                p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
                p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
                p[idx].isCompleted = true;
                completed++;
            }
        } else {
            currentTime++;
        }
    }
    ganttChart.push_back({"END", currentTime});

    // 1. Gantt Chart Output
    cout << "1. Gantt Chart:\n|";
    for (int i = 0; i < ganttChart.size() - 1; i++) cout << "  " << ganttChart[i].first << "  |";
    cout << "\n";
    for (int i = 0; i < ganttChart.size(); i++) cout << ganttChart[i].second << "     ";
    cout << "\n\n";

    // 2-4. Process Table
    cout << left << setw(8) << "PID" << setw(8) << "CT" << setw(8) << "TAT" << setw(8) << "WT" << endl;
    double totalTAT = 0, totalWT = 0;
    for (const auto& proc : p) {
        cout << left << setw(8) << proc.id << setw(8) << proc.completionTime 
             << setw(8) << proc.turnaroundTime << setw(8) << proc.waitingTime << endl;
        totalTAT += proc.turnaroundTime;
        totalWT += proc.waitingTime;
    }

    // 5-6. Averages
    cout << fixed << setprecision(2);
    cout << "\n5. Average Waiting Time: " << totalWT / n << " ms";
    cout << "\n6. Average Turnaround Time: " << totalTAT / n << " ms" << endl;

    return 0;
}
