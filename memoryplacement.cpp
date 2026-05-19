#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// ─────────────────────────────────────────────────────────
// First Fit
// ─────────────────────────────────────────────────────────
void firstFit(
    vector<int> blocks,
    vector<int> procs)
{
    cout << "\n=== First Fit ===\n";

    for (int i = 0; i < (int)procs.size(); i++)
    {
        bool done = false;

        for (int j = 0; j < (int)blocks.size(); j++)
        {
            if (blocks[j] >= procs[i])
            {
                cout << "Process "
                     << i + 1
                     << " (" << procs[i] << "KB)"
                     << " -> Block "
                     << j + 1
                     << " (" << blocks[j] << "KB)\n";

                blocks[j] -= procs[i];

                done = true;
                break;
            }
        }

        if (!done)
        {
            cout << "Process "
                 << i + 1
                 << " (" << procs[i] << "KB)"
                 << " -> NOT ALLOCATED\n";
        }
    }
}

// ─────────────────────────────────────────────────────────
// Best Fit
// ─────────────────────────────────────────────────────────
void bestFit(
    vector<int> blocks,
    vector<int> procs)
{
    cout << "\n=== Best Fit ===\n";

    for (int i = 0; i < (int)procs.size(); i++)
    {
        int best = -1;
        int bestSz = INT_MAX;

        for (int j = 0; j < (int)blocks.size(); j++)
        {
            if (blocks[j] >= procs[i] &&
                blocks[j] < bestSz)
            {
                bestSz = blocks[j];
                best = j;
            }
        }

        if (best != -1)
        {
            cout << "Process "
                 << i + 1
                 << " (" << procs[i] << "KB)"
                 << " -> Block "
                 << best + 1
                 << " (" << blocks[best] << "KB)\n";

            blocks[best] -= procs[i];
        }
        else
        {
            cout << "Process "
                 << i + 1
                 << " (" << procs[i] << "KB)"
                 << " -> NOT ALLOCATED\n";
        }
    }
}

// ─────────────────────────────────────────────────────────
// Worst Fit
// ─────────────────────────────────────────────────────────
void worstFit(
    vector<int> blocks,
    vector<int> procs)
{
    cout << "\n=== Worst Fit ===\n";

    for (int i = 0; i < (int)procs.size(); i++)
    {
        int worst = -1;
        int worstSz = -1;

        for (int j = 0; j < (int)blocks.size(); j++)
        {
            if (blocks[j] >= procs[i] &&
                blocks[j] > worstSz)
            {
                worstSz = blocks[j];
                worst = j;
            }
        }

        if (worst != -1)
        {
            cout << "Process "
                 << i + 1
                 << " (" << procs[i] << "KB)"
                 << " -> Block "
                 << worst + 1
                 << " (" << blocks[worst] << "KB)\n";

            blocks[worst] -= procs[i];
        }
        else
        {
            cout << "Process "
                 << i + 1
                 << " (" << procs[i] << "KB)"
                 << " -> NOT ALLOCATED\n";
        }
    }
}

// ─────────────────────────────────────────────────────────
// Next Fit
// ─────────────────────────────────────────────────────────
void nextFit(
    vector<int> blocks,
    vector<int> procs)
{
    cout << "\n=== Next Fit ===\n";

    int last = 0;
    int sz = blocks.size();

    for (int i = 0; i < (int)procs.size(); i++)
    {
        bool done = false;

        int start = last;

        do
        {
            if (blocks[last] >= procs[i])
            {
                cout << "Process "
                     << i + 1
                     << " (" << procs[i] << "KB)"
                     << " -> Block "
                     << last + 1
                     << " (" << blocks[last] << "KB)\n";

                blocks[last] -= procs[i];

                done = true;
                break;
            }

            last = (last + 1) % sz;

        } while (last != start);

        if (!done)
        {
            cout << "Process "
                 << i + 1
                 << " (" << procs[i] << "KB)"
                 << " -> NOT ALLOCATED\n";
        }
    }
}

// ─────────────────────────────────────────────────────────
// Main Function
// ─────────────────────────────────────────────────────────
int main()
{
    vector<int> blocks =
    {
        100,
        500,
        200,
        300,
        600
    };

    vector<int> procs =
    {
        212,
        417,
        112,
        426
    };

    cout << "Memory Blocks (KB): ";

    for (int b : blocks)
        cout << b << " ";

    cout << "\n";

    cout << "Process Sizes (KB): ";

    for (int p : procs)
        cout << p << " ";

    cout << "\n";

    firstFit(blocks, procs);

    bestFit(blocks, procs);

    worstFit(blocks, procs);

    nextFit(blocks, procs);

    return 0;
}