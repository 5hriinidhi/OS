#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

bool contains(const vector<int> &frames, int page) {
    for (int i = 0; i < (int)frames.size(); i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

void printResult(const string &name, int total, int faults) {
    int hits = total - faults;
    cout << "\n" << name << " Page Replacement\n";
    cout << "Page Faults = " << faults << endl;
    cout << "Hits = " << hits << endl;
    cout << fixed << setprecision(2);
    cout << "Fault Ratio = " << (double)faults / total << endl;
    cout << "Hit Ratio = " << (double)hits / total << endl;
}

void fifo(const vector<int> &pages, int frameCount) {
    vector<int> frames;
    queue<int> q;
    int faults = 0;

    for (int i = 0; i < (int)pages.size(); i++) {
        if (!contains(frames, pages[i])) {
            faults++;
            if ((int)frames.size() < frameCount) {
                frames.push_back(pages[i]);
            } else {
                int victim = q.front();
                q.pop();
                for (int j = 0; j < (int)frames.size(); j++) {
                    if (frames[j] == victim) {
                        frames[j] = pages[i];
                        break;
                    }
                }
            }
            q.push(pages[i]);
        }
    }

    printResult("FIFO", pages.size(), faults);
}

void lru(const vector<int> &pages, int frameCount) {
    vector<int> frames;
    int faults = 0;

    for (int i = 0; i < (int)pages.size(); i++) {
        vector<int>::iterator it = find(frames.begin(), frames.end(), pages[i]);
        if (it == frames.end()) {
            faults++;
            if ((int)frames.size() == frameCount) {
                frames.erase(frames.begin());
            }
            frames.push_back(pages[i]);
        } else {
            frames.erase(it);
            frames.push_back(pages[i]);
        }
    }

    printResult("LRU", pages.size(), faults);
}

int predict(const vector<int> &pages, const vector<int> &frames, int index) {
    int farthest = index;
    int result = -1;

    for (int i = 0; i < (int)frames.size(); i++) {
        int j;
        for (j = index; j < (int)pages.size(); j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    result = i;
                }
                break;
            }
        }
        if (j == (int)pages.size()) {
            return i;
        }
    }

    return result == -1 ? 0 : result;
}

void optimal(const vector<int> &pages, int frameCount) {
    vector<int> frames;
    int faults = 0;

    for (int i = 0; i < (int)pages.size(); i++) {
        if (contains(frames, pages[i])) {
            continue;
        }

        faults++;
        if ((int)frames.size() < frameCount) {
            frames.push_back(pages[i]);
        } else {
            int index = predict(pages, frames, i + 1);
            frames[index] = pages[i];
        }
    }

    printResult("Optimal", pages.size(), faults);
}

int main() {
    int n, frames;
    cout << "Enter number of pages: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter reference string: ";
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }

    cout << "Enter number of frames: ";
    cin >> frames;

    fifo(pages, frames);
    lru(pages, frames);
    optimal(pages, frames);

    return 0;
}
