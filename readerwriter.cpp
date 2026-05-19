#include <iostream>
using namespace std;

class Semaphore {
    int val;

public:
    Semaphore(int v) { val = v; }

    bool wait() {
        if (val > 0) {
            val--;
            return true;
        }
        return false;
    }

    void signal() { val++; }
};

class RW {
    Semaphore mutex, wrt;
    int readers, data;

public:
    RW() : mutex(1), wrt(1) {
        readers = 0;
        data = 0;
    }

    void read(int id) {
        mutex.wait();

        readers++;

        if (readers == 1)
            wrt.wait();

        mutex.signal();

        cout << "Reader " << id
             << " reads " << data << endl;

        mutex.wait();

        readers--;

        if (readers == 0)
            wrt.signal();

        mutex.signal();
    }

    void write(int id, int val) {
        wrt.wait();

        data = val;

        cout << "Writer " << id
             << " writes " << data << endl;

        wrt.signal();
    }
};

int main() {
    RW r;

    r.read(1);
    r.read(2);

    r.write(1, 10);

    r.read(3);

    return 0;
}