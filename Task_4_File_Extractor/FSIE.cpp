#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        cerr << "The program should be run with command line parameter." << endl;
        exit(1);
    }
    uint16_t entnum = 0;
    ifstream file(argv[1], ios::in | ios::binary);
    file.seekg(2, ios::beg);
    file.read((char*)&entnum, 2);
    entnum = entnum & 0x0000FFFF; // ignore 3rd & 4th byte
    vector<unsigned> fsize(entnum);
    vector<unsigned> foffset(entnum);
    cout << "entnum = " << entnum << endl;
    for (unsigned i = 0; i < entnum; ++i)
    {
        char buff[4];
        file.read(buff, 3);
        unsigned* buffp = reinterpret_cast<unsigned*>(buff);
        fsize[i] = *buffp;
        fsize[i] = (unsigned)(fsize[i] & 0x00FFFFFF); // ignore 4th byte
        file.read(buff, 4);
        foffset[i] = *buffp;
    }
    for (unsigned i = 0; i < entnum; ++i)
    {
        cout << "fsize = " << fsize[i] << endl;
        cout << "foffset = " << foffset[i] << endl;
        file.seekg(foffset[i] + fsize[i], ios::beg);
        string filename;
        getline(file, filename, '\0');
        if (argc == 3) {
            filename = string(argv[2]) + '/' + filename;
        }
        cout << "finename = " << filename << endl;
        ofstream newfile(filename, ios::out | ios::binary);
        file.seekg(foffset[i], ios::beg);
        char* buff = new char[fsize[i]];
        file.read(buff, fsize[i]);
        newfile.write(buff, fsize[i]);
        delete[] buff;

        newfile.close();
    }
    file.close();
    return 0;
}