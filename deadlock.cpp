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
    vector<vector<int> > request(n, vector<int>(m));
    vector<int> avail(m);

    cout << "\nEnter Allocation Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> alloc[i][j];
        }
    }

    cout << "\nEnter Request Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> request[i][j];
        }
    }

    cout << "\nEnter Available Resources:\n";
    for (int i = 0; i < m; i++) {
        cin >> avail[i];
    }

    vector<int> work = avail;
    vector<bool> finish(n, false);

    for (int count = 0; count < n; count++) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < m; j++) {
                        work[j] += alloc[i][j];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            break;
        }
    }

    vector<int> deadlocked;
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            deadlocked.push_back(i);
        }
    }

    if (deadlocked.empty()) {
        cout << "\nNo deadlock detected.\n";
    } else {
        cout << "\nDeadlock detected.\nDeadlocked processes: ";
        for (int i = 0; i < (int)deadlocked.size(); i++) {
            cout << "P" << deadlocked[i];
            if (i != (int)deadlocked.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    return 0;
}
