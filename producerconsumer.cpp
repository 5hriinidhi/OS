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

class ProducerConsumer {
    Semaphore mutex, empty, full;

    int buffer[5];
    int in, out;

public:
    ProducerConsumer() : mutex(1), empty(5), full(0) {
        in = 0;
        out = 0;
    }

    void produce(int item) {

        if (empty.wait()) {

            mutex.wait();

            buffer[in] = item;

            cout << "Produced: " << item << endl;

            in = (in + 1) % 5;

            mutex.signal();

            full.signal();
        }
        else {
            cout << "Buffer Full!" << endl;
        }
    }

    void consume() {

        if (full.wait()) {

            mutex.wait();

            int item = buffer[out];

            cout << "Consumed: " << item << endl;

            out = (out + 1) % 5;

            mutex.signal();

            empty.signal();
        }
        else {
            cout << "Buffer Empty!" << endl;
        }
    }
};

int main() {

    ProducerConsumer p;

    p.produce(10);
    p.produce(20);
    p.produce(30);

    p.consume();
    p.consume();

    p.produce(40);

    p.consume();
    p.consume();

    return 0;
}