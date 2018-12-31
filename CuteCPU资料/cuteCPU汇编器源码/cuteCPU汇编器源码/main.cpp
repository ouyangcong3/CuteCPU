#include <iostream>
#include "CAsm.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    CAsm asmfile;

    asmfile.compile("./sourcefile.a");


    return 0;
}
