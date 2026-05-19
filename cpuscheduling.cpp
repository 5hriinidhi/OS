#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

// Structure for Process
struct Process
{
    int pid;
    int at;        // Arrival Time
    int bt;        // Burst Time
    int priority;

    int ct  = 0;   // Completion Time
    int tat = 0;   // Turnaround Time
    int wt  = 0;   // Waiting Time
};

// Function to print table
void printTable(vector<Process>& p, bool showPriority = false)
{
    float sumTAT = 0;
    float sumWT  = 0;

    if (showPriority)
        cout << "PID  AT  BT  PR  CT  TAT  WT\n";
    else
        cout << "PID  AT  BT  CT  TAT  WT\n";

    for (auto& x : p)
    {
        sumTAT += x.tat;
        sumWT  += x.wt;

        if (showPriority)
        {
            cout << x.pid << "    "
                 << x.at << "   "
                 << x.bt << "   "
                 << x.priority << "   "
                 << x.ct << "   "
                 << x.tat << "    "
                 << x.wt << "\n";
        }
        else
        {
            cout << x.pid << "    "
                 << x.at << "   "
                 << x.bt << "   "
                 << x.ct << "   "
                 << x.tat << "    "
                 << x.wt << "\n";
        }
    }

    cout << "Avg TAT = " << sumTAT / p.size()
         << "   Avg WT = " << sumWT / p.size()
         << "\n";
}

// ─────────────────────────────────────────────────────────
// FCFS Scheduling
// ─────────────────────────────────────────────────────────
void fcfs(vector<Process> p)
{
    cout << "\n=== FCFS ===\n";

    sort(p.begin(), p.end(),
         [](Process a, Process b)
         {
             return a.at < b.at;
         });

    int t = 0;

    for (auto& x : p)
    {
        if (t < x.at)
            t = x.at;

        x.wt  = t - x.at;
        x.ct  = t + x.bt;
        x.tat = x.ct - x.at;

        t = x.ct;
    }

    printTable(p);
}

// ─────────────────────────────────────────────────────────
// SJF (Non-Preemptive)
// ─────────────────────────────────────────────────────────
void sjf(vector<Process> p)
{
    cout << "\n=== SJF (Non-Preemptive) ===\n";

    int n = p.size();
    int t = 0;
    int done = 0;

    vector<bool> visited(n, false);
    vector<Process> result;

    while (done < n)
    {
        int idx = -1;
        int minBT = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (!visited[i] &&
                p[i].at <= t &&
                p[i].bt < minBT)
            {
                minBT = p[i].bt;
                idx = i;
            }
        }

        if (idx == -1)
        {
            t++;
            continue;
        }

        p[idx].wt  = t - p[idx].at;
        p[idx].ct  = t + p[idx].bt;
        p[idx].tat = p[idx].ct - p[idx].at;

        t = p[idx].ct;

        visited[idx] = true;
        done++;

        result.push_back(p[idx]);
    }

    printTable(result);
}

// ─────────────────────────────────────────────────────────
// Round Robin Scheduling
// ─────────────────────────────────────────────────────────
void roundRobin(vector<Process> p, int q)
{
    cout << "\n=== Round Robin (Quantum = "
         << q << ") ===\n";

    int n = p.size();
    int t = 0;
    int done = 0;

    vector<int> rem(n);

    for (int i = 0; i < n; i++)
        rem[i] = p[i].bt;

    queue<int> rq;
    vector<bool> inQ(n, false);

    // Add processes with arrival time 0
    for (int i = 0; i < n; i++)
    {
        if (p[i].at == 0)
        {
            rq.push(i);
            inQ[i] = true;
        }
    }

    while (done < n)
    {
        if (rq.empty())
        {
            t++;

            for (int i = 0; i < n; i++)
            {
                if (!inQ[i] && p[i].at <= t)
                {
                    rq.push(i);
                    inQ[i] = true;
                }
            }

            continue;
        }

        int idx = rq.front();
        rq.pop();

        int exec = min(rem[idx], q);

        t += exec;
        rem[idx] -= exec;

        // Add newly arrived processes
        for (int i = 0; i < n; i++)
        {
            if (!inQ[i] && p[i].at <= t)
            {
                rq.push(i);
                inQ[i] = true;
            }
        }

        // Process completed
        if (rem[idx] == 0)
        {
            p[idx].ct  = t;
            p[idx].tat = t - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;

            done++;
        }
        else
        {
            rq.push(idx);
        }
    }

    printTable(p);
}

// ─────────────────────────────────────────────────────────
// Priority Scheduling (Non-Preemptive)
// Lower number = Higher priority
// ─────────────────────────────────────────────────────────
void prioritySched(vector<Process> p)
{
    cout << "\n=== Priority (Non-Preemptive) ===\n";

    int n = p.size();
    int t = 0;
    int done = 0;

    vector<bool> visited(n, false);
    vector<Process> result;

    while (done < n)
    {
        int idx = -1;
        int minP = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (!visited[i] &&
                p[i].at <= t &&
                p[i].priority < minP)
            {
                minP = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1)
        {
            t++;
            continue;
        }

        p[idx].wt  = t - p[idx].at;
        p[idx].ct  = t + p[idx].bt;
        p[idx].tat = p[idx].ct - p[idx].at;

        t = p[idx].ct;

        visited[idx] = true;
        done++;

        result.push_back(p[idx]);
    }

    printTable(result, true);
}

// ─────────────────────────────────────────────────────────
// Main Function
// ─────────────────────────────────────────────────────────
int main()
{
    // pid, arrival time, burst time, priority
    vector<Process> procs =
    {
        {1, 0, 5, 2},
        {2, 1, 3, 1},
        {3, 2, 8, 3},
        {4, 3, 2, 4}
    };

    fcfs(procs);

    sjf(procs);

    roundRobin(procs, 2);

    prioritySched(procs);

    return 0;
}