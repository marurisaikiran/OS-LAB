#include<iostream>
#include<dirent.h>

using namespace std;

int main() {
    DIR *dir = opendir(".");
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
        cout << entry->d_name << endl;

    closedir(dir);
    return 0;
}
