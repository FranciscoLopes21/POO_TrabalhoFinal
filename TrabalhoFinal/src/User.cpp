#include "User.h"
#include <random>
#include <ctime>

#include "Maquina.h"
#include "Casino.h"

User::User(int _nUser, string _nome, string _morada, int _idade, Casino* _casino)
{//ctor
    // Inicializa as variveis da classe com os valores passados como argumentos
    nUser = _nUser;
    nome = _nome;
    morada = _morada;
    idade = _idade;
    casino = _casino;
    // Inicializa a carteira com um valor aleatório entre 1 e 1000
    carteira = rand() % 1000 + 1;
    // Inicializa outras variaveis da classe
    ganhos = 0;
    jogadas = 0;
    aJogar = false;
    maquinaAssociada = nullptr;
    horaEntrada=0;
    horaSaida=0;
    tempoCasino=0;
}

User::~User()
{
    //dtor
}

//Metodo Run do User
void User::Run(){
    if(maquinaAssociada!=nullptr){ //Verifica se o jogador está associado a uma máquina
           if(jogadas>=1){ //Verifica se o jogador tem jogadas
                if(maquinaAssociada->getEstado()== ON){ //Verifica se a máquina associada está ligada
                    jogarNaMaquina(); //Invoca função para o jogador jogar na maquina
                }
                else if (maquinaAssociada->getEstado()== AVARIADA){ //Se maquina estiver AVARIADA
                    cout << "Os jogadores estão a espera que a maquina seja reparada" << endl; //Printa mensagem de aviso
                }
            }else{ //Se os jogadores não tiverem mais jogadas
                //Processo de libertação da maquina e saida do casino
                getMaquinaAssociada()->userSaiu();
                userSaiCasino();
            }
    }
}

//Processo de associar jogador a maquina
void User::associarMaquina(Maquina* maquina) {
    setMaquinaAssociada(maquina); //Associar maquina ao jogador
    maquina->associarUser(this); //Associa jogador a maquina
    User * u = maquina->getUserAtual(); //Obtem jogador atual da maquina
    string nomeUser = u->getNome(); //Obtem o nome
    time_t currentTime;
    horaEntrada = time(&currentTime); //Obtem hora atual da entrada do jogador na maquina
    cout << "Jogador " << nomeUser << " foi escolheu a Maquina: " <<  maquinaAssociada->getNome() << endl; //Printa mensagem
}

//Processo jogador entra na fila de espera
void User::entrarFilaEspera(Maquina* maquina) {
    maquina->entrarFilaEspera(this); //Adicionar Jogador a lista de espera da maquina
}

//Processo jogar na maquina
void User::jogarNaMaquina(){
    setJogadas(getJogadas()-1); //Decrementa numero de jogadas do jogador
    maquinaAssociada->rodadas(this); //Chama função rodadas da maquina para jogar na maquina passando o jogador por parametro
}

//Processo jogador sai do casino
void User::userSaiCasino(){
    if(getMaquinaAssociada() != nullptr){ //Verifica se jogador esta associado a uma maquina
        setMaquinaAssociada(nullptr); //Desassocia jogador da maquina
        //Atualiza o número de jogadores no casino
        int jogadores = casino->getJogadoresNoCasino();
        casino->setJogadoresNoCasino(jogadores - 1);
        //Obtem hora atual da saida do jogador no casino
        time_t currentTime;
        horaSaida = time(&currentTime);
        //Calcula tempo total que o user passou no casino
        tempoCasino = difftime(horaSaida, horaEntrada);
        //Mostra tempo em minutos
        cout << "Tempo do jogador no casino em minutos: " << tempoCasino * 0.0166667<< endl;
        cout << "User " << getNome() << " saiu da maquina" << endl;
    }else{
        //Caso o jogador não esteja associado a nenhuma maquina
        int jogadores = casino->getJogadoresNoCasino();
        casino->setJogadoresNoCasino(jogadores - 1);
        //Printa mensagem
        cout << "User " << getNome() << " saiu da maquina" << endl;
    }
}

//Calcular memoria User
int User::Memoria() {
    int mem = sizeof(*this); //Tamanho atual do objeto User
    mem += sizeof(Casino*); //Tamanho do ponteiro Casino
    mem += sizeof(Maquina*); //Tamanho do ponteiro User
    mem += nome.size() + morada.size(); //Tamanho da string "nome" e "morada"
    return mem; //Retorna memoria total
}

//Mostra dados do jogador
void User::mostrarDados(){
    cout << "Numero de jogador: " << nUser << endl;
    cout << "Nome: " << nome << endl;
    cout << "Morada: " << morada << endl;
    cout << "Idade: " << idade << endl;
    cout << "Carteira: " << carteira << endl;
    cout << "Ganhos: " << ganhos << endl;
    cout << "Jogadas: " << jogadas << endl;
}
