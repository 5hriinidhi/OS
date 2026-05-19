#include <cmath>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int totalMemory;
map<int, vector<pair<int, int> > > freeList;

int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

void showFreeList() {
    cout << "\nFree List:\n";
    for (map<int, vector<pair<int, int> > >::iterator it = freeList.begin(); it != freeList.end(); ++it) {
        if (!it->second.empty()) {
            cout << "Block size " << it->first << ": ";
            for (int i = 0; i < (int)it->second.size(); i++) {
                cout << "[" << it->second[i].first << "-" << it->second[i].second << "] ";
            }
            cout << endl;
        }
    }
}

void allocateMemory(int request) {
    int needed = nextPowerOfTwo(request);
    int current = needed;

    while ((freeList.find(current) == freeList.end() || freeList[current].empty()) && current <= totalMemory) {
        current *= 2;
    }

    if (current > totalMemory) {
        cout << "Allocation failed for " << request << " bytes.\n";
        return;
    }

    pair<int, int> block = freeList[current].back();
    freeList[current].pop_back();

    while (current > needed) {
        current /= 2;
        pair<int, int> rightBlock(block.first + current, block.second);
        block.second = block.first + current - 1;
        freeList[current].push_back(rightBlock);
    }

    cout << "Allocated " << request << " bytes as block size " << needed
         << " at [" << block.first << "-" << block.second << "]\n";
}

int main() {
    int choice, request;
    cout << "Enter total memory size: ";
    cin >> totalMemory;

    totalMemory = nextPowerOfTwo(totalMemory);
    freeList[totalMemory].push_back(make_pair(0, totalMemory - 1));

    while (true) {
        cout << "\n1. Allocate\n2. Display Free List\n3. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter memory request: ";
            cin >> request;
            allocateMemory(request);
        } else if (choice == 2) {
            showFreeList();
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
