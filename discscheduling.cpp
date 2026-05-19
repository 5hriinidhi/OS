#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

// ─────────────────────────────────────────────────────────
// FCFS Disk Scheduling
// ─────────────────────────────────────────────────────────
void fcfs(vector<int> req, int head)
{
    cout << "\n=== FCFS ===\n";

    int total = 0;

    cout << "Sequence: " << head;

    for (int r : req)
    {
        total += abs(r - head);

        head = r;

        cout << " -> " << r;
    }

    cout << "\nTotal Head Movement: "
         << total
         << "\n";
}

// ─────────────────────────────────────────────────────────
// SSTF Disk Scheduling
// ─────────────────────────────────────────────────────────
void sstf(vector<int> req, int head)
{
    cout << "\n=== SSTF ===\n";

    int total = 0;

    int n = req.size();

    vector<bool> visited(n, false);

    cout << "Sequence: " << head;

    for (int i = 0; i < n; i++)
    {
        int minD = INT_MAX;

        int idx = -1;

        for (int j = 0; j < n; j++)
        {
            if (!visited[j] &&
                abs(req[j] - head) < minD)
            {
                minD = abs(req[j] - head);

                idx = j;
            }
        }

        visited[idx] = true;

        total += minD;

        head = req[idx];

        cout << " -> " << req[idx];
    }

    cout << "\nTotal Head Movement: "
         << total
         << "\n";
}

// ─────────────────────────────────────────────────────────
// SCAN (Elevator Algorithm)
// ─────────────────────────────────────────────────────────
void scan(
    vector<int> req,
    int head,
    int diskSize)
{
    cout << "\n=== SCAN (Elevator) ===\n";

    vector<int> left;
    vector<int> right;

    // Divide requests
    for (int r : req)
    {
        if (r < head)
            left.push_back(r);
        else
            right.push_back(r);
    }

    // Sort requests
    sort(left.rbegin(), left.rend());

    sort(right.begin(), right.end());

    int total = 0;

    cout << "Sequence: " << head;

    // Move right first
    for (int r : right)
    {
        total += abs(r - head);

        head = r;

        cout << " -> " << r;
    }

    // Move left
    for (int r : left)
    {
        total += abs(r - head);

        head = r;

        cout << " -> " << r;
    }

    cout << "\nTotal Head Movement: "
         << total
         << "\n";
}

// ─────────────────────────────────────────────────────────
// C-SCAN Disk Scheduling
// ─────────────────────────────────────────────────────────
void cscan(
    vector<int> req,
    int head,
    int diskSize)
{
    cout << "\n=== C-SCAN ===\n";

    vector<int> left;
    vector<int> right;

    // Divide requests
    for (int r : req)
    {
        if (r < head)
            left.push_back(r);
        else
            right.push_back(r);
    }

    // Sort requests
    sort(left.begin(), left.end());

    sort(right.begin(), right.end());

    int total = 0;

    cout << "Sequence: " << head;

    // Move right first
    for (int r : right)
    {
        total += abs(r - head);

        head = r;

        cout << " -> " << r;
    }

    // Jump back to start
    total += head;

    cout << " -> 0";

    head = 0;

    // Service left-side requests
    for (int r : left)
    {
        total += abs(r - head);

        head = r;

        cout << " -> " << r;
    }

    cout << "\nTotal Head Movement: "
         << total
         << "\n";
}

// ─────────────────────────────────────────────────────────
// Main Function
// ─────────────────────────────────────────────────────────
int main()
{
    vector<int> requests =
    {
        176,
        79,
        34,
        60,
        92,
        11,
        41,
        114
    };

    int head = 50;

    int diskSize = 200;

    cout << "Requests: ";

    for (int r : requests)
        cout << r << " ";

    cout << "\nInitial Head: "
         << head
         << "\n";

    fcfs(requests, head);

    sstf(requests, head);

    scan(requests, head, diskSize);

    cscan(requests, head, diskSize);

    return 0;
}
