#include "Maquina.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Casino.h"

Maquina::Maquina(int _nM, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino) {

    nMaquina = _nM;
    nome = _nome;
    x = _x;
    y = _y;
    premio = _premio;
    prob = _prob;
    tipo = _tipo;
    aposta = _aposta;


    estado = ON;
    temperaturaSensor = 0.0;
    nAvarias = 0;
    nJogos = 0;
    //utilizacao = false;
    userAtual=nullptr;
    casino = _casino;

}

Maquina::~Maquina()
{
    //dtor

    //destui Maquinas
    for (list<Maquina *>::iterator it = vizinhos.begin(); it != vizinhos.end(); ++it)
           delete (*it);
}

void Maquina::DisplayInfo() const {
    string estadoString;
    switch (estado) {
        case ON:
            estadoString = "ON";
            break;
        case OFF:
            estadoString = "OFF";
            break;
        case AVARIADA:
            estadoString = "AVARIADA";
            break;
        default:
            estadoString = "Unknown";
            break;
    }

    cout << "ID: " << nMaquina << " | Nome: " << nome << " | Posi��o: (" << x << ", " << y << ") | Pr�mio: " << premio << " | ProbG: " << prob << " | Estado: " << estadoString << endl;
}

void Maquina::Run(){
    cout << "Eu M�quina: " << nMaquina << " Estou ligada" << endl;
    //a temperatura aumenta quando liga e vai aumentado se estiver a ser utilizada
    //temperaturaSensor += 1;

    cout << "Lista de vizinhos: " << vizinhos.size() << endl;

    verificaEstado();


}




bool Maquina::verificaEstado()
{

    //estado = "bom";
    if(estado==ON){
        cout << "\033[1;32mMaquina Ligada\033[0m"<< nMaquina << "\n";
    }else if(estado==OFF){
        cout << "\033[1;33mMaquina Desligada\033[0m\n";
    }else if(estado==AVARIADA){
        cout << "\033[1;31mMaquina Avariada\033[0m\n";
    }
    return true;
}

void Maquina::Desligar() {
    if(estado == AVARIADA){
        estado = AVARIADA;
    }else{
        estado = OFF; // Altera o estado da m�quina para OFF
    }
}

void Maquina::Ligar() {
    if(estado == AVARIADA){
        estado = AVARIADA;
    }else{
        estado = ON; // Altera o estado da m�quina para ON
    }

}

void Maquina::avariaMaquina(){
    estado = AVARIADA;
    nAvarias ++;
}

void Maquina::entrarFilaEspera(User* user) {
    if (userAtual != nullptr) {
        filaEspera.push_back(user);
        cout << "User " << user->getNome() << " entrou na fila de espera para a m�quina " << nome << endl;
    }
}

void Maquina::associarUser(User* user) {

    if(getUserAtual() ==nullptr){
        setUserAtual(user);

        int rodadas = user->getCarteira()/this->getAposta();
        user->setJogadas(3);

        cout << "User " << user->getNome() << " trocou carteira " << user->getCarteira() << " por " << user->getJogadas() << " fichas" << endl;
        //cout << "User " << user->getJogadas() << " jogadas para jogar" << nome << endl;
    }

    /*if (userAtual == nullptr) {
        userAtual = user;
        setUtilizacao(true);
        int rodadas = user->getCarteira()/this->getAposta();
        user->setJogadas(3);
        cout << "User " << user->getNome() << " come�ou a jogar na m�quina " << nome << endl;
        cout << "User " << user->getJogadas() << " jogadas para jogar" << nome << endl;
    }*/
}

void Maquina::rodadas(User* user){

    // Gerar um �ndice aleat�rio usando a opera��o de m�dulo
    float randomProb = rand() % 100;


    if (randomProb <= getProb()) {
        cout << "Jogador " << user->getNome() << " ganhou na m�quina " << nome << "  premio   "<< premio << endl;
        float ganhosUser = user->getGanhos() + premio;
        user->setGanhos(ganhosUser);


        subirProbabilidade();



    } else {
        cout << "Jogador " << user->getNome() << " perdeu na m�quina " << nome << endl;
    }


    setNJogos(nJogos + 1);




    // Se houver usu�rios na fila de espera, inicie o pr�ximo
    /*if (!filaEspera.empty()) {
        usuarioAtual = filaEspera.front();
        filaEspera.pop();
        std::cout << "Usu�rio " << usuarioAtual->getNome() << " come�ou a jogar na m�quina " << nome << std::endl;
        // L�gica de iniciar o jogo para o pr�ximo usu�rio...
    } else {
        usuarioAtual = nullptr;
    }*/

}

void Maquina::userSaiu() {

    setUserAtual(nullptr);
    cout << "Maquina " << getNome() << " ficou livre" << endl;
    //setUtilizacao(false);
}

bool Maquina::repararMaquina(){

    bool reparado= false;


    estado = ON;
    reparado = true;


    return reparado;


}

void Maquina::adicionarVizinho(Maquina* vizinho) {
    vizinhos.push_back(vizinho);
}


void Maquina::subirProbabilidade(){

    casino->SubirProbabilidadeVizinhas(this,2,vizinhos);


}

bool Maquina::removerVizinho(int id_maq){

    for (list<Maquina *>::iterator it = vizinhos.begin(); it != vizinhos.end();it++) {
        if ((*it)->getID() == id_maq) {
            cout << "M�quina vizinha removida com ID: " << id_maq << endl;
            vizinhos.erase(it); // Remove a m�quina da lista
            return true;
            cout << "M�quina vizinha removida." << endl;
        }
    }

    return false;

}

