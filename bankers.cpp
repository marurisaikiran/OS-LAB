#include <iostream>
#include <vector>

using namespace std;

void calculateNeed(vector<vector<int>>& need, vector<vector<int>>& max, vector<vector<int>>& alloc, int P, int R) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

bool isSafe(int P, int R, vector<int>& avail, vector<vector<int>>& max, vector<vector<int>>& alloc) {
    vector<vector<int>> need(P, vector<int>(R));
    calculateNeed(need, max, alloc, P, R);

    vector<bool> finish(P, false);
    vector<int> safeSeq(P);
    vector<int> work = avail;

    int count = 0; while (count < P) { bool found = false; for (int p = 
        0; p < P; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[p][j] > work[j])
                        break;

                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += alloc[p][k];

                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "\nSystem is NOT in a safe state.";
            return false;
        }
    }

    cout << "\nSystem is in a safe state.\nSafe sequence: ";
    for (int i = 0; i < P; i++)
        cout << "P" << safeSeq[i] << (i == P - 1 ? "" : " -> ");
    cout << endl;
    return true;
}

int main() {
    int P, R;
    cout << "Enter number of processes: "; cin >> P;
    cout << "Enter number of resources: "; cin >> R;

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

    isSafe(P, R, avail, max, alloc);

    return 0;
}
