#include "Maquina.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Casino.h"

Maquina::Maquina(int _idM, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino) {
    idMaquina = _idM;
    nome = _nome;
    x = _x;
    y = _y;
    premio = _premio;
    prob = _prob;
    tipo = _tipo;
    aposta = _aposta;
    casino = _casino;
    userAtual=nullptr;
    quente = false;
    estado = ON;
    temperaturaSensor = 20.0;
    nAvarias = 0;
    nJogos = 0;
}

Maquina::~Maquina()
{
    //dtor

    //destui Maquinas
    for (list<Maquina *>::iterator it = vizinhos.begin(); it != vizinhos.end(); ++it)
           delete (*it);
}

void Maquina::informacaoMaquina() {
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
    if(estado== ON){ //verifica se o estado é ON para ajustar a cor do estado quando printa
        //printa dados
        cout << "| ID: " << getID() << " | Nome: " << getNome() << " | Tipo: " << getTipo() << " | Probabilidade: " << getProb() << " | Estado: \033[1;32m"<<estadoString<<"\033[0m" << " | Temperatura: " << getTemperaturaSensor() << endl;
    }else if (estado== OFF){ //verifica se o estado é OFF para ajustar a cor do estado quando printa
        //printa dados
        cout << "| ID: " << getID() << " | Nome: " << getNome() << " | Tipo: " << getTipo() << " | Probabilidade: " << getProb() << " | Estado: \033[1;33m"<<estadoString<<"\033[0m" << " | Temperatura: " << getTemperaturaSensor() << endl;
    }else if (estado== AVARIADA){ //verifica se o estado é AVARIADA para ajustar a cor do estado quando printa
        //printa dados
        cout << "| ID: " << getID() << " | Nome: " << getNome() << " | Tipo: " << getTipo() << " | Probabilidade: " << getProb() << " | Estado: \033[1;31m"<<estadoString<<"\033[0m" << " | Temperatura: " << getTemperaturaSensor() << endl;
    }
}

void Maquina::Run(){
    cout << endl;
    cout << "Eu Máquina: " << idMaquina << " Estou ligada" << endl;
    cout << "Lista de vizinhos: " << vizinhos.size() << endl;
    verificaEstado();
    //verificar probabilidade muito grande//
    if(prob >= 35.0){
        cout << "\033[3;41;30m Probabilidade da maquina: " << prob << "\033[0m\t\t" << endl;//VERMELHO
    }
    else if(prob  >= 30.0){
        cout << "\033[3;43;30m Probabilidade da maquina: " << prob << "\033[0m\t\t" << endl;//AMARELO
    }
    else if(prob < 29.0){
        cout << "\033[3;42;30m Probabilidade da maquina: " << prob << "\033[0m\t\t" << endl;//VERDE
    }
}

bool Maquina::verificaEstado()
{
    if(estado==ON){
        cout << "\033[1;32mMaquina Ligada\033[0m"<< idMaquina << "\n";
    }else if(estado==OFF){
        cout << "\033[1;33mMaquina Desligada\033[0m\n";
    }else if(estado==AVARIADA){
        cout << "\033[1;31mMaquina Avariada\033[0m\n";
    }
    return true;
}

bool Maquina::Desligar(){
    bool desligado = false;
    if(estado == AVARIADA){
        cout << "Esta maquina encontra-se avariada!" << endl;
        cout << "Deseja reparar (S/N): " << endl; //printa se deseja repara alguam das maquinas que apareceu
        char repar; //variavel do tipo char para guardar resposta
        cin >> repar; //guarda resposta
        if(repar == 'S' || repar == 's'){ //verifica se escolheram reparar
            repararMaquina(); //chama função complementar para reparar maquina
            estado = OFF; // Altera o estado da máquina para OFF
            desligado = true;
            saemTodos();
        }
        else{
            cout << "Esta maquina não foi desligada, permanecerá avariada!" << endl;
            estado = AVARIADA;
        }
    }else if(estado == ON){
        estado = OFF; // Altera o estado da máquina para OFF
        desligado = true;
        saemTodos();
    }
    return desligado;
}

