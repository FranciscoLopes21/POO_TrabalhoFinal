#include <iostream>
#include "Casino.h"
#include "XMLReader.h"

using namespace std;

int main()
{
    string nomeCasino;
    int maxJogadores, probabilidadeUser, horaAbertura, horaFecho;

    if (XMLReader::LoadCasinoData("XML_Projecto.xml",  nomeCasino, maxJogadores, probabilidadeUser, horaAbertura, horaFecho)) {

        Casino CS(nomeCasino);
        CS.CarregarDados(maxJogadores,probabilidadeUser,horaAbertura,horaFecho);

        cout << "Bem-Vindo ao " << nomeCasino << "!!!" << endl;

        CS.Run();

    } else {
        std::cerr << "Falha ao carregar as configurações do Casino a partir do XML." << std::endl;
        return 1;
    }



    return 0;
}
