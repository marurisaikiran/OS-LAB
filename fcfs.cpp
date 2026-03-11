#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    string id;
    int bt, at, ct, tat, wt;
};

int main() {
    // Hardcoded data from your table
    vector<Process> p = {
        {"P1", 10, 0}, {"P6", 13, 1}, {"P2", 5, 2}, {"P9", 2, 3}, 
        {"P3", 8, 4},  {"P4", 6, 6},  {"P8", 9, 7}, {"P7", 7, 9}, 
        {"P5", 10, 10}, {"P10", 8, 12}
    };

    // Sort by Arrival Time
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.at < b.at;
    });

    int currentTime = 0;
    vector<pair<string, int>> gantt;

    for (int i = 0; i < p.size(); i++) {
        if (currentTime < p[i].at) currentTime = p[i].at;
        currentTime += p[i].bt;
        p[i].ct = currentTime;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        gantt.push_back({p[i].id, currentTime});
    }

    // Output Table and Gantt Chart
    cout << "--- FCFS SCHEDULING ---\nID\tAT\tBT\tCT\tTAT\tWT\n";
    float sumWT = 0, sumTAT = 0;
    for (auto x : p) {
        sumWT += x.wt; sumTAT += x.tat;
        cout << x.id << "\t" << x.at << "\t" << x.bt << "\t" << x.ct << "\t" << x.tat << "\t" << x.wt << endl;
    }
    cout << "\nAverage WT: " << sumWT/10 << "\nAverage TAT: " << sumTAT/10 << endl;
    return 0;
}
