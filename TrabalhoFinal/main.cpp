#include <iostream>
#include "Casino.h""

using namespace std;

int main()
{
    Casino CS;
    string nomeCasino = CS.getNome();
    cout << "Bem-Vindo ao " << nomeCasino << "!!!" << endl;

    CS.Run();
    return 0;
}
