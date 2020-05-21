#include <iostream>
#include <dirent.h> // dirent struct , opendir(), closedir()
#include <sys/types.h> // for dirent.h
#include <sys/stat.h> // stat struct
#include <ctime> // localtime() , tm struct
#include <pwd.h> // getpwuid(), passwd_t

using namespace std;

const char chars[] = "rwxrwxrwx";

char type (const struct stat mode); // return file type flag

int main(int argc, char* argv[])
{
    DIR* dir;
    dirent* pdir;
    dir = opendir((argc == 2 ? argv[1] : ".")); // open current or defined directory 
    if (dir == NULL)
    {
        exit(1);
    }
    struct stat st;
    while ((pdir = readdir(dir)) != NULL)
    {
        stat(pdir->d_name, &st); // get info about file
        char flags[10];
        for (size_t i = 0; i < 9; i++) {
            flags[i] = (st.st_mode & (1 << (8 - i))) ? chars[i] : '-'; // generate file permission flag
        }
        flags[9] = '\0';
        cout << type(st) << flags << '\t' << st.st_nlink; // print file type and permission info and hard link number
        cout << (*getpwuid(st.st_uid)).pw_name << '\t'; // print owner name by id
        cout << (*getpwuid(st.st_gid)).pw_name << '\t'; // print owner group name by id
        tm time = *localtime(&st.st_mtime); // interpret last modification time in normal format
        cout << st.st_size << "\t" 
            //next 5 lines print last modification time
            << (time.tm_mday < 10 ? "0" : "") << time.tm_mday << "."
            << (time.tm_mon + 1 < 10 ? "0" : "") << time.tm_mon + 1 << "."
            << time.tm_year + 1900 << ". "
            << (time.tm_hour < 10 ? "0" : "") << time.tm_hour << ":"
            << (time.tm_min < 10 ? "0" : "") << time.tm_min << "\t"
            << pdir->d_name << endl; // print file name
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
