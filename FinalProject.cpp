//Final Project Operating Systems
//Vincent Truong
//Ashton Trumble
//Page Replacement Algorithm Implementation


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void simulateFIFO(const int* referenceString, int size, int frames);
void simulateLRU(const int* referenceString, int size, int frames);
void simulateOPT(const int* referenceString, int size, int frames);

int main() {
    // Prompt for input file
    string fileName;
    cout << "Enter the input file name (ex FIFO.txt, OPT.txt, or LRU.txt ): ";
    cin >> fileName;

    ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Error: Cannot open file " << fileName << endl;
        return 1;
    }

    string line;
    getline(inputFile, line); // Read the first line of the file
    inputFile.close();

    if (line.empty()) {
        cerr << "Error: Input file is empty or invalid." << endl;
        return 1;
    }

    // Parse the input
    char algorithm = line[0];          // The first character indicates the algorithm
    int frames = line[2] - '0';        // The second character indicates the number of frames (convert char to int)
    int referenceString[100];          // Array to hold the reference string
    int size = 0;                      // Size of the reference string

    stringstream ss(line.substr(4));   // Skip the first three characters (e.g., "F,4,")
    int num;
    while (ss >> num) {
        referenceString[size++] = num;
        if (ss.peek() == ',') ss.ignore(); // Ignore commas
    }

    // Choose and run the appropriate algorithm
    switch (algorithm) {
        case 'F':
            simulateFIFO(referenceString, size, frames);
            break;
        case 'L':
            simulateLRU(referenceString, size, frames);
            break;
        case 'O':
            simulateOPT(referenceString, size, frames);
            break;
        default:
            cerr << "Error: Unknown algorithm type '" << algorithm << "'" << endl;
            return 1;
    }

    return 0;
}

// FIFO Algorithm
void simulateFIFO(const int* referenceString, int size, int frames) {
    int memory[frames];     // Array for memory frames
    int index = 0;          // Tracks the position for replacement
    int pageFaults = 0;
    bool isPageFault;

    // Initialize memory array with -1
    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
    }

    cout << "FIFO Simulation:\n";
    for (int i = 0; i < size; i++) {
        isPageFault = true;

        // Check if the page is already in memory
        for (int j = 0; j < frames; j++) {
            if (memory[j] == referenceString[i]) {
                isPageFault = false;
                break;
            }
        }

        // If page fault occurs, replace the page at the current index
        if (isPageFault) {
            memory[index] = referenceString[i];
            index = (index + 1) % frames; // Move to the next frame (circular array)
            pageFaults++;
        }

        // Display the current state of memory
        cout << referenceString[i] << " -> ";
        for (int j = 0; j < frames; j++) {
            if (memory[j] != -1) cout << memory[j] << " ";
        }
        if (isPageFault) cout << "(Page Fault)";
        cout << endl;
    }

    cout << "Total Page Faults: " << pageFaults << endl;
}

// LRU Algorithm
void simulateLRU(const int* referenceString, int size, int frames) {
    int memory[frames];       // Array for memory frames
    int usage[frames];        // Array to track usage (age)
    int pageFaults = 0;
    bool isPageFault;

    // Initialize arrays
    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
        usage[i] = 0;
    }

    cout << "LRU Simulation:\n";
    for (int i = 0; i < size; i++) {
        isPageFault = true;

        // Check if the page is already in memory
        for (int j = 0; j < frames; j++) {
            if (memory[j] == referenceString[i]) {
                isPageFault = false;
                usage[j] = i; // Update usage for this page
                break;
            }
        }

        // If page fault occurs, replace the least recently used page
        if (isPageFault) {
            int lruIndex = 0;
            for (int j = 1; j < frames; j++) {
                if (usage[j] < usage[lruIndex]) {
                    lruIndex = j;
                }
            }
            memory[lruIndex] = referenceString[i];
            usage[lruIndex] = i;
            pageFaults++;
        }

        // Display the current state of memory
        cout << referenceString[i] << " -> ";
        for (int j = 0; j < frames; j++) {
            if (memory[j] != -1) cout << memory[j] << " ";
        }
        if (isPageFault) cout << "(Page Fault)";
        cout << endl;
    }

    cout << "Total Page Faults: " << pageFaults << endl;
}

// OPT Algorithm
void simulateOPT(const int* referenceString, int size, int frames) {
    int memory[frames];
    int pageFaults = 0;
    bool isPageFault;

    // Initialize memory array with -1
    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
    }

    cout << "OPT Simulation:\n";
    for (int i = 0; i < size; i++) {
        isPageFault = true;

        // Check if the page is already in memory
        for (int j = 0; j < frames; j++) {
            if (memory[j] == referenceString[i]) {
                isPageFault = false;
                break;
            }
        }

        // If page fault occurs, replace the optimal page
        if (isPageFault) {
            int replaceIndex = -1;
            int farthest = i;

            for (int j = 0; j < frames; j++) {
                bool found = false;
                for (int k = i + 1; k < size; k++) {
                    if (memory[j] == referenceString[k]) {
                        if (k > farthest) {
                            farthest = k;
                            replaceIndex = j;
                        }
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    replaceIndex = j;
                    break;
                }
            }

            memory[replaceIndex] = referenceString[i];
            pageFaults++;
        }

        // Display the current state of memory
        cout << referenceString[i] << " -> ";
        for (int j = 0; j < frames; j++) {
            if (memory[j] != -1) cout << memory[j] << " ";
        }
        if (isPageFault) cout << "(Page Fault)";
        cout << endl;
    }

    cout << "Total Page Faults: " << pageFaults << endl;
}
