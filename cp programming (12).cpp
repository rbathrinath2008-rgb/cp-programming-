#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include <vector>

using namespace std;

// Custom Allocator
template <typename T>
class CustomAllocator {
public:
    using value_type = T;

    CustomAllocator() {}

    template <typename U>
    CustomAllocator(const CustomAllocator<U>&) {}

    T* allocate(size_t n) {
        cout << "[Allocator] Allocating memory for " << n << " object(s)\n";
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) {
        cout << "[Allocator] Deallocating memory\n";
        ::operator delete(p);
    }
};

// Comparison operators for allocator support
template <typename T, typename U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&) { return false; }

// File Record
struct FileRecord {
    char fileName[50];
    int fileSize;
};

// Add file record (saves to file and loads into custom allocator vector)
void addFile(vector<FileRecord, CustomAllocator<FileRecord>>& memoryFiles) {
    FileRecord file;

    cout << "Enter File Name: ";
    cin >> file.fileName;

    cout << "Enter File Size (KB): ";
    cin >> file.fileSize;

    // Save to binary file
    ofstream outFile("filesystem.dat", ios::binary | ios::app);

    if (outFile) {
        outFile.write(reinterpret_cast<char*>(&file), sizeof(FileRecord));
        outFile.close();

        // Also add to our custom allocator-backed vector
        memoryFiles.push_back(file);

        cout << "File record added successfully.\n";
    } else {
        cout << "Unable to open file.\n";
    }
}

// Display file records from the custom allocator container
void displayFiles(const vector<FileRecord, CustomAllocator<FileRecord>>& memoryFiles) {
    if (memoryFiles.empty()) {
        cout << "No file records found in memory.\n";
        return;
    }

    cout << "\n--- File System Records (via Custom Allocator) ---\n";
    for (size_t i = 0; i < memoryFiles.size(); ++i) {
        cout << "File Name : " << memoryFiles[i].fileName << endl;
        cout << "File Size : " << memoryFiles[i].fileSize << " KB\n";
        cout << "--------------------------\n";
    }
}

int main() {
    // Initialize vector using your CustomAllocator
    vector<FileRecord, CustomAllocator<FileRecord>> memoryFiles;

    // Load existing records from disk into memory upon startup
    ifstream inFile("filesystem.dat", ios::binary);
    if (inFile) {
        FileRecord file;
        while (inFile.read(reinterpret_cast<char*>(&file), sizeof(FileRecord))) {
            memoryFiles.push_back(file);
        }
        inFile.close();
    }

    int choice;

    do {
        cout << "\nMINI FILE SYSTEM SIMULATOR\n";
        cout << "1. Add File\n";
        cout << "2. Display Files\n";
        cout << "3. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addFile(memoryFiles);
                break;

            case 2:
                displayFiles(memoryFiles);
                break;

            case 3:
                cout << "Exiting File System Simulator...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 3);

    return 0;
}