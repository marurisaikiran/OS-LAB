#include <iostream>
#include <vector>

using namespace std;

// Function to calculate the Need Matrix
void calculateNeed(vector<vector<int>>& need, vector<vector<int>>& max, vector<vector<int>>& alloc, int P, int R) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

// Function to check if the current state is safe (Safety Algorithm)
bool isSafe(int P, int R, vector<int> avail, vector<vector<int>> max, vector<vector<int>> alloc) {
    vector<vector<int>> need(P, vector<int>(R));
    calculateNeed(need, max, alloc, P, R);

    vector<bool> finish(P, false);
    vector<int> safeSeq(P);
    vector<int> work = avail;

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (need[p][j] > work[j])
                        break;
                }

                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += alloc[p][k];
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }

    cout << "\n[System is in a SAFE state]";
    cout << "\nSafe sequence: ";
    for (int i = 0; i < P; i++)
        cout << "P" << safeSeq[i] << (i == P - 1 ? "" : " -> ");
    cout << endl;
    return true;
}

// The "Modified" part: Resource-Request Algorithm
void resourceRequest(int P, int R, vector<int>& avail, vector<vector<int>>& max, vector<vector<int>>& alloc) {
    int pid;
    cout << "\n--- Resource Request Scenario ---";
    cout << "\nEnter Process ID making the request (0 to " << P - 1 << "): ";
    cin >> pid;

    vector<int> request(R);
    cout << "Enter request for each resource type: ";
    for (int i = 0; i < R; i++) cin >> request[i];

    vector<vector<int>> need(P, vector<int>(R));
    calculateNeed(need, max, alloc, P, R);

    // Step 1: Check if request <= Need
    for (int i = 0; i < R; i++) {
        if (request[i] > need[pid][i]) {
            cout << "Error: Process has exceeded its maximum claim!\n";
            return;
        }
    }

    // Step 2: Check if request <= Available
    for (int i = 0; i < R; i++) {
        if (request[i] > avail[i]) {
            cout << "Resources not available. Process P" << pid << " must wait.\n";
            return;
        }
    }

    // Step 3: Pretend to allocate (Trial Allocation)
    for (int i = 0; i < R; i++) {
        avail[i] -= request[i];
        alloc[pid][i] += request[i];
    }

    // Step 4: Check if this new state is safe
    if (isSafe(P, R, avail, max, alloc)) {
        cout << "SUCCESS: Request granted. Resources allocated to P" << pid << ".\n";
    } else {
        // Rollback if unsafe
        for (int i = 0; i < R; i++) {
            avail[i] += request[i];
            alloc[pid][i] -= request[i];
        }
        cout << "FAILURE: Request denied. System would become unsafe.\n";
    }
}

int main() {
    int P, R;
    cout << "Enter number of processes: "; cin >> P;
    cout << "Enter number of resource types: "; cin >> R;

    vector<int> avail(R);
    cout << "Enter Available Resources: ";
    for (int i = 0; i < R; i++) cin >> avail[i];

    vector<vector<int>> alloc(P, vector<int>(R));
    cout << "Enter Allocation Matrix:\n";
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++) cin >> alloc[i][j];

    vector<vector<int>> max(P, vector<int>(R));
    cout << "Enter Maximum Matrix:\n";
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++) cin >> max[i][j];

    // Initial safety check
    if (isSafe(P, R, avail, max, alloc)) {
        // Trigger the modified request algorithm
        resourceRequest(P, R, avail, max, alloc);
    } else {
        cout << "\nInitial system state is already UNSAFE (Deadlock possible).\n";
    }

    return 0;
}
