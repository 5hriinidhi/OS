#include <iostream>
#include <vector>

using namespace std;

void waitSemaphore(int &s) {
    while (s <= 0) {
        return;
    }
    s--;
}

void signalSemaphore(int &s) {
    s++;
}

int main() {
    int n, choice, id;
    cout << "Enter number of philosophers: ";
    cin >> n;

    vector<int> forks(n, 1);
    for (int i = 0; i < n; i++) {
        forks[i] = 1;
    }

    while (true) {
        cout << "\n1. Philosopher wants to eat\n2. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter philosopher number (0 to " << n - 1 << "): ";
            cin >> id;

            if (id < 0 || id >= n) {
                cout << "Invalid philosopher number.\n";
                continue;
            }

            int left = id;
            int right = (id + 1) % n;

            cout << "Philosopher " << id << " is thinking.\n";

            if (id % 2 == 0) {
                waitSemaphore(forks[right]);
                cout << "Philosopher " << id << " picked right fork " << right << endl;
                waitSemaphore(forks[left]);
                cout << "Philosopher " << id << " picked left fork " << left << endl;
            } else {
                waitSemaphore(forks[left]);
                cout << "Philosopher " << id << " picked left fork " << left << endl;
                waitSemaphore(forks[right]);
                cout << "Philosopher " << id << " picked right fork " << right << endl;
            }

            cout << "Philosopher " << id << " is eating.\n";

            signalSemaphore(forks[left]);
            signalSemaphore(forks[right]);
            cout << "Philosopher " << id << " put down both forks.\n";
        } else if (choice == 2) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
