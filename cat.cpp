#include <iostream>
#include <cstdio>
using namespace std;

int main(int argc, char *argv[]) {
    FILE *fp;
    char ch;

    // Check if filename is provided
    if (argc < 2) {
        cout << "Usage: ./cat <filename>" << endl;
        return 1;
    }

    // Open file in read mode
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        cout << "Error opening file" << endl;
        return 1;
    }

    // Read and display file contents
    while ((ch = fgetc(fp)) != EOF) {
        cout << ch;
    }

    fclose(fp);
    return 0;
}