void Maquina::saemTodos(){
    if (getUserAtual()== nullptr){
        if(filaEspera.size() > 0){
            for (list<User *>::iterator it = filaEspera.begin(); it != filaEspera.end();it++) {
                (*it)->userSaiCasino();
            }
        }
    }else if(getUserAtual()!= nullptr){
        User *user = getUserAtual();
        user->userSaiCasino();
        if(filaEspera.size() > 0){
            for (list<User *>::iterator it = filaEspera.begin(); it != filaEspera.end();it++) {
                (*it)->userSaiCasino();
            }
        }
    }
}

void Maquina::Ligar() {
    if(estado == AVARIADA){ //verifica se a maquina esta AVARIADA
        estado = AVARIADA;
    }else{
        estado = ON; // Altera o estado da máquina para ON
    }
}

void Maquina::avariaMaquina(){
    estado = AVARIADA;
    nAvarias ++;
}

void Maquina::entrarFilaEspera(User* user) {
    if (userAtual != nullptr) {
        filaEspera.push_back(user);
        cout << "User " << user->getNome() << " entrou na fila de espera para a máquina " << nome << endl;
    }
}

void Maquina::associarUser(User* user) {
    if(getUserAtual() ==nullptr){
        setUserAtual(user);
        int rodadas = user->getCarteira()/this->getAposta();
        user->setJogadas(rodadas);
        cout << "Jogador " << user->getNome() << " trocou " << user->getCarteira() << "E por " << user->getJogadas() << " fichas" << endl;
    }
}

void Maquina::rodadas(User* user){
    // Gerar um índice aleatório usando a operação de módulo
    float randomProb = rand() % 100;
    int dinheiroCasino = 0;
    int dinheiroDado = 0;
    dinheiroCasino += casino->getTotalCaixa();
    dinheiroDado += casino->getTotalDinheiroDado();
    if (randomProb <= getProb()) {
        cout << "Jogador " << user->getNome() << " ganhou na máquina " << nome << "  premio   "<< premio << endl;
        float ganhosUser = user->getGanhos() + premio;
        user->setGanhos(ganhosUser);
        dinheiroCasino -= premio;
        dinheiroDado += premio;
        subirProbabilidade();
    } else {
        cout << "Jogador " << user->getNome() << " perdeu na máquina " << nome << endl;
        dinheiroCasino += aposta;
    }
    setNJogos(nJogos + 1);
    casino->setTotalCaixa(dinheiroCasino);
    casino->setTotalDinheiroDado(dinheiroDado);
}

void Maquina::userSaiu() {
    setUserAtual(nullptr);
    cout << "Maquina " << getNome() << " ficou livre" << endl;
}

bool Maquina::repararMaquina(){
    bool reparado= false;
    if(getQuente() == false && getTemperaturaSensor() < 35.0){
        estado = ON;
        reparado = true;
    }else if(getQuente() == true ){
        reparado = false;
        cout << "Maquina esta a arrefecer - " << getTemperaturaSensor() << "º" << endl;
    }
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
            cout << "Máquina vizinha removida com ID: " << id_maq << endl;
            vizinhos.erase(it); // Remove a máquina da lista
            return true;
            cout << "Máquina vizinha removida." << endl;
        }
    }
    return false;
}

bool Maquina::removerVizinhoTodos(){
    cout << "Lista de vizinhos antes: " << vizinhos.size() << endl;
    // Limpa a lista de vizinhos
    vizinhos.clear();
    return true;
}

int Maquina::Memoria() {
    int mem = sizeof(*this);
    // Adicione a memória associada a membros dinâmicos, se houver
    // Exemplo considerando listas dinâmicas
    mem += sizeof(User*) * filaEspera.size(); // tamanho da lista de ponteiros
    mem += sizeof(Maquina*) * vizinhos.size(); // tamanho da lista de ponteiros
    return mem;
}

