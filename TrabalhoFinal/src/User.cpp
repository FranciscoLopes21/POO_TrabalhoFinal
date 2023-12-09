#include "User.h"

#include <random>
#include <ctime>

#include "Maquina.h"

User::User(int _nUser, string _nome, string _morada, int _idade)
{
    //ctor
    nUser = _nUser;
    nome = _nome;
    morada = _morada;
    idade = _idade;

    carteira = rand() % 1000 + 1;
    ganhos = 0;
    aJogar = false;
}

User::~User()
{
    //dtor
}

void User::Run(){

    cout << "Eu user: " << nUser << endl;
    cout << "nome: " << nome << endl;
    cout << "morada: " << morada << endl;
    cout << "idade: " << idade << endl;
    cout << "carteira: " << carteira << endl;
    cout << "ganhos: " << ganhos << endl;

}

void User::associarMaquina(Maquina* maquina) {
    maquinaAssociada = maquina;
    maquina->associarUser(this);

    cout << "Maquina Associada " <<  maquinaAssociada->getNome()  << endl;
}

void User::entrarFilaEspera(Maquina* maquina) {
    maquina->entrarFilaEspera(this);
}
