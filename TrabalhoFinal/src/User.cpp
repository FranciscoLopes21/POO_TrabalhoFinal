#include "User.h"

#include <random>
#include <ctime>

#include "Maquina.h"
#include "Casino.h"

User::User(int _nUser, string _nome, string _morada, int _idade, Casino* _casino)
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

    horaEntrada=0;
    horaSaida=0;
    tempoCasino=0;

    // Define o ponteiro para o Casino
    casino = _casino;
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

                if(maquinaAssociada->getEstado()== ON){

                    jogarNaMaquina();
                }
                else if (maquinaAssociada->getEstado()== AVARIADA){

                    cout << "Os jogadores estão a espera que a maquina seja reparada" << endl;

                }
            }else{
                //sair casino
                getMaquinaAssociada()->userSaiu();
                userSaiCasino();
            }

    }

}

void User::associarMaquina(Maquina* maquina) {

    setMaquinaAssociada(maquina);
    maquina->associarUser(this);

    User * u = maquina->getUserAtual();
    string nomeUser = u->getNome();

    time_t currentTime;
    horaEntrada = time(&currentTime);

    cout << "User " << nomeUser << " foi associado a Maquina: " <<  maquinaAssociada->getNome() << endl;

}

void User::entrarFilaEspera(Maquina* maquina) {
    maquina->entrarFilaEspera(this);
}

void User::jogarNaMaquina(){

    setJogadas(getJogadas()-1);
    maquinaAssociada->rodadas(this);

}

void User::userSaiCasino(){

    if(getMaquinaAssociada() != nullptr){

        setMaquinaAssociada(nullptr);

        int jogadores = casino->getJogadoresNoCasino();
        casino->setJogadoresNoCasino(jogadores - 1);

        time_t currentTime;
        horaSaida = time(&currentTime);

        //tempo em segundos
        tempoCasino = difftime(horaSaida, horaEntrada);

        // Exibir a diferença em minutos
        cout << "Diferença em minutos: " << tempoCasino * 0.0166667<< endl;

        //setAJogar(false);
        cout << "User " << getNome() << " saiu da maquina" << endl;

    }else{

        int jogadores = casino->getJogadoresNoCasino();
        casino->setJogadoresNoCasino(jogadores - 1);
        cout << "User " << getNome() << " saiu da maquina" << endl;
    }

}

//Calcular memoria User
int User::Memoria() {

    int mem = sizeof(*this);
    // Adicione a memória associada a membros dinâmicos, se houver
    // Exemplo considerando listas dinâmicas
    //mem += sizeof(User*) * filaEspera.size(); // tamanho da lista de ponteiros
    //mem += sizeof(Maquina*) * vizinhos.size(); // tamanho da lista de ponteiros
    return mem;

}
