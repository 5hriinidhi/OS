#include <iostream>
#include <map>

using namespace std;

// Total memory size
int totalMemory;

// Map:
// block size -> number of free blocks
map<int, int> freeBlocks;

// Function to find next power of 2
int nextPow2(int n)
{
    int p = 1;

    while (p < n)
        p *= 2;

    return p;
}

// Function to display free blocks
void printStatus()
{
    cout << "Free Blocks: ";

    for (auto& kv : freeBlocks)
    {
        if (kv.second > 0)
        {
            cout << kv.first
                 << "KB x"
                 << kv.second
                 << "  ";
        }
    }

    cout << "\n";
}

// Allocation Function
int allocate(int size)
{
    int need = nextPow2(size);

    cout << "\nAllocate "
         << size
         << "KB -> needs "
         << need
         << "KB block\n";

    // Find smallest block >= needed size
    for (auto it = freeBlocks.lower_bound(need);
         it != freeBlocks.end();
         ++it)
    {
        if (it->second > 0)
        {
            int blk = it->first;

            freeBlocks[blk]--;

            // Split blocks until desired size reached
            while (blk > need)
            {
                blk /= 2;

                freeBlocks[blk]++;

                cout << "  Split -> 2 x "
                     << blk
                     << "KB blocks\n";
            }

            cout << "  Allocated "
                 << need
                 << "KB\n";

            printStatus();

            return need;
        }
    }

    cout << "  Allocation FAILED\n";

    return -1;
}

// Release Function
void release(int blockSize)
{
    cout << "\nRelease "
         << blockSize
         << "KB block\n";

    freeBlocks[blockSize]++;

    // Merge buddy blocks if possible
    while (blockSize < totalMemory)
    {
        if (freeBlocks[blockSize] >= 2)
        {
            freeBlocks[blockSize] -= 2;

            blockSize *= 2;

            freeBlocks[blockSize]++;

            cout << "  Merged into "
                 << blockSize
                 << "KB block\n";
        }
        else
        {
            break;
        }
    }

    printStatus();
}

// Main Function
int main()
{
    totalMemory = 1024;

    // Initially whole memory is free
    freeBlocks[1024] = 1;

    cout << "=== Buddy System "
         << "(Total: "
         << totalMemory
         << "KB) ===\n";

    printStatus();

    // Allocation Examples
    int b1 = allocate(100);   // gets 128KB

    int b2 = allocate(240);   // gets 256KB

    int b3 = allocate(64);    // gets 64KB

    // Release Examples
    release(b1);

    release(b3);

    return 0;
}