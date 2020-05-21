#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <pwd.h>

using namespace std;

const char chars[] = "rwxrwxrwx";

char type (const struct stat mode);

int main(int argc, char* argv[])
{
    DIR* dir;
    dirent* pdir;
    struct stat st;
    tm time;
    const char* dirname = (argc == 2 ? argv[1] : ".");
    dir = opendir(dirname);
    if (dir == NULL)
    {
        exit(1);
    }
    if (dir == NULL)
    {
        exit(1);
    }
    while ((pdir = readdir(dir)) != NULL)
    {
        stat(pdir->d_name, &st);
        time = *localtime(&st.st_mtime);
        char flags[10];
        for (size_t i = 0; i < 9; i++) {
            flags[i] = (st.st_mode & (1 << (8 - i))) ? chars[i] : '-';
        }
        flags[9] = '\0';
        cout << type(st) << flags << '\t' << st.st_nlink;
        struct passwd uname = *getpwuid(st.st_uid);
        cout << uname.pw_name << '\t';
        uname = *getpwuid(st.st_gid);
        cout << uname.pw_name << '\t';
        cout<< st.st_size << "\t"
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
    char type(const struct stat st)
    {
        switch (st.st_mode & S_IFMT) {
        case S_IFREG:
            return '-';
            break;
        case S_IFDIR:
            return 'd';
            break;
        case S_IFBLK:
            return 'b';
            break;
        case S_IFCHR:
            return 'c';
            break;
        case S_IFIFO:
            return 'p';
        }
        return 0;
    }
