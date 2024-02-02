#include "Maquina.h"
#include <stdlib.h>
#include <time.h>

#include "Casino.h"

Maquina::Maquina(int _idM, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino)
{//ctor
    // Inicializa as variaveis da classe com os valores passados como argumentos
    idMaquina = _idM;
    nome = _nome;
    x = _x;
    y = _y;
    premio = _premio;
    prob = _prob;
    tipo = _tipo;
    aposta = _aposta;
    casino = _casino;
    // Inicializa outras variaveis da classe
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
    vizinhos.clear();  // Limpar a lista de vizinhos
    filaEspera.clear();  // Limpar a lista de usuários na fila de espera
}

//Metodo de mostrar estado da maquina
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

//Metodo Run
void Maquina::Run(){
    //Printa informação
    cout << endl;
    cout << "Máquina: " << idMaquina << endl;
    cout << "Numero de vizinhos: " << vizinhos.size() << endl;
    verificaEstado(); //Verifica estado da maquina
    //verificar probabilidade e printa mensagem com a cor de fundo consoante o nivel de probabilidade
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

//Verificar estado
bool Maquina::verificaEstado()
{
    //Verifica o estado da máquina e printa mensagens com a cor respetiva ao estado
    if(estado==ON){
        cout << "\033[1;32mMaquina Ligada\033[0m\n";
    }else if(estado==OFF){
        cout << "\033[1;33mMaquina Desligada\033[0m\n";
    }else if(estado==AVARIADA){
        cout << "\033[1;31mMaquina Avariada\033[0m\n";
    }
    return true;
}

//Processo de desligar maquina
bool Maquina::Desligar(){
    bool desligado = false;
    if(estado == AVARIADA){ //Se estado AVARIADA
        cout << "Esta maquina encontra-se avariada!" << endl;
        cout << "Deseja reparar (S/N): " << endl; //printa se deseja repara alguam das maquinas que apareceu
        char repar; //variavel do tipo char para guardar resposta
        cin >> repar; //guarda resposta
        if(repar == 'S' || repar == 's'){ //verifica se escolheram reparar
            repararMaquina(); //chama função complementar para reparar maquina
            estado = OFF; // Altera o estado da máquina para OFF
            desligado = true;
            saemTodos(); //Retirar todos os jogadores da maquina
        }
        else{ //Se não pretender reparar a maquina
            cout << "Esta maquina não foi desligada, permanecerá avariada!" << endl;
            estado = AVARIADA; //Estado continua como AVARIADA
        }
    }else if(estado == ON){ //Se estado ON
        estado = OFF; // Altera o estado da máquina para OFF
        desligado = true;
        saemTodos(); //Retirar todos os jogadores da maquina
    }
    return desligado;
}

//Processo de retirar todos os jogadores associados a amquina
void Maquina::saemTodos(){
    if (getUserAtual()== nullptr){ //Se maquina livre
        if(filaEspera.size() > 0){ //Se existirem jogadores na fila de espera
            for (list<User *>::iterator it = filaEspera.begin(); it != filaEspera.end();it++) {
                (*it)->userSaiCasino(); //Tira jogadores do casino
            }
        }
    }else if(getUserAtual()!= nullptr){ //Se maquina ocupada
        User *user = getUserAtual();
        user->userSaiCasino(); //Remove jogador atual da maquina
        if(filaEspera.size() > 0){ //Se existirem jogadores na fila de espera
            for (list<User *>::iterator it = filaEspera.begin(); it != filaEspera.end();it++) {
                (*it)->userSaiCasino(); //Tira jogadores do casino
            }
        }
    }
}

//Processo de ligar maquina
void Maquina::Ligar() {
    if(estado == AVARIADA){ //Se maquina esta AVARIADA
        estado = AVARIADA;
    }else{ //Caso seja diferente de AVARIADA
        estado = ON; //Troca o estado para ON
    }
}

//Processo de avariar maquina
void Maquina::avariaMaquina(){
    estado = AVARIADA; //Troca estado para AVARIADA
    nAvarias ++; //Incrementa numero de avarias
}

//Processo de jogador entrar na fila de espera
void Maquina::entrarFilaEspera(User* user) {
    if (userAtual != nullptr) { //Se maquina ocupada
        filaEspera.push_back(user); //Adiciona jogador a fila de espera
        cout << "User " << user->getNome() << " entrou na fila de espera para a máquina " << nome << endl;
    }
}

//Porcesso de associar jogador a maquina
void Maquina::associarUser(User* user) {
    if(getUserAtual() ==nullptr){ //Se maquina livre
        setUserAtual(user); //Associa jogador
        //Calcula o número de jogadas que o jogador pode fazer com dinheiro da carteira
        int rodadas = user->getCarteira()/this->getAposta();
        user->setJogadas(rodadas);
        cout << "Jogador " << user->getNome() << " trocou " << user->getCarteira() << " Euros por " << user->getJogadas() << " fichas" << endl;
    }
}

//Processo de jogada na maquina
void Maquina::rodadas(User* user){
    float randomProb = rand() % 100; //Gera um numero random
    int dinheiroCasino = 0;
    int dinheiroDado = 0;
    dinheiroCasino += casino->getTotalCaixa();
    dinheiroDado += casino->getTotalDinheiroDado();
    if (randomProb <= getProb()) { //Se a probabilidade random for menor ou igual a probabilidade de ganhar da maquina
        cout << "Jogador " << user->getNome() << " ganhou na máquina " << nome << "  premio   "<< premio << endl;
        //Atualiza os ganhos do jogador
        float ganhosUser = user->getGanhos() + premio;
        user->setGanhos(ganhosUser);
        //Atualiza dinheiro do casino
        dinheiroCasino -= premio;
        dinheiroDado += premio;
        //Aumenta a probabilidade da máquina
        subirProbabilidade();
    } else {
        cout << "Jogador " << user->getNome() << " perdeu na máquina " << nome << endl;
        //Atualiza dinheiro do casino
        dinheiroCasino += aposta;
    }
    setNJogos(nJogos + 1); //Atualiza o número de jogos da máquina
    //Atualiza dinheiro do casino
    casino->setTotalCaixa(dinheiroCasino);
    casino->setTotalDinheiroDado(dinheiroDado);
}

//Processo de saida do jogador
void Maquina::userSaiu() {
    setUserAtual(nullptr); //Remove jogador da maquina
    cout << "Maquina " << getNome() << " ficou livre" << endl;
}

//Processo de raparação da maquina
bool Maquina::repararMaquina(){
    bool reparado= false;
    // Verifica se a máquina não está quente e a temperatura é inferior a 35.0
    if(getQuente() == false && getTemperaturaSensor() < 35.0){
        estado = ON; //Trocar estado para ON
        reparado = true;
    // Se a máquina estiver quente printa mensagem
    }else if(getQuente() == true ){
        reparado = false;
        cout << "Maquina esta a arrefecer - " << getTemperaturaSensor() << "º" << endl;
    }
    return reparado;
}

//Processo de adicionar vizinho
void Maquina::adicionarVizinho(Maquina* vizinho) {
    vizinhos.push_back(vizinho); //Adiciona a máquina à lista de vizinhos
}

//Processo para subir probabilidade das maquinas vizinhas
void Maquina::subirProbabilidade(){
    //Chama o método SubirProbabilidadeVizinhas do Casino
    //Para aumentar a probabilidade das máquinas vizinhas
    casino->SubirProbabilidadeVizinhas(this,2,vizinhos);
}

//Processo de remover vizinho
bool Maquina::removerVizinho(int id_maq){
    for (list<Maquina *>::iterator it = vizinhos.begin(); it != vizinhos.end();it++) { //Percorre lista de maquinas vizinhas
        if ((*it)->getID() == id_maq) { //Se id da maquina for igual ao id da maquina desejada
            cout << "Máquina vizinha removida com ID: " << id_maq << endl;
            vizinhos.erase(it); // Remove a máquina da lista
            return true;
            cout << "Máquina vizinha removida." << endl;
        }
    }
    return false;
}

//Porcesso de remover todos os vizinhos
bool Maquina::removerVizinhoTodos(){
    cout << "Lista de vizinhos antes: " << vizinhos.size() << endl;
    vizinhos.clear(); //Limpa a lista de vizinhos
    return true;
}

//Calcular memoria
int Maquina::Memoria() {
    int mem = sizeof(*this); //Tamanho atual do objeto Maquina
    mem += sizeof(User*) * filaEspera.size(); //Tamanho da lista de ponteiros User na fila de espera
    mem += sizeof(Maquina*) * vizinhos.size(); //Tamanho da lista de ponteiros Maquina vizinhas
    mem += sizeof(casino); //Tamanho do ponteiro para Casino
    mem += sizeof(userAtual); //Tamanho do ponteiro para User
    mem += nome.size() + tipo.size(); //Tamanho da string "nome" e "tipo"
    return mem; //Retorna memoria total
}



