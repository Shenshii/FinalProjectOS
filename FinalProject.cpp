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
void simulateOPT(const int* referenceString, int size, int frames);

int main() {
    while (true) { // Loop to allow multiple simulations until the user decides to exit
        string fileName; // Declare a variable to store the file name
        cout << "Enter the input file name (ex FIFO.txt, OPT.txt), or type 'Exit' to quit: "; // Prompt user
        cin >> fileName; // Read the input file name or exit command from user

        if (fileName == "Exit" || fileName == "exit") { // Check if the user wants to exit
            break; // Exit the loop
        }

        ifstream inputFile(fileName); // Open the specified file
        if (!inputFile) { // Check if the file opening failed
            cerr << "Error: Cannot open file " << fileName << endl; // Print error message if file cannot be opened
            continue; // Skip to the next iteration of the loop
        }

        string line; // Declare a variable to store lines read from the file
        if (!getline(inputFile, line) || line.empty()) { // Attempt to read the first line and check if it is empty
            cerr << "Error: Input file is empty or invalid." << endl; // Print error message if the line is empty
            continue; // Skip to the next iteration of the loop
        }
        inputFile.close(); // Close the input file

        if (line.size() < 4 || line[1] != ',' || line[3] != ',') { // Basic format check for expected input format
            cerr << "Error: Input file format is incorrect." << endl;
            continue; // Skip to the next iteration of the loop
        }

        char algorithm = line[0]; // The first character indicates the algorithm
        int frames = line[2] - '0'; // The second character indicates the number of frames (convert char to int)
        if (frames <= 0) { // Check if frames are a valid number
            cerr << "Error: Number of frames must be positive." << endl;
            continue; // Skip to the next iteration of the loop
        }

        int referenceString[100]; // Array to hold the reference string
        int size = 0; // Size of the reference string

        stringstream ss(line.substr(4)); // Create a stringstream from the substring that skips the first three characters
        int num; // Variable to store numbers read from the stringstream
        while (ss >> num) { // Read numbers from the stringstream
            referenceString[size++] = num; // Store the numbers in the referenceString array
            if (ss.peek() == ',') ss.ignore(); // Ignore commas in the stringstream
        }

        if (size == 0) { // Check if no reference numbers were found
            cerr << "Error: No reference string found in input." << endl;
            continue; // Skip to the next iteration of the loop
        }

        // Choose and run the appropriate algorithm
        switch (algorithm) {
            case 'F':
                simulateFIFO(referenceString, size, frames); // Call the simulateFIFO function if 'F' is chosen
                break;
            case 'O':
                simulateOPT(referenceString, size, frames); // Call the simulateOPT function if 'O' is chosen
                break;
            default:
                cerr << "Error: Unknown algorithm type '" << algorithm << "'" << endl; // Print an error message if the algorithm type is unknown
                continue; // Skip to the next iteration of the loop
        }
    }
    
    cout << "Exiting the program." << endl; // Inform the user that the program is exiting
    return 0; // Return 0 to indicate successful completion
}



// FIFO Algorithm
void simulateFIFO(const int* referenceString, int size, int frames) {
    int* memory = new int[frames]; // Dynamically allocate memory for frame storage
    int index = 0; // Initialize index to keep track of the circular queue position
    int pageFaults = 0; // Counter for page faults
    bool isPageFault; // Flag to check if a page fault occurred

    for (int i = 0; i < frames; i++) {
        memory[i] = -1; // Initialize memory slots to -1 indicating empty
    }

    cout << "FIFO Simulation:\n"; // Print simulation start message
    for (int i = 0; i < size; i++) {
        isPageFault = true; // Assume a page fault occurred

        for (int j = 0; j < frames; j++) {
            if (memory[j] == referenceString[i]) {
                isPageFault = false; // Set page fault to false if page is already in memory
                break;
            }
        }

        if (isPageFault) {
            memory[index] = referenceString[i]; // Replace the page in the frame at the current index
            index = (index + 1) % frames; // Update index to the next frame position, wrap around using modulo
            pageFaults++; // Increment page fault count
        }

        cout << referenceString[i] << " -> "; // Output the current reference string item
        for (int j = 0; j < frames; j++) {
            if (memory[j] != -1) cout << memory[j] << " "; // Print the contents of the memory frames
        }
        if (isPageFault) cout << "(Page Fault)"; // Print a page fault message if a fault occurred
        cout << endl;
    }

    cout << "Total Page Faults: " << pageFaults << endl; // Print the total number of page faults

    delete[] memory; // Free the dynamically allocated memory
}


// OPT Algorithm
void simulateOPT(const int* referenceString, int size, int frames) {
    int* memory = new int[frames]; // Dynamically allocate memory for frame storage
    int pageFaults = 0; // Counter for page faults
    bool isPageFault; // Flag to check if a page fault occurred

    for (int i = 0; i < frames; i++) {
        memory[i] = -1; // Initialize memory slots to -1 indicating empty
    }

    cout << "OPT Simulation:\n"; // Print simulation start message
    for (int i = 0; i < size; i++) { // For loop for increasing size
        isPageFault = true; // Assume a page fault occurred

        for (int j = 0; j < frames; j++) {
            if (memory[j] == referenceString[i]) {
                isPageFault = false; // Set page fault to false if page is already in memory
                break;
            }
        }

        if (isPageFault) {
            int replaceIndex = -1; // Initialize the index for the page to be replaced
            int farthest = i; // Initialize the farthest use index
            for (int j = 0; j < frames; j++) {
                int foundLater = -1; // Initialize found index for later used pages
                for (int k = i + 1; k < size; k++) {
                    if (memory[j] == referenceString[k]) {
                        foundLater = k; // Update found index when the page is found later in the reference string
                        break;
                    }
                }

                if (foundLater == -1) { // If the page is not found later, it is the best to replace
                    replaceIndex = j; // Update the replace index
                    break;
                } else if (foundLater > farthest) {
                    farthest = foundLater; // Update the farthest index if the current one is farther
                    replaceIndex = j; // Update the replace index
                }
            }

            if (replaceIndex == -1) replaceIndex = 0; // Set to replace the first frame if no better option is found

            memory[replaceIndex] = referenceString[i]; // Replace the page at the replace index
            pageFaults++; // Increment the page fault count
        }

        cout << referenceString[i] << " -> "; // Output the current reference string item
        for (int j = 0; j < frames; j++) {
            if (memory[j] != -1) cout << memory[j] << " ";  // Print the contents of the memory frames
        }
        if (isPageFault) cout << "(Page Fault)"; // Print a page fault message if a fault occurred
        cout << endl;
    }

    cout << "Total Page Faults: " << pageFaults << endl; // Print the total number of page faults

    delete[] memory; // Free the dynamically allocated memory
}

