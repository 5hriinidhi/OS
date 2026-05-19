#include <iostream>
using namespace std;

class Semaphore {
    int val;

public:
    Semaphore(int v) {
        val = v;
    }

    bool wait() {
        if (val > 0) {
            val--;
            return true;
        }
        return false;
    }

    void signal() {
        val++;
    }
};

class DiningPhilosopher {

    Semaphore fork[5];

public:

    DiningPhilosopher()
        : fork{Semaphore(1), Semaphore(1),
               Semaphore(1), Semaphore(1),
               Semaphore(1)} {}

    void eat(int i) {

        int left = i;
        int right = (i + 1) % 5;

        if (fork[left].wait()) {

            cout << "Philosopher "
                 << i + 1
                 << " picked left fork\n";

            if (fork[right].wait()) {

                cout << "Philosopher "
                     << i + 1
                     << " picked right fork\n";

                cout << "Philosopher "
                     << i + 1
                     << " is Eating\n";

                fork[right].signal();

                fork[left].signal();

                cout << "Philosopher "
                     << i + 1
                     << " released forks\n";
            }
            else {

                fork[left].signal();

                cout << "Right fork not available\n";
            }
        }
        else {
            cout << "Left fork not available\n";
        }
    }
};

int main() {

    DiningPhilosopher d;

    d.eat(0);
    d.eat(1);
    d.eat(2);
    d.eat(3);
    d.eat(4);

    return 0;
}