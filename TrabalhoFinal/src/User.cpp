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
    jogadas = 0;
    aJogar = false;
    maquinaAssociada = nullptr;
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
    cout << "jogadas: " << jogadas << endl;
    //bool aJogar;
    //Maquina * maquinaAssociada;

    if(maquinaAssociada!=nullptr){
        if(jogadas>=1){
            jogarNaMaquina();
        }else{
            //sair casino
            getMaquinaAssociada()->userSaiu();
            userSaiCasino();

        }
    }


}

void User::associarMaquina(Maquina* maquina) {
    /*maquinaAssociada = maquina;
    maquina->associarUser(this);
    setAJogar(true);*/

    setMaquinaAssociada(maquina);
    maquina->associarUser(this);

    User * u = maquina->getUserAtual();
    string nomeUser = u->getNome();

    cout << "User " << nomeUser << " foi associado a Maquina: " <<  maquinaAssociada->getNome() << endl;
}

void User::entrarFilaEspera(Maquina* maquina) {
    maquina->entrarFilaEspera(this);
}

void User::jogarNaMaquina(){

    //if(maquinaAssociada!=nullptr){
        setJogadas(getJogadas()-1);
        maquinaAssociada->rodadas(this);
    //}
}

void User::userSaiCasino(){

    setMaquinaAssociada(nullptr);
    //setAJogar(false);
    std::cout << "User " << getNome() << " saiu da maquina" << std::endl;

}
