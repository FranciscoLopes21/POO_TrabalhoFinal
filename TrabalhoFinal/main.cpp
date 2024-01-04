#include <iostream>
#include "Casino.h"
#include "XMLReader.h"

using namespace std;

int main()
{
    string nomeCasino;
    int maxJogadores, probabilidadeUser, horaAbertura, minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho;

    if (XMLReader::LoadCasinoData("XML_Projecto.xml",  nomeCasino, maxJogadores, probabilidadeUser, horaAbertura,minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho)) {

        Casino CS(nomeCasino);
        CS.CarregarDados(maxJogadores,probabilidadeUser,horaAbertura,minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho);
        CS.LoadMachinesFromXML("XML_Projecto.xml");

        cout << "Bem-Vindo ao " << nomeCasino << "!!!" << endl;

        CS.Run();

    } else {
        std::cerr << "Falha ao carregar as configurações do Casino a partir do XML." << std::endl;
        return 1;
    }



    return 0;
}
