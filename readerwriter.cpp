#include <iostream>

using namespace std;

int mutexLock = 1;
int wrt = 1;
int readCount = 0;
int sharedData = 0;

void waitSemaphore(int &s) {
    while (s <= 0) {
        return;
    }
    s--;
}

void signalSemaphore(int &s) {
    s++;
}

void startReading(int id) {
    waitSemaphore(mutexLock);
    readCount++;
    if (readCount == 1) {
        waitSemaphore(wrt);
    }
    signalSemaphore(mutexLock);

    cout << "Reader " << id << " is reading data = " << sharedData << endl;
}

void stopReading(int id) {
    waitSemaphore(mutexLock);
    readCount--;
    cout << "Reader " << id << " finished reading.\n";
    if (readCount == 0) {
        signalSemaphore(wrt);
    }
    signalSemaphore(mutexLock);
}

void startWriting(int id) {
    if (wrt <= 0) {
        cout << "Writer " << id << " is waiting because resource is busy.\n";
        return;
    }

    waitSemaphore(wrt);
    sharedData += 10;
    cout << "Writer " << id << " wrote data. New value = " << sharedData << endl;
}

void stopWriting(int id) {
    signalSemaphore(wrt);
    cout << "Writer " << id << " finished writing.\n";
}

int main() {
    int choice, id;

    while (true) {
        cout << "\nReader Writer Problem\n";
        cout << "1. Start Reading\n2. Stop Reading\n3. Start Writing\n4. Stop Writing\n5. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter reader id: ";
            cin >> id;
            startReading(id);
        } else if (choice == 2) {
            cout << "Enter reader id: ";
            cin >> id;
            stopReading(id);
        } else if (choice == 3) {
            cout << "Enter writer id: ";
            cin >> id;
            startWriting(id);
        } else if (choice == 4) {
            cout << "Enter writer id: ";
            cin >> id;
            stopWriting(id);
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
