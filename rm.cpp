#include <iostream>
#include <cstdio>   // for remove()

using namespace std;

int main(int argc, char* argv[]) {
    // Check if filename is provided
    if (argc < 2) {
        cout << "Usage: ./rm <filename>" << endl;
        return 1;
    }

    // Attempt to delete the file
    if (remove(argv[1]) == 0) {
        cout << "File deleted successfully" << endl;
    } else {
        cout << "Error deleting file" << endl;
    }

    return 0;
}

