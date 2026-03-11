#include <iostream>
#include <vector>
#include <queue>
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
};

int main() {
    // Hardcoded data matching your previous structure
    vector<Process> p = {
        {"P1", 10, 0}, {"P2", 5, 2}, {"P3", 8, 4}, {"P4", 6, 6},
        {"P5", 10, 10}, {"P6", 13, 1}, {"P7", 7, 9}, {"P8", 9, 7},
        {"P9", 2, 3}, {"P10", 8, 12}
    };

    int n = p.size();
    int timeQuantum = 2; // Fixed slice of time for each process
    
    // Sort by arrival time initially
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    for (int i = 0; i < n; i++) p[i].remainingTime = p[i].burstTime;

    int completed = 0, currentTime = 0;
    queue<int> readyQueue;
    vector<bool> visited(n, false);
    vector<pair<string, int>> ganttChart;

    // Start with the first process that arrives
    readyQueue.push(0);
    visited[0] = true;

    while (completed != n) {
        if (readyQueue.empty()) {
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (p[i].arrivalTime <= currentTime && !visited[i]) {
                    readyQueue.push(i);
                    visited[i] = true;
                }
            }
            continue;
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        ganttChart.push_back({p[idx].id, currentTime});

        // Execute for Time Quantum or remaining time, whichever is smaller
        int executeTime = min(p[idx].remainingTime, timeQuantum);
        p[idx].remainingTime -= executeTime;
        currentTime += executeTime;

        // Check for new arrivals while the current process was running
        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && !visited[i]) {
                readyQueue.push(i);
                visited[i] = true;
            }
        }

        if (p[idx].remainingTime > 0) {
            // If not finished, put it back at the end of the queue
            readyQueue.push(idx);
        } else {
            // Process finished
            p[idx].completionTime = currentTime;
            p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
            completed++;
        }
    }
    ganttChart.push_back({"END", currentTime});

    // Output results
    cout << "Round Robin (Time Quantum = " << timeQuantum << ")\n";
    cout << "1. Gantt Chart:\n|";
    for (int i = 0; i < ganttChart.size() - 1; i++) cout << "  " << ganttChart[i].first << "  |";
    cout << "\n";
    for (int i = 0; i < ganttChart.size(); i++) cout << ganttChart[i].second << "    ";
    
    cout << "\n\n" << left << setw(8) << "PID" << setw(8) << "CT" << setw(8) << "TAT" << setw(8) << "WT" << endl;
    double totalTAT = 0, totalWT = 0;
    for (const auto& proc : p) {
        cout << left << setw(8) << proc.id << setw(8) << proc.completionTime << setw(8) << proc.turnaroundTime << setw(8) << proc.waitingTime << endl;
        totalTAT += proc.turnaroundTime;
        totalWT += proc.waitingTime;
    }

    cout << fixed << setprecision(2) << "\nAvg WT: " << totalWT/n << " ms\nAvg TAT: " << totalTAT/n << " ms" << endl;

    return 0;
}
