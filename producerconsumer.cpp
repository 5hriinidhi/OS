#include <iostream>
#include <queue>

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
    int bufferSize, choice, item = 1;
    cout << "Enter buffer size: ";
    cin >> bufferSize;

    queue<int> buffer;
    int mutexLock = 1;
    int emptySlots = bufferSize;
    int fullSlots = 0;

    while (true) {
        cout << "\n1. Produce\n2. Consume\n3. Display Buffer\n4. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            if ((int)buffer.size() == bufferSize) {
                cout << "Buffer is full. Producer must wait.\n";
                continue;
            }

            waitSemaphore(emptySlots);
            waitSemaphore(mutexLock);
            buffer.push(item);
            cout << "Producer produced item " << item << endl;
            item++;
            signalSemaphore(mutexLock);
            signalSemaphore(fullSlots);
        } else if (choice == 2) {
            if (buffer.empty()) {
                cout << "Buffer is empty. Consumer must wait.\n";
                continue;
            }

            waitSemaphore(fullSlots);
            waitSemaphore(mutexLock);
            cout << "Consumer consumed item " << buffer.front() << endl;
            buffer.pop();
            signalSemaphore(mutexLock);
            signalSemaphore(emptySlots);
        } else if (choice == 3) {
            queue<int> temp = buffer;
            cout << "Buffer: ";
            if (temp.empty()) {
                cout << "empty";
            }
            while (!temp.empty()) {
                cout << temp.front() << " ";
                temp.pop();
            }
            cout << endl;
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
