#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
using namespace std;
int main(int argc, char* argv[])
{
    DIR* dir;
    dirent* pdir;
    struct stat st;
    tm time;
    dir = opendir((argv[1] ? argv[1] : "."));
    if (dir == NULL)
    {
        exit(1);
    }
    while ((pdir = readdir(dir)) != NULL)
    {
        stat(pdir->d_name, &st);
        time = *localtime(&st.st_mtime);
        cout << st.st_mode << "\t"
            << st.st_size << "\t"
            << (time.tm_mday < 10 ? "0" : "") << time.tm_mday << "."
            << (time.tm_mon + 1 < 10 ? "0" : "") << time.tm_mon + 1 << "."
            << time.tm_year + 1900 << ". "
            << (time.tm_hour < 10 ? "0" : "") << time.tm_hour << ":"
            << (time.tm_min < 10 ? "0" : "") << time.tm_min << "\t"
            << pdir->d_name << endl;
    }
    closedir(dir);
    return 0;
}