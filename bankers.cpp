#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n, m;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resource types: ";
    cin >> m;

    vector<vector<int> > alloc(n, vector<int>(m));
    vector<vector<int> > maxNeed(n, vector<int>(m));
    vector<vector<int> > need(n, vector<int>(m));
    vector<int> avail(m);

    cout << "\nEnter Allocation Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> alloc[i][j];
        }
    }

    cout << "\nEnter Max Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> maxNeed[i][j];
            need[i][j] = maxNeed[i][j] - alloc[i][j];
        }
    }

    cout << "\nEnter Available Resources:\n";
    for (int i = 0; i < m; i++) {
        cin >> avail[i];
    }

    vector<int> work = avail;
    vector<bool> finish(n, false);
    vector<int> safeSequence;

    for (int count = 0; count < n; count++) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < m; j++) {
                        work[j] += alloc[i][j];
                    }
                    finish[i] = true;
                    safeSequence.push_back(i);
                    found = true;
                }
            }
        }

        if (!found) {
            break;
        }
    }

    if ((int)safeSequence.size() == n) {
        cout << "\nSystem is in a safe state.\nSafe sequence: ";
        for (int i = 0; i < n; i++) {
            cout << "P" << safeSequence[i];
            if (i != n - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "\nSystem is not in a safe state.\n";
    }

    return 0;
}
