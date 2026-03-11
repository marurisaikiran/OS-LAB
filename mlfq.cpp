#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    string id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    int lastServedTime; // For aging logic
    int queueLevel;     // 1, 2, or 3
};

void calculateMetrics(vector<Process>& processes) {
    float totalWT = 0, totalTAT = 0;
    cout << "\nTable Results:\n";
    cout << "ID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto& p : processes) {
        p.turnAroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnAroundTime - p.burstTime;
        totalWT += p.waitingTime;
        totalTAT += p.turnAroundTime;
        cout << p.id << "\t" << p.arrivalTime << "\t" << p.burstTime << "\t" 
             << p.completionTime << "\t" << p.turnAroundTime << "\t" << p.waitingTime << endl;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << totalWT / processes.size() << " ms";
    cout << "\nAverage Turnaround Time: " << totalTAT / processes.size() << " ms\n";
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    vector<Process*> completed;
    queue<Process*> q1, q2, q3;

    for (int i = 0; i < n; i++) {
        cout << "Enter ID, Arrival Time, Burst Time for P" << i+1 << ": ";
        cin >> processes[i].id >> processes[i].arrivalTime >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].queueLevel = 1;
        processes[i].lastServedTime = processes[i].arrivalTime;
    }

    // Sort by arrival time initially
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    int completedCount = 0;
    vector<pair<string, pair<int, int>>> gantt;

    while (completedCount < n) {
        // 1. Move arriving processes to Q1
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                bool alreadyInQueue = false;
                // Check if already in one of the queues
                // (In a real system, this is handled by state flags)
            }
        }
        
        // Push newly arrived processes to Q1
        static int lastIn = 0;
        while(lastIn < n && processes[lastIn].arrivalTime <= currentTime) {
            q1.push(&processes[lastIn]);
            lastIn++;
        }

        // 2. Aging Mechanism (Every cycle, check Q2 and Q3)
        // Check Q2 for promotion to Q1
        int q2_size = q2.size();
        for(int i=0; i<q2_size; i++) {
            Process* p = q2.front(); q2.pop();
            if (currentTime - p->lastServedTime >= 20) {
                p->queueLevel = 1;
                q1.push(p);
            } else {
                q2.push(p);
            }
        }
        // Check Q3 for promotion to Q2
        int q3_size = q3.size();
        for(int i=0; i<q3_size; i++) {
            Process* p = q3.front(); q3.pop();
            if (currentTime - p->lastServedTime >= 20) {
                p->queueLevel = 2;
                q2.push(p);
            } else {
                q3.push(p);
            }
        }

        // 3. Scheduling Execution
        Process* current = nullptr;
        int timeLimit = 0;

        if (!q1.empty()) {
            current = q1.front(); q1.pop();
            timeLimit = 4;
        } else if (!q2.empty()) {
            current = q2.front(); q2.pop();
            timeLimit = 8;
        } else if (!q3.empty()) {
            current = q3.front(); q3.pop();
            timeLimit = current->remainingTime; // FCFS
        } else {
            currentTime++; // Idle
            continue;
        }

        // Execute process
        int executionTime = min(current->remainingTime, timeLimit);
        
        // Handle preemption if in Q2/Q3 (Check if high priority arrives during execution)
        // Simplified for this simulation: we execute the quantum or until arrival
        int actualExec = 0;
        for(int i=0; i<executionTime; i++) {
            actualExec++;
            current->remainingTime--;
            currentTime++;
            
            // Check for new arrivals in Q1 that would preempt Q2 or Q3
            if (current->queueLevel > 1) {
                bool higherArrival = false;
                for(int j=lastIn; j<n; j++) {
                    if(processes[j].arrivalTime <= currentTime) {
                        higherArrival = true;
                        break;
                    }
                }
                if(higherArrival) break; 
            }
        }

        gantt.push_back({current->id, {currentTime - actualExec, currentTime}});
        current->lastServedTime = currentTime;

        if (current->remainingTime == 0) {
            current->completionTime = currentTime;
            completedCount++;
        } else {
            // Move down levels if quantum exceeded
            if (current->queueLevel == 1 && actualExec == 4) current->queueLevel = 2, q2.push(current);
            else if (current->queueLevel == 2 && actualExec == 8) current->queueLevel = 3, q3.push(current);
            else if (current->queueLevel == 1) q1.push(current);
            else if (current->queueLevel == 2) q2.push(current);
            else q3.push(current);
        }
    }

    // Output Gantt Chart
    cout << "\nGantt Chart:\n|";
    for (auto g : gantt) cout << "  " << g.first << "  |";
    cout << "\n0";
    for (auto g : gantt) cout << setw(7) << g.second.second;
    cout << "\n";

    calculateMetrics(processes);

    return 0;
}
