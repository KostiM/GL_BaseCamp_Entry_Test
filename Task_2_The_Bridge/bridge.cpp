#include <iostream>
#include <string> // stoi()
using namespace std;
int main(int argc, char *argv[])
{
    if (argv[1] && argv[2])
    {
        int N = 0,
            D = 0;
        try
        {
            N = stoi(argv[1]),
            D = stoi(argv[2]);
        }
        catch (std::exception ex)
        {
            cout << "Incorrect parameter(s). " << ex.what() << endl;
            return 1;
        }
        if (N < 0 || D < 0)
        {
            cout << "Incorrect parameter(s)." << endl;
            return 1;
        }
        cout << (2*D+(N-1)*D)*N/2 << endl; //the answer is sum of arithmetic progression
    }
    else
    {
        cout << "Program should be run with command line parameters." << endl;
        return 1;
    }
    return 0;
}