#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

void fcfs(const vector<int> &requests, int head) {
    int totalSeek = 0;
    cout << "\nFCFS Movement: " << head;

    for (int i = 0; i < (int)requests.size(); i++) {
        totalSeek += abs(requests[i] - head);
        head = requests[i];
        cout << " -> " << head;
    }

    cout << "\nTotal Seek Time = " << totalSeek << endl;
}

void sstf(vector<int> requests, int head) {
    vector<bool> visited(requests.size(), false);
    int totalSeek = 0;

    cout << "\nSSTF Movement: " << head;
    for (int i = 0; i < (int)requests.size(); i++) {
        int index = -1;
        int minDistance = 1000000000;

        for (int j = 0; j < (int)requests.size(); j++) {
            int distance = abs(requests[j] - head);
            if (!visited[j] && distance < minDistance) {
                minDistance = distance;
                index = j;
            }
        }

        visited[index] = true;
        totalSeek += minDistance;
        head = requests[index];
        cout << " -> " << head;
    }

    cout << "\nTotal Seek Time = " << totalSeek << endl;
}

void scan(vector<int> requests, int head, int diskSize) {
    int totalSeek = 0;
    requests.push_back(head);
    requests.push_back(diskSize - 1);
    sort(requests.begin(), requests.end());

    int pos = 0;
    for (int i = 0; i < (int)requests.size(); i++) {
        if (requests[i] == head) {
            pos = i;
            break;
        }
    }

    cout << "\nSCAN Movement: ";
    for (int i = pos; i < (int)requests.size(); i++) {
        cout << requests[i];
        if (i != (int)requests.size() - 1) cout << " -> ";
        if (i != (int)requests.size() - 1) {
            totalSeek += abs(requests[i + 1] - requests[i]);
        }
    }

    for (int i = pos - 1; i >= 0; i--) {
        totalSeek += abs(requests[i + 1] - requests[i]);
        cout << " -> " << requests[i];
    }

    cout << "\nTotal Seek Time = " << totalSeek << endl;
}

void cscan(vector<int> requests, int head, int diskSize) {
    int totalSeek = 0;
    requests.push_back(head);
    requests.push_back(0);
    requests.push_back(diskSize - 1);
    sort(requests.begin(), requests.end());

    int pos = 0;
    for (int i = 0; i < (int)requests.size(); i++) {
        if (requests[i] == head) {
            pos = i;
            break;
        }
    }

    cout << "\nC-SCAN Movement: ";
    for (int i = pos; i < (int)requests.size(); i++) {
        cout << requests[i];
        if (i != (int)requests.size() - 1) cout << " -> ";
        if (i != (int)requests.size() - 1) {
            totalSeek += abs(requests[i + 1] - requests[i]);
        }
    }

    totalSeek += diskSize - 1;
    cout << " -> 0";

    for (int i = 1; i < pos; i++) {
        totalSeek += abs(requests[i] - requests[i - 1]);
        cout << " -> " << requests[i];
    }

    cout << "\nTotal Seek Time = " << totalSeek << endl;
}

int main() {
    int n, head, diskSize, choice;
    cout << "Enter number of disk requests: ";
    cin >> n;

    vector<int> requests(n);
    cout << "Enter request queue: ";
    for (int i = 0; i < n; i++) {
        cin >> requests[i];
    }

    cout << "Enter initial head position: ";
    cin >> head;
    cout << "Enter disk size: ";
    cin >> diskSize;

    cout << "\n1. FCFS\n2. SSTF\n3. SCAN\n4. C-SCAN\nChoice: ";
    cin >> choice;

    if (choice == 1) fcfs(requests, head);
    else if (choice == 2) sstf(requests, head);
    else if (choice == 3) scan(requests, head, diskSize);
    else if (choice == 4) cscan(requests, head, diskSize);
    else cout << "Invalid choice.\n";

    return 0;
}
