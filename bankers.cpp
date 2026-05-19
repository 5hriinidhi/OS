#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int n, m;   // n = number of processes
            // m = number of resource types

// Function to check safe state
bool isSafe(vector<vector<int>>& alloc,
            vector<vector<int>>& need,
            vector<int>& avail,
            bool print = true)
{
    vector<int> work = avail;

    vector<bool> finish(n, false);

    vector<int> safeSeq;

    while ((int)safeSeq.size() < n)
    {
        bool found = false;

        for (int i = 0; i < n; i++)
        {
            if (!finish[i])
            {
                bool ok = true;

                for (int j = 0; j < m; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        ok = false;
                        break;
                    }
                }

                // Process can execute
                if (ok)
                {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];

                    finish[i] = true;

                    safeSeq.push_back(i);

                    found = true;
                }
            }
        }

        // No suitable process found
        if (!found)
        {
            if (print)
                cout << "UNSAFE STATE - Deadlock possible!\n";

            return false;
        }
    }

    // Safe state
    if (print)
    {
        cout << "SAFE STATE\n";

        cout << "Safe sequence: ";

        for (int i = 0; i < n; i++)
        {
            cout << "P" << safeSeq[i];

            if (i < n - 1)
                cout << " -> ";
        }

        cout << "\n";
    }

    return true;
}

// Function to request resources
void requestResource(
    int pid,
    vector<int> req,
    vector<vector<int>>& alloc,
    vector<vector<int>>& need,
    vector<int>& avail)
{
    cout << "\nP" << pid << " requests: ";

    for (int x : req)
        cout << x << " ";

    cout << "\n";

    // Check if request exceeds need or availability
    for (int j = 0; j < m; j++)
    {
        if (req[j] > need[pid][j])
        {
            cout << "Error: Exceeds maximum claim!\n";
            return;
        }

        if (req[j] > avail[j])
        {
            cout << "P" << pid << " must WAIT.\n";
            return;
        }
    }

    // Pretend allocation
    for (int j = 0; j < m; j++)
    {
        avail[j]      -= req[j];
        alloc[pid][j] += req[j];
        need[pid][j]  -= req[j];
    }

    // Check safety
    if (isSafe(alloc, need, avail))
    {
        cout << "Request GRANTED to P"
             << pid << "\n";
    }
    else
    {
        cout << "Request DENIED "
             << "(would cause unsafe state)\n";

        // Rollback
        for (int j = 0; j < m; j++)
        {
            avail[j]      += req[j];
            alloc[pid][j] -= req[j];
            need[pid][j]  += req[j];
        }
    }
}

// Main Function
int main()
{
    n = 5;
    m = 3;

    // Allocation Matrix
    vector<vector<int>> alloc =
    {
        {0,1,0},
        {2,0,0},
        {3,0,2},
        {2,1,1},
        {0,0,2}
    };

    // Maximum Matrix
    vector<vector<int>> maxm =
    {
        {7,5,3},
        {3,2,2},
        {9,0,2},
        {2,2,2},
        {4,3,3}
    };

    // Available Resources
    vector<int> avail = {3,3,2};

    // Need Matrix = Max - Allocation
    vector<vector<int>> need(
        n,
        vector<int>(m)
    );

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            need[i][j] =
                maxm[i][j] - alloc[i][j];
        }
    }

    cout << "=== Banker's Algorithm ===\n";

    // Check initial safe state
    isSafe(alloc, need, avail);

    // Example Request:
    // P1 requests {1,0,2}
    requestResource(
        1,
        {1,0,2},
        alloc,
        need,
        avail
    );

    return 0;
}