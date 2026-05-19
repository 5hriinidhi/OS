#include <iostream>
#include <vector>

using namespace std;

void display(const vector<int> &processSize, const vector<int> &allocation) {
    cout << "\nProcess\tSize\tBlock\n";
    for (int i = 0; i < (int)processSize.size(); i++) {
        cout << "P" << i + 1 << "\t" << processSize[i] << "\t";
        if (allocation[i] == -1) {
            cout << "Not Allocated";
        } else {
            cout << allocation[i] + 1;
        }
        cout << endl;
    }
}

void firstFit(vector<int> blocks, const vector<int> &processes) {
    vector<int> allocation(processes.size(), -1);
    for (int i = 0; i < (int)processes.size(); i++) {
        for (int j = 0; j < (int)blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                allocation[i] = j;
                blocks[j] -= processes[i];
                break;
            }
        }
    }
    display(processes, allocation);
}

void bestFit(vector<int> blocks, const vector<int> &processes) {
    vector<int> allocation(processes.size(), -1);
    for (int i = 0; i < (int)processes.size(); i++) {
        int best = -1;
        for (int j = 0; j < (int)blocks.size(); j++) {
            if (blocks[j] >= processes[i] && (best == -1 || blocks[j] < blocks[best])) {
                best = j;
            }
        }
        if (best != -1) {
            allocation[i] = best;
            blocks[best] -= processes[i];
        }
    }
    display(processes, allocation);
}

void worstFit(vector<int> blocks, const vector<int> &processes) {
    vector<int> allocation(processes.size(), -1);
    for (int i = 0; i < (int)processes.size(); i++) {
        int worst = -1;
        for (int j = 0; j < (int)blocks.size(); j++) {
            if (blocks[j] >= processes[i] && (worst == -1 || blocks[j] > blocks[worst])) {
                worst = j;
            }
        }
        if (worst != -1) {
            allocation[i] = worst;
            blocks[worst] -= processes[i];
        }
    }
    display(processes, allocation);
}

void nextFit(vector<int> blocks, const vector<int> &processes) {
    vector<int> allocation(processes.size(), -1);
    int j = 0;

    for (int i = 0; i < (int)processes.size(); i++) {
        int checked = 0;
        while (checked < (int)blocks.size()) {
            if (blocks[j] >= processes[i]) {
                allocation[i] = j;
                blocks[j] -= processes[i];
                break;
            }
            j = (j + 1) % blocks.size();
            checked++;
        }
    }

    display(processes, allocation);
}

int main() {
    int m, n, choice;
    cout << "Enter number of memory blocks: ";
    cin >> m;

    vector<int> blocks(m);
    cout << "Enter block sizes: ";
    for (int i = 0; i < m; i++) {
        cin >> blocks[i];
    }

    cout << "Enter number of processes: ";
    cin >> n;

    vector<int> processes(n);
    cout << "Enter process sizes: ";
    for (int i = 0; i < n; i++) {
        cin >> processes[i];
    }

    cout << "\n1. First Fit\n2. Best Fit\n3. Next Fit\n4. Worst Fit\nChoice: ";
    cin >> choice;

    if (choice == 1) firstFit(blocks, processes);
    else if (choice == 2) bestFit(blocks, processes);
    else if (choice == 3) nextFit(blocks, processes);
    else if (choice == 4) worstFit(blocks, processes);
    else cout << "Invalid choice.\n";

    return 0;
}
