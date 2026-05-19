#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <climits>

using namespace std;

// ─────────────────────────────────────────────────────────
// FIFO Page Replacement
// ─────────────────────────────────────────────────────────
int fifo(vector<int>& pages, int frames)
{
    list<int> mem;

    int faults = 0;

    cout << "\n=== FIFO ===\n";

    for (int page : pages)
    {
        bool hit =
            (find(mem.begin(), mem.end(), page)
             != mem.end());

        if (!hit)
        {
            faults++;

            // Remove oldest page
            if ((int)mem.size() == frames)
                mem.pop_front();

            mem.push_back(page);

            cout << "Page "
                 << page
                 << " -> FAULT  | ";
        }
        else
        {
            cout << "Page "
                 << page
                 << " -> HIT    | ";
        }

        cout << "Frames: ";

        for (int p : mem)
            cout << p << " ";

        cout << "\n";
    }

    return faults;
}

// ─────────────────────────────────────────────────────────
// LRU Page Replacement
// ─────────────────────────────────────────────────────────
int lru(vector<int>& pages, int frames)
{
    list<int> mem;

    // Back = most recently used

    int faults = 0;

    cout << "\n=== LRU ===\n";

    for (int page : pages)
    {
        auto it =
            find(mem.begin(), mem.end(), page);

        // Page Fault
        if (it == mem.end())
        {
            faults++;

            if ((int)mem.size() == frames)
                mem.pop_front();

            cout << "Page "
                 << page
                 << " -> FAULT  | ";
        }
        else
        {
            // Remove old position
            mem.erase(it);

            cout << "Page "
                 << page
                 << " -> HIT    | ";
        }

        // Add as most recently used
        mem.push_back(page);

        cout << "Frames: ";

        for (int p : mem)
            cout << p << " ";

        cout << "\n";
    }

    return faults;
}

// ─────────────────────────────────────────────────────────
// Optimal Page Replacement
// ─────────────────────────────────────────────────────────
int optimal(vector<int>& pages, int frames)
{
    vector<int> mem;

    int faults = 0;

    int n = pages.size();

    cout << "\n=== Optimal ===\n";

    for (int i = 0; i < n; i++)
    {
        bool hit =
            (find(mem.begin(), mem.end(), pages[i])
             != mem.end());

        if (!hit)
        {
            faults++;

            // Empty frame available
            if ((int)mem.size() < frames)
            {
                mem.push_back(pages[i]);
            }
            else
            {
                // Find page used farthest in future
                int replIdx = 0;
                int farthest = -1;

                for (int j = 0; j < (int)mem.size(); j++)
                {
                    int nxt = -1;

                    for (int k = i + 1; k < n; k++)
                    {
                        if (pages[k] == mem[j])
                        {
                            nxt = k;
                            break;
                        }
                    }

                    // Page never used again
                    if (nxt == -1)
                    {
                        replIdx = j;
                        break;
                    }

                    // Farthest future use
                    if (nxt > farthest)
                    {
                        farthest = nxt;
                        replIdx = j;
                    }
                }

                mem[replIdx] = pages[i];
            }

            cout << "Page "
                 << pages[i]
                 << " -> FAULT  | ";
        }
        else
        {
            cout << "Page "
                 << pages[i]
                 << " -> HIT    | ";
        }

        cout << "Frames: ";

        for (int p : mem)
            cout << p << " ";

        cout << "\n";
    }

    return faults;
}

// ─────────────────────────────────────────────────────────
// Main Function
// ─────────────────────────────────────────────────────────
int main()
{
    vector<int> pages =
    {
        7,0,1,2,0,3,0,4,2,3,0,3,2
    };

    int frames = 3;

    cout << "Reference String: ";

    for (int p : pages)
        cout << p << " ";

    cout << "\nFrames: "
         << frames
         << "\n";

    int f1 = fifo(pages, frames);

    int f2 = lru(pages, frames);

    int f3 = optimal(pages, frames);

    cout << "\n=== Summary ===\n";

    cout << "FIFO    Page Faults: "
         << f1 << "\n";

    cout << "LRU     Page Faults: "
         << f2 << "\n";

    cout << "Optimal Page Faults: "
         << f3 << "\n";

    return 0;
}