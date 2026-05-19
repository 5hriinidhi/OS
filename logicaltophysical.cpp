#include <iostream>
#include <vector>

using namespace std;

void paging() {
    int pageSize, pages, logicalAddress;
    cout << "\nPaging\n";
    cout << "Enter page size: ";
    cin >> pageSize;
    cout << "Enter number of pages: ";
    cin >> pages;

    vector<int> pageTable(pages);
    cout << "Enter frame number for each page:\n";
    for (int i = 0; i < pages; i++) {
        cout << "Page " << i << ": ";
        cin >> pageTable[i];
    }

    cout << "Enter logical address: ";
    cin >> logicalAddress;

    int pageNumber = logicalAddress / pageSize;
    int offset = logicalAddress % pageSize;

    if (pageNumber < 0 || pageNumber >= pages) {
        cout << "Invalid logical address. Page fault.\n";
    } else {
        int physicalAddress = pageTable[pageNumber] * pageSize + offset;
        cout << "Page Number = " << pageNumber << endl;
        cout << "Offset = " << offset << endl;
        cout << "Physical Address = " << physicalAddress << endl;
    }
}

void segmentation() {
    int segments, segmentNumber, offset;
    cout << "\nSegmentation\n";
    cout << "Enter number of segments: ";
    cin >> segments;

    vector<int> base(segments);
    vector<int> limit(segments);

    for (int i = 0; i < segments; i++) {
        cout << "Segment " << i << " base: ";
        cin >> base[i];
        cout << "Segment " << i << " limit: ";
        cin >> limit[i];
    }

    cout << "Enter segment number: ";
    cin >> segmentNumber;
    cout << "Enter offset: ";
    cin >> offset;

    if (segmentNumber < 0 || segmentNumber >= segments || offset >= limit[segmentNumber]) {
        cout << "Invalid logical address. Segmentation fault.\n";
    } else {
        int physicalAddress = base[segmentNumber] + offset;
        cout << "Physical Address = " << physicalAddress << endl;
    }
}

int main() {
    int choice;
    while (true) {
        cout << "\n1. Paging\n2. Segmentation\n3. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) paging();
        else if (choice == 2) segmentation();
        else if (choice == 3) break;
        else cout << "Invalid choice.\n";
    }

    return 0;
}
