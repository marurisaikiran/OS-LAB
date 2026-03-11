#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main() {
    string filename, line, word;
    int lines = 0, words = 0, chars = 0;

    cout << "Enter filename: ";
    cin >> filename;

    ifstream file(filename);
    while (getline(file, line)) {
        lines++;
        chars += line.length();
        stringstream ss(line);
        while (ss >> word) words++;
    }

    cout << "Lines: " << lines << endl;
    cout << "Words: " << words << endl;
    cout << "Characters: " << chars << endl;
    return 0;
}
