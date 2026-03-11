#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    if (unlink(filename.c_str()) == 0)
        cout << "File deleted successfully\n";
    else
        cout << "Error deleting file\n";

    return 0;
}
