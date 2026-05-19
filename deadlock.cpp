#include <iostream>
#include <vector>

using namespace std;

int n, m;   // n = number of processes
            // m = number of resource types

// Deadlock Detection Function
bool detectDeadlock(
    vector<vector<int>>& alloc,
    vector<vector<int>>& request,
    vector<int>& avail)
{
    vector<int> work = avail;

    vector<bool> finish(n, false);

    // Mark processes with zero allocation as finished
    for (int i = 0; i < n; i++)
    {
        bool hasAlloc = false;

        for (int j = 0; j < m; j++)
        {
            if (alloc[i][j] > 0)
            {
                hasAlloc = true;
                break;
            }
        }

        if (!hasAlloc)
            finish[i] = true;
    }

    bool found = true;

    while (found)
    {
        found = false;

        for (int i = 0; i < n; i++)
        {
            if (!finish[i])
            {
                bool canFinish = true;

                // Check if request <= available
                for (int j = 0; j < m; j++)
                {
                    if (request[i][j] > work[j])
                    {
                        canFinish = false;
                        break;
                    }
                }

                // Process can finish
                if (canFinish)
                {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];

                    finish[i] = true;

                    found = true;

                    cout << "P"
                         << i
                         << " can complete\n";
                }
            }
        }
    }

    cout << "\n--- Deadlock Detection Result ---\n";

    bool deadlock = false;

    for (int i = 0; i < n; i++)
    {
        if (!finish[i])
        {
            cout << "P"
                 << i
                 << " is DEADLOCKED\n";

            deadlock = true;
        }
    }

    if (!deadlock)
    {
        cout << "No deadlock detected. "
             << "All processes can complete.\n";
    }

    return deadlock;
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
        {3,0,3},
        {2,1,0},
        {0,0,2}
    };

    // Request Matrix
    vector<vector<int>> request =
    {
        {0,0,0},
        {2,0,2},
        {0,0,0},
        {1,0,0},
        {0,0,2}
    };

    // Available Resources
    vector<int> avail = {0,0,0};

    cout << "=== Deadlock Detection Algorithm ===\n";

    detectDeadlock(
        alloc,
        request,
        avail
    );

    return 0;
}