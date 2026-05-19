#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Process {
    int id;
    int at;
    int bt;
    int priority;
    int remaining;
    int ct;
    int tat;
    int wt;
    bool done;
    bool inQueue;
};

void printTable(vector<Process> p) {
    double totalWt = 0;
    double totalTat = 0;

    cout << "\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n";
    for (int i = 0; i < (int)p.size(); i++) {
        totalWt += p[i].wt;
        totalTat += p[i].tat;
        cout << "P" << p[i].id << "\t" << p[i].at << "\t" << p[i].bt << "\t"
             << p[i].priority << "\t" << p[i].ct << "\t" << p[i].tat << "\t"
             << p[i].wt << endl;
    }

    cout << fixed << setprecision(2);
    cout << "Average Waiting Time = " << totalWt / p.size() << endl;
    cout << "Average Turnaround Time = " << totalTat / p.size() << endl;
}

vector<Process> inputProcesses(bool needPriority) {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "\nProcess P" << p[i].id << endl;
        cout << "Arrival Time: ";
        cin >> p[i].at;
        cout << "Burst Time: ";
        cin >> p[i].bt;
        if (needPriority) {
            cout << "Priority (smaller number = higher priority): ";
            cin >> p[i].priority;
        } else {
            p[i].priority = 0;
        }
        p[i].remaining = p[i].bt;
        p[i].ct = p[i].tat = p[i].wt = 0;
        p[i].done = false;
        p[i].inQueue = false;
    }
    return p;
}

void fcfs() {
    vector<Process> p = inputProcesses(false);
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.at < b.at;
    });

    int time = 0;
    cout << "\nGantt Chart: ";
    for (int i = 0; i < (int)p.size(); i++) {
        if (time < p[i].at) {
            time = p[i].at;
        }
        cout << "| P" << p[i].id << " ";
        time += p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
    cout << "|\n";

    printTable(p);
}

void sjf() {
    vector<Process> p = inputProcesses(false);
    int time = 0;
    int completed = 0;

    cout << "\nGantt Chart: ";
    while (completed < (int)p.size()) {
        int idx = -1;
        int bestBt = 1000000000;

        for (int i = 0; i < (int)p.size(); i++) {
            if (!p[i].done && p[i].at <= time && p[i].bt < bestBt) {
                bestBt = p[i].bt;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        cout << "| P" << p[idx].id << " ";
        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = true;
        completed++;
    }
    cout << "|\n";

    printTable(p);
}

void roundRobin() {
    vector<Process> p = inputProcesses(false);
    int quantum;
    cout << "Enter time quantum: ";
    cin >> quantum;

    queue<int> q;
    int time = 0;
    int completed = 0;

    cout << "\nGantt Chart: ";
    while (completed < (int)p.size()) {
        for (int i = 0; i < (int)p.size(); i++) {
            if (!p[i].inQueue && p[i].at <= time && p[i].remaining > 0) {
                q.push(i);
                p[i].inQueue = true;
            }
        }

        if (q.empty()) {
            time++;
            continue;
        }

        int idx = q.front();
        q.pop();

        int runTime = min(quantum, p[idx].remaining);
        cout << "| P" << p[idx].id << " ";
        p[idx].remaining -= runTime;
        time += runTime;

        for (int i = 0; i < (int)p.size(); i++) {
            if (!p[i].inQueue && p[i].at <= time && p[i].remaining > 0) {
                q.push(i);
                p[i].inQueue = true;
            }
        }

        if (p[idx].remaining > 0) {
            q.push(idx);
        } else {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    cout << "|\n";

    printTable(p);
}

void priorityScheduling() {
    vector<Process> p = inputProcesses(true);
    int time = 0;
    int completed = 0;

    cout << "\nGantt Chart: ";
    while (completed < (int)p.size()) {
        int idx = -1;
        int bestPriority = 1000000000;

        for (int i = 0; i < (int)p.size(); i++) {
            if (!p[i].done && p[i].at <= time && p[i].priority < bestPriority) {
                bestPriority = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        cout << "| P" << p[idx].id << " ";
        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = true;
        completed++;
    }
    cout << "|\n";

    printTable(p);
}

int main() {
    int choice;
    while (true) {
        cout << "\nCPU Scheduling Algorithms\n";
        cout << "1. FCFS\n2. SJF\n3. Round Robin\n4. Priority\n5. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) fcfs();
        else if (choice == 2) sjf();
        else if (choice == 3) roundRobin();
        else if (choice == 4) priorityScheduling();
        else if (choice == 5) break;
        else cout << "Invalid choice.\n";
    }

    return 0;
}
