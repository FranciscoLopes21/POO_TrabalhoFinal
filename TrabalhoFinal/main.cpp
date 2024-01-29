#include <iostream>
#include "Casino.h"
#include "XMLReader.h"

using namespace std;

int main()
{
    string nomeFicheiro = "XML_Projecto.xml";
    string nomeCasino = "Casino Figueira";

    Casino CS(nomeCasino);
    CS.Load(nomeFicheiro);
    CS.LoadMaquina(nomeFicheiro);

    cout << "Bem-Vindo ao " << nomeCasino << "!!!" << endl;

    CS.Run();

    return 0;
}
