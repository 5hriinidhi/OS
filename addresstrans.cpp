#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ─────────────────────────────────────────────────────────
// Paging: Logical Address -> Physical Address
// ─────────────────────────────────────────────────────────
void paging()
{
    cout << "\n=== Paging ===\n";

    int pageSize;
    int logicalAddr;

    cout << "Enter page size (bytes): ";
    cin >> pageSize;

    // Page Table:
    // index = page number
    // value = frame number
    vector<int> pageTable =
    {
        2, 4, 1, 7, 5
    };

    cout << "Page Table:\n";

    for (int i = 0; i < (int)pageTable.size(); i++)
    {
        cout << "  Page "
             << i
             << " -> Frame "
             << pageTable[i]
             << "\n";
    }

    cout << "Enter logical address: ";
    cin >> logicalAddr;

    int pageNum = logicalAddr / pageSize;

    int offset = logicalAddr % pageSize;

    // Invalid page
    if (pageNum >= (int)pageTable.size())
    {
        cout << "ERROR: Invalid page number!\n";
        return;
    }

    int frameNum = pageTable[pageNum];

    int physAddr =
        frameNum * pageSize + offset;

    cout << "\nLogical Address  : "
         << logicalAddr << "\n";

    cout << "Page Number      : "
         << pageNum << "\n";

    cout << "Offset           : "
         << offset << "\n";

    cout << "Frame Number     : "
         << frameNum << "\n";

    cout << "Physical Address : "
         << physAddr << "\n";

    cout << "Formula: Frame("
         << frameNum
         << ") x PageSize("
         << pageSize
         << ") + Offset("
         << offset
         << ") = "
         << physAddr
         << "\n";
}

// ─────────────────────────────────────────────────────────
// Segmentation
// ─────────────────────────────────────────────────────────

// Segment Structure
struct Segment
{
    string name;

    int base;

    int limit;
};

void segmentation()
{
    cout << "\n=== Segmentation ===\n";

    vector<Segment> segTable =
    {
        {"Code",  1000, 400},
        {"Data",  2000, 600},
        {"Stack", 3000, 200}
    };

    cout << "Segment Table:\n";

    cout << "No  Name    Base  Limit\n";

    for (int i = 0; i < (int)segTable.size(); i++)
    {
        cout << i
             << "   "
             << segTable[i].name
             << "   "
             << segTable[i].base
             << "   "
             << segTable[i].limit
             << "\n";
    }

    int segNum;
    int offset;

    cout << "Enter segment number: ";
    cin >> segNum;

    cout << "Enter offset        : ";
    cin >> offset;

    // Invalid segment
    if (segNum >= (int)segTable.size())
    {
        cout << "Invalid segment!\n";
        return;
    }

    // Offset exceeds segment limit
    if (offset >= segTable[segNum].limit)
    {
        cout << "SEGMENTATION FAULT! "
             << "Offset exceeds limit.\n";

        return;
    }

    int physAddr =
        segTable[segNum].base + offset;

    cout << "Segment       : "
         << segTable[segNum].name
         << "\n";

    cout << "Physical Addr : Base("
         << segTable[segNum].base
         << ") + Offset("
         << offset
         << ") = "
         << physAddr
         << "\n";
}

// ─────────────────────────────────────────────────────────
// Main Function
// ─────────────────────────────────────────────────────────
int main()
{
    paging();

    segmentation();

    return 0;
}
