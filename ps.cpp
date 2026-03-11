#include <iostream>
#include <dirent.h>
#include <cctype>
using namespace std;

int main() {
    DIR *dir = opendir("/proc");
    struct dirent *entry;

    cout << "PID\n";
    while ((entry = readdir(dir)) != NULL)
        if (isdigit(entry->d_name[0]))
            cout << entry->d_name << endl;

    closedir(dir);
    return 0;
}
