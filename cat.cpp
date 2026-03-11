#include <iostream>
#include <cstdio>
using namespace std;

int main() {
    char filename[50];
    cout << "Enter filename: ";
    cin >> filename;

    FILE *fp = fopen(filename, "r");
    char ch;

    while ((ch = fgetc(fp)) != EOF)
        cout << ch;

    fclose(fp);
    return 0;
}
