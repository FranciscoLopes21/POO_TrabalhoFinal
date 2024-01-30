#include <iostream>
#include <sstream>
#include <fstream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iomanip>
#include <random>
#include <cmath>
#include <cctype>
#include <ctime>
#include <string>
#include <list>
////////////////////////
#include "Casino.h"
#include "pugixml.hpp"
#include "XMLReader.h"
#include "MBlackJack.h"
#include "MPoker.h"
#include "MRoleta.h"
#include "MSlot.h"
#include "XMLWriter.h"
#include <windows.h>

using namespace std;

Casino::Casino(string _nome)
{//ctor
    // Inicializa as variaveis da classe com os valores passados como argumentos
    nome = _nome;
}

Casino::~Casino()
{
    //dtor
    //destui Maquinas
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){
        delete (*it);
    }
    LM.clear();
    //destui User
    for (list<User *>::iterator it = LU.begin(); it != LU.end(); ++it){
        delete (*it);
    }
    LU.clear();
}

//Processo de carregar dados do casino através do XML
bool Casino::Load(const string& ficheiro) {
    int maxJogadores, probabilidadeUser, horaAbertura, minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho;
    //Chama XMLReader::LoadCasinoData para obter os dados do XML
    bool result = XMLReader::LoadCasinoData("XML_Projecto.xml", maxJogadores, probabilidadeUser, horaAbertura,minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho);
    if (result) { //Se a operação de leitura do XML for bem sucedida
        //Chama CarregarDados passando por parametros os dados obtidos
        CarregarDados(maxJogadores,probabilidadeUser,horaAbertura,minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho);
    } else {
        cout << "Falha ao carregar as configurações do Casino a partir do XML." << endl;
    }
    return true;
}

void Casino::CarregarDados(int _maxJogadores, int _probabilidadeUser, int _horaAbertura, int _minutosAbertura, int _segundosAbertura,
                            int _horaFecho, int _minutosFecho, int _segundosFecho)
{
    maxJogadores = _maxJogadores;
    probabilidadeUser = _probabilidadeUser;
    horaAbertura = _horaAbertura;
    minutosAbertura = _minutosAbertura;
    segundosAbertura = _segundosAbertura;
    horaFecho = _horaFecho;
    minutosFecho = _minutosFecho;
    segundosFecho = _segundosFecho;
    jogadoresNoCasino=0;
    totalCaixa=0;
    totalDinheiroDado=0;
}

//Processo de carregar dados da maquina através do XML
bool Casino::LoadMaquina(const string& ficheiro) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(ficheiro.c_str());
    //Verifica se houve algum erro ao carregar o XML
    if (!result) {
        cout << "Erro ao carregar o arquivo XML: " << result.description() << endl;
        return false;
    }
    //Obtém a lista de máquinas do node XML
    pugi::xml_node listaMaq = doc.child("DADOS").child("LISTA_MAQ");
    //Percorre o XML e obtem os node da maquina
    for (pugi::xml_node maquina = listaMaq.child("MAQUINA"); maquina; maquina = maquina.next_sibling("MAQUINA")){
        //Guarda os dados de cada nó de máquina
        int id = maquina.child("ID").text().as_int();
        string nome = maquina.child("NOME").text().get();
        int x = maquina.child("X").text().as_int();
        int y = maquina.child("Y").text().as_int();
        int premio = maquina.child("PREMIO").text().as_int();
        int probG = maquina.child("PROB_G").text().as_int();
        string tipo = maquina.child("TIPO").text().get();
        //Cria uma instância da máquina com base no tipo
        Maquina* m = nullptr;
        if (tipo == "slot" || tipo == "Slot") {
            m = new MSlot(id, nome, x, y, premio, probG, tipo,5,this);
        } else if (tipo == "roleta" || tipo == "Roleta") {
            m = new MRoleta(id, nome, x, y, premio, probG, tipo,10,this);
        }else if (tipo == "poker" || tipo == "Poker") {
            m = new MPoker(id, nome, x, y, premio, probG, tipo,15,this);
        }  else if (tipo == "blackjack" || tipo == "Blackjack") {
            m = new MBlackJack(id, nome, x, y, premio, probG, tipo,20,this);
        }
        //Adiciona a máquina à lista do casino
        Add(m);
    }
    return true;
}

//adicionar jogador/user
bool Casino::Add(User *ut){
    bool resultado = false;
    if (ut == nullptr) { //Se ponteiro do User fou nulo
        resultado = false;
    }else{
        //Adiciona o User à lista de users do casino
        LU.push_back(ut);
        resultado = true;
    }
    return resultado;
}

//Adicionar máquina
bool Casino::Add(Maquina *m){ //recebe por parametro um ponteiro para um objecto do tipo Maquina
    bool resultado = false;
    if (m == nullptr) { //Se ponteiro de Maquina for nulo
        resultado = false;
    }else{
        adicionarVizinho(m); //Função para adicionar vizinho
        LM.push_back(m); //Adicionar maquina a lisat de maquinas do casino
        resultado = true;
    }
    return resultado;
}

void Casino::Listar(ostream &f = std::cout){

    cout << "Listar estado atual do Casino" << endl; //printa ação da função
    cout << endl; //escreve nova linha
    cout << "| ID Maquina | Nome Maquina | Probabilidade | Estado | temperatura |" << endl; //printa campos que vão ser mostrados

    f << "Estado Casino " << nome << endl; //escreve no ficheiro
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        //escreve no ficheiro os campos - id, nome, probabilidade, estado, temperatura
        f << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: " << estadoString((*it)->getEstado()) << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        if((*it)->getEstado()== ON){ //verifica se o estado é ON para ajustar a cor do estado quando printa
            //printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;32m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== OFF){ //verifica se o estado é ON para ajustar a cor do estado quando printa
            //printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;33m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== AVARIADA){ //verifica se o estado é ON para ajustar a cor do estado quando printa
            //printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;31m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }
    }
    cout << endl;
    cout << "Numero de Maquinas: " << LM.size() << endl; //maximo de jogadores no casino
    cout << endl; //adiciona linha
    cout << "Jogadores no casino: " << jogadoresNoCasino << endl; //printa a quantidade de jogadores no casino atualmente
    cout << "Numero total de jogadores: " << LU.size() << endl; //printa a quantidade de jogadores que já passaram pelo casino
    cout << endl; //adiciona linha
    cout << "Dinheiro em caixa: " << totalCaixa << endl; //maximo de jogadores no casino
    cout << "Dinheiro dado: " << totalDinheiroDado << endl; //hora de abertura
}

void Casino::Desligar(int id_maq) { //recebe o id da maquina que pretendem desligar
    bool desligada = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if((*it)->getID() == id_maq){ //se id maquina for igual ao id da maquina que pretendem desligar
            (*it)->Desligar(); //chama função para desligar maquina
            desligada = true;
            cout << "Máquina " << (*it)->getNome() << " desligada." << endl;
        }
    }
    if(desligada == false){ //Verifica se a maquina desejada foi desligada
        cout << "Máquina não encontrada." << endl; //se não encontrar a maquina desejada mostra mensagem de "erro"
    }
}

//Desligar todas as maquinas
void Casino::desligarTodasMaquinas() {
    bool desligada = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //perocorre todas as maquinas
        desligada = (*it)->Desligar(); //maquina chama função Desligar()
        if(desligada == false){ //Verifica se a maquina desejada foi desligada
            cout << "Máquina " << (*it)->getID() << " não foi desligada." << endl; //se não encontrar a maquina desejada mostra mensagem de "erro"
        }
    }
}

//Saber o estado de uma dada Máquina dado o seu ID
estadoMaquina Casino::Get_Estado(int id_maq) { //recebe id da maquina desejada
    bool maquina = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if((*it)->getID() == id_maq){ //se id maquina for igual ao id da maquina que pretendem desligar
            maquina = true;
        }
        if(maquina){
            return (*it)->getEstado(); //returna estado da maquina
        }
    }
    if(!maquina){
        cout << "Máquina não encontrada." << endl; //se não encontrar a maquina desejada mostra mensagem de "erro"
        return estadoMaquina::OFF; //e retorna estado OFF
    }
    return estadoMaquina::OFF; //e retorna estado OFF
}

//Função complementar para listar maquinas de um dado tipo
void Casino::listarTipoMaquina(){
    string Tipo; //Varivel do tipo string para guardar o tipo de maquina desejada
    cout << "Tipo de maquina: "; //Pede o tipo de maquina pretendida
    cin >> Tipo; //Guarda tipo de maquina desejada
    ofstream F("MaquinasTipo.txt"); //Abrir fecheiro
    if (!F.is_open()) { //Se ficheiro não foi aberto corretamente
        cout << "Erro ao abrir o arquivo MaquinasTipo.txt" << endl;
    }else{
        list<Maquina*>* maquinasDoTipo = Listar_Tipo(Tipo, F); //Invoca função para listar maquinas de um dado tipo
        for (list<Maquina *>::iterator it = maquinasDoTipo->begin(); it != maquinasDoTipo->end(); it++) {
            if ((*it)->getTipo() == Tipo) {
                cout << "ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << endl;
            }
        }
        delete maquinasDoTipo; //Liberta memoria alocada pela lista
        F.close(); //Fechar ficheiro
    }
}

//listar maquinas de um dado tipo
list<Maquina *> * Casino::Listar_Tipo(string Tipo, std::ostream &f) {
    //Cria lista armazenar máquinas
    list<Maquina *> * maquinasDoTipo = new list<Maquina*>;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if ((*it)->getTipo() == Tipo) { //Verifica se a máquina atual tem o tipo desejado
            //Adiciona a máquina à lista
            maquinasDoTipo->push_back((*it));
            //Escreve informações sobre a máquina no ficheiro
            f << "ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << endl;
        }
    }
    return maquinasDoTipo;
}

list<string> * Casino::Ranking_Dos_Fracos(){
    // Lista para armazenar as informações das máquinas avariadas ordenadas por número de avarias
    list<string>* rankingAvariadas = new list<string>;
    // Criar uma cópia da lista principal
    list<Maquina*> maquinasCopy(LM.begin(), LM.end());
    // Ordenar a lista de máquinas por número de avarias (do maior para o menor)
    maquinasCopy.sort([](Maquina* a, Maquina* b) {
        return a->getnAvarias() > b->getnAvarias();
    });
    // Preencher a lista de strings com informações das máquinas ordenadas
    for (list<Maquina *>::iterator it = maquinasCopy.begin(); it != maquinasCopy.end(); ++it) {
        string info =   " |ID: " + to_string((*it)->getID()) +
                        " | Tipo: " + (*it)->getTipo() +
                        " | Nome: " + (*it)->getNome() +
                        " | Avarias: " + to_string((*it)->getnAvarias());
        rankingAvariadas->push_back(info);
    }
    return rankingAvariadas;
}

void Casino:: showRankingAvarias(){
    // Chamar a função para obter o ranking das máquinas avariadas
    list<string>* rankingAvariadas = Ranking_Dos_Fracos();
    // Verificar se a lista não está vazia antes de tentar acessar os elementos
    if (rankingAvariadas->size() <= 0) {
        cout << "Nenhuma máquina avariada encontrada." << endl;
    } else {
        // Percorrer e exibir as informações das máquinas no ranking
        for (list<string>::iterator it = rankingAvariadas->begin(); it != rankingAvariadas->end(); ++it) {
            cout << (*it) << endl;
        }
    }
    delete rankingAvariadas; //Liberta memoria alocada pela lista
}

//Ranking dos mais trabalhadores - as que são usadas
list<Maquina *> *Casino::Ranking_Das_Mais_Trabalhadores() {
    //Crie uma cópia da lista de máquinas
    list<Maquina *> * copiaMaquinas = new list<Maquina *>(LM.begin(), LM.end());
    //Ordene a lista usando a função de comparação
    copiaMaquinas->sort([](Maquina* a, Maquina* b) {
        return a->getNJogos() > b->getNJogos();
    });
    return copiaMaquinas;
}

void Casino::listarRankingMaisTrabalhadores(){
    list<Maquina*>* maquinasMaisTrabalhadores = Ranking_Das_Mais_Trabalhadores();
    for (list<Maquina *>::iterator it = maquinasMaisTrabalhadores->begin(); it != maquinasMaisTrabalhadores->end(); ++it) {
        cout << "ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Numero de jogos: " << (*it)->getNJogos() << endl;
    }
    delete maquinasMaisTrabalhadores; //Liberta memoria alocada pela lista
}

//jogadores mais frequentes - jogadores que mais tempo passaram a jogar
list<User *> * Casino::Jogadores_Mais_Frequentes (){
    // Crie uma cópia da lista de máquinas
    list<User *> *copiaUser = new list<User *>(LU.begin(), LU.end());
    copiaUser->sort([](User* a, User* b) {
        return a->getTempoCasino() > b->getTempoCasino();
    });
    return copiaUser;
}

void Casino::listarJogadoresMaisFrequentes(){
    list<User*>* copiaUser = Jogadores_Mais_Frequentes();
    for (list<User *>::iterator it = copiaUser->begin(); it != copiaUser->end(); ++it) {
        cout << "ID: " << (*it)->getNUser() << " | Nome: " << (*it)->getNome() << " | tempo casino: " << (*it)->getTempoCasino() << endl;
    }
    delete copiaUser; //Liberta memoria alocada pela lista
}

//jogadores que ganharam mais dinheiro
list<User *> * Casino::Jogadores_Mais_Ganhos () {
    // Crie uma cópia da lista de máquinas
    list<User *> *copiaUser = new list<User *>(LU.begin(), LU.end());
    copiaUser->sort([](User* a, User* b) {
        return a->getGanhos() > b->getGanhos();
    });
    return copiaUser;
}

void Casino::listarJogadoresMaisGanhos(){
    list<User*>* jogadoresMaisGanhos = Jogadores_Mais_Ganhos();
    for (list<User *>::iterator it = jogadoresMaisGanhos->begin(); it != jogadoresMaisGanhos->end(); ++it) {
        cout << "ID: " << (*it)->getNUser() << " | Nome: " << (*it)->getNome() << " | ganhos: " << (*it)->getGanhos() << endl;
    }
    delete jogadoresMaisGanhos; //Liberta memoria alocada pela lista
}

//Relatório casino - informação do estado atual de cada máquina
void Casino::Relatorio(string fich_xml) {
    cout << "Gerar Relatorio" << endl; //Printa cabeçalho da função
    cout<< "Nome ficheiro: " << fich_xml <<endl; //Printa nome do ficheiro
    XMLWriter XX; //Cria uma instância de XMLWriter para escrever no arquivo XML
    XX.WriteStartDocument(fich_xml); //Escreve no documanto XML com o nome passado por parametro
    XX.WriteStartElement("RELATORIO"); //Abre o elemento "Relatorio"
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre as maquinas do casino
        XX.WriteStartElement("Maquina"); //Abre o elemento “Maquina”
            XX.WriteElementString("ID",to_string((*it)->getID())); //Escreve no elemento "ID" com o id da maquina
            XX.WriteElementString("TIPO",(*it)->getTipo()); //Escreve no elemento "TIPO" com o tipo da maquina
            XX.WriteElementString("ESTADO",estadoString((*it)->getEstado())); //Escreve no elemento "ESTADO" com o estado da maquina
        XX.WriteEndElement(); //Fecha o Elemento “Maquina”
    }
    XX.WriteEndElement(); //Fecha o Elemento “RELATORIO”
}

//subir probabilidade das maquinas vizinhas da maquina que ganhou
void Casino::SubirProbabilidadeVizinhas(Maquina *M_ganhou, float R, list<Maquina *> &lmvizinhas) {
    // Lista para armazenar máquinas afetadas
    list<Maquina *> maquinasAfetadas;
    for (list<Maquina *>::iterator it = lmvizinhas.begin(); it != lmvizinhas.end(); ++it) {
        int distanciaY = abs((*it)->getY() - M_ganhou->getY());
        if (distanciaY <= R) {
            float probAt = (*it)->getProb();
            (*it)->setProb(probAt + 1.0);
            maquinasAfetadas.push_back((*it));
        }
    }
    mostrarMaquinasAfetadas(maquinasAfetadas);
}

void Casino::mostrarMaquinasAfetadas(list<Maquina *> &maquinasAfetadas) {
    cout << "Máquinas afetadas:" << endl;
    for (list<Maquina *>::iterator it = maquinasAfetadas.begin(); it != maquinasAfetadas.end(); ++it) {
        cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl;
    }
}

//Função complementar para lisatgem de maquina com probabilidade de ganhar superior a X
void Casino::ListarMaquinasProbabilidade() {
    float xProbabilidade; //variavel do tipo float para guardar probabilidade desejada
    cout<< "Listar maquinas com probabilidade superiora X" <<endl; //Printa cabeçalho da função
    cout << "Probabilidade: ";
    cin >> xProbabilidade; //guardar probabilidade
    ofstream F("ListaProbX.txt"); //Abrir ficheiro
    Listar(xProbabilidade, F); //Chama função Listar() que irá escrever no ficheiro as maquinas com probabilidade superior a X
    F.close(); //Fechar ficheiro
}

//Listar maquinas com probabilidade de ganhar maior que X
void Casino::Listar(float X, ostream &f) { //Recebe um float com a probabilidade desejada e uma referência a um objeto de fluxo de saída
    f << "Máquinas com probabilidade " << X << " maior que de ganhar:" << endl; //Escreve no ficheiro
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre todas as maquinas do casino
        if ((*it)->getProb() > X) { //Se a probabilidade maquina for superior a probabilidade desejada
            f << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl; //Escreve no ficheiro o id da maquina e a probabilidade da mesma
            cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl; //Printa id da maquina e a probabilidade da mesma
        }
    }
}

//Função Run Casino
void Casino::Run(){
    char key;
    bool maquinasJaLigadas = false;
    bool casinoEncerrado = false;
    while(true){
        cout<< "Menu" <<endl;
        time_t now = time(nullptr);
            tm* current_time = localtime(&now);

        int currentHour = current_time->tm_hour;//Hora atual
        int currentMinute = current_time->tm_min;//Minuto atual
        int currentSecond = current_time->tm_sec;//Segundo atual

        int openingTimeInSeconds = horaAbertura * 3600 + minutosAbertura * 60 + segundosAbertura;// Converte o horário de abertura para segundos
        int closingTimeInSeconds = horaFecho * 3600 + minutosFecho * 60 + segundosFecho;// Converte o horário de fechamento para segundos
        int currentTimeInSeconds = currentHour * 3600 + currentMinute * 60 + currentSecond;// Converte o horário atual para segundos

        if (currentTimeInSeconds >= openingTimeInSeconds && currentTimeInSeconds <= closingTimeInSeconds) {

                    // O casino está aberto!
                    cout << "O casino está aberto!" << endl;
                    casinoEncerrado = false;
                    if (!maquinasJaLigadas) { //Verifica se já tem todas as maquinas ligadas
                        ligarTodasMaquinas(); //Liga todas as maquinas
                        maquinasJaLigadas = true; //Guarda estado como já as ligou
                    }

                    // Todas as maquinas existentes passam pela função avariar e corre maquina
                    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){
                        avariar((*it)->getID()); //Função avariar
                        (*it)->Run(); //Corre maquina
                    }

                    // Verifica se o Casino está cheio  ou se podem entrar jogadores
                    if(jogadoresNoCasino < maxJogadores){
                            if(entrarJogador()){ //Verifica se entrou algum jogador
                                User *user = userEntraCasino("pessoas.txt"); //Guarda no apontador user o user retornado pela função userEntraCasino
                                if (Add(user)){ //Adiciona User
                                    cout << endl;
                                    cout << "Jogador entrou no Casino com sucesso!" << endl;
                                    jogadoresNoCasino ++; //Incrementa mais um jogador
                                    int randomIndex = rand() % LM.size(); //Vai buscar um numero random entre 0 e tamanho da lista
                                    auto it = next(LM.begin(), randomIndex); //o next faz com que se obtenha a posição correta da maquina
                                    //o auto it serve para ir buscar o tipo correto de dados que neste caso é máquina
                                    Maquina *maquina = *it; //guarda no apontador maquina o ponteiro it referente ao objeto amquina radnom
                                    if(maquina->getUserAtual()!=nullptr){ //Verifica se a maquina escolhida anteriormente está a ser usada
                                        user->entrarFilaEspera(maquina); //Se estiver o user entra na fila de espera
                                    }else{
                                        user->associarMaquina(maquina); //se não estiver o user associa-se a uma maquina
                                    }
                                    user->mostrarDados(); //Mostra dados do jogador
                                }else{
                                    cout << "Erro: Este jogador não entrou pois está a nulo." << endl; //mensagem de erro caso o usser seja invalido
                                }
                            }
                    }

                    //Se existirem mais do que 1 user percorre a lista de useres
                    if(LU.size()>=1){
                        for (list<User *>::iterator it = LU.begin(); it != LU.end(); ++it){
                                cout << endl;
                                (*it)->Run(); //correr user
                                cout << endl;
                        }
                    }

                    // Verifica se saiu algum user
                    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
                        if ((*it)->getUserAtual()==nullptr) { //verifica se a maqina atual não possui nenhum jogador
                            if (!(*it)->getFilaEspera().empty()) { //verifica se lista de espera da maquina não esta vazia
                                User *useruser = (*it)->getFilaEspera().front(); //Vai buscar o user a frente na lista de espera da maquina
                                useruser->associarMaquina(*it); //user associa-se a maquina
                                (*it)->removerUsersFilaEspera(useruser); //e é retirado da lista de espera

                                cout << "User: " <<  useruser->getNome() << " saiu da fila de espera e sentou-se na maquina" << endl;

                            }else{
                                (*it)->setUserAtual(nullptr); //se não houver ninguem na lisat de espera o useratual da maquinafica a null
                            }
                        }
                    }

            } else {
                cout << "O casino está fechado. Aguardando o horário de abertura." << endl;

                if (!casinoEncerrado){
                    string nome = devolveData();
                    Relatorio(nome);
                    casinoEncerrado = true;
                    if (maquinasJaLigadas) { //Se casino fechado então verifica se as amquina ainda estão ligadas
                        desligarTodasMaquinas(); //Desliga todas as maquina
                        maquinasJaLigadas = false; //Guarda estado como já desligou
                    }

                }

            }

            if (_kbhit()) { //Função de c++ que verifica se foi pressionada alguma tecla
                key = _getch(); //guarda tecka pressionada
                if (key == 'm' || key == 'M') { //verifica se é m ou M
                    Menu(); //abre Menu
                }
            }
    }

}


//Menu
void Casino::Menu(){
    int op = 0; //variavel que irá guardar a opção
    do {
        cout << "||||||||||||||||||||||" << endl; //printa menu
        cout << "|        Menu        |" << endl; //printa menu
        cout << "|   1- Dados Casino  |" << endl; //printa menu
        cout << "|  2- Gestao Casino  |" << endl; //printa menu
        cout << "| 3- Gestao Maquinas |" << endl; //printa menu
        cout << "|   4- Gestao User   |" << endl; //printa menu
        cout << "|  5- Memoria Total  |" << endl; //printa menu
        cout << "|       0- Sair      |" << endl; //printa menu
        cout << "||||||||||||||||||||||" << endl; //printa menu
        cout << endl; //adiciona linha
        cout << "Opcao: "; //pergunta por opção desejada
        cin >> op; //guarda opção desejada
        switch(op){ //verifica opção
            case 1: //caso seja 1
                system("cls"); //limpa ecra
                dadosCasino(); //chama função que exibe dados sobre o casino
                break; //quebra verificação
            case 2: //caso seja 2
                system("cls"); //limpa ecrã
                gestaoCasino(); //chama função gestaoCasino que irá exibir um menu para gerir casino
                break; //quebra verificação
            case 3: //caso seja 3
                system("cls"); //limpa ecrã
                gestaoMaquinas(); //chama função gestaoCasino que irá exibir um menu para gerir maquinas
                break; //quebra verificação
            case 4:
                system("cls"); //limpa ecrã
                gestaoUseres();
                break; //quebra verificação
            case 5:
                system("cls"); //limpa ecrã
                Memoria_Total(); //chama função gestaoCasino que irá exibir a memoria total usada pelo programa
                break; //quebra verificação
            case 0:
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    }
    while (op != 0);

}

//Mostrar dados do casino
void Casino::dadosCasino() {
    int slot = 0, roleta = 0, poker = 0, blackjack = 0;

    for (auto it = LM.begin(); it != LM.end(); ++it) { //percorre todas as maquinas do casino

        if ((*it)->getTipo() == "slot" || (*it)->getTipo() == "Slot") {
            slot ++;
        }else if((*it)->getTipo() == "roleta" || (*it)->getTipo() == "Roleta"){
            roleta ++;
        }else if((*it)->getTipo() == "poker" || (*it)->getTipo() == "Poker"){
            poker ++;
        }else if((*it)->getTipo() == "blackjack" || (*it)->getTipo() == "Blackjack"){
            blackjack ++;
        }
    }
    cout << "/////Dados Casino/////" << endl; //Cabeçalho dados casino
    cout << "Nome do Casino: " << nome << endl; //nome casino
    cout << "Máximo de Jogadores: " << maxJogadores << endl; //maximo de jogadores no casino
    cout << "Hora de Abertura: " << horaAbertura << ":" << minutosAbertura << ":" << segundosAbertura << endl; //hora de abertura
    cout << "Hora de Encerramento: " << horaFecho << ":" << minutosFecho << ":" << segundosFecho << endl; //hora de fecho
    cout << "Dinheiro em caixa: " << totalCaixa << endl; //maximo de jogadores no casino
    cout << "Dinheiro dado: " << totalDinheiroDado << endl; //hora de abertura
    cout << endl;
    cout << "/////Maquinas/////" << endl; //hora de fechocout << "Nome do Casino: " << nome << endl; //nome casino
    cout << "Numero de Maquinas: " << LM.size() << endl; //maximo de jogadores no casino
    cout << "Numero de Maquinas Slot: " << slot << endl; //maximo de jogadores no casino
    cout << "Numero de Maquinas Roleta: " << roleta << endl; //maximo de jogadores no casino
    cout << "Numero de Maquinas Poker: " << poker << endl; //maximo de jogadores no casino
    cout << "Numero de Maquinas BlackJack: " << blackjack << endl; //maximo de jogadores no casino
    cout << endl;
    cout << "/////Users/////" << endl; //hora de fechocout << "Nome do Casino: " << nome << endl; //nome casino
    cout << "Numero de jogadores que passaram no casino: " << LU.size() << endl; //maximo de jogadores no casino
    cout << "Numero de jogadores no casino: " << jogadoresNoCasino << endl; //hora de abertura
    cout << endl;
}

//Menu Gestão Casino
void Casino::gestaoCasino(){
    int op = 0;
    string nome;
    ofstream F("estadoAtualCasino.txt"); //Cria objeto ofstream para escrever no arquivo "estadoAtualCasino.txt"
    system("cls"); //limpa ecra
    do {
        cout << endl;
        cout << "Gestao Casino" << endl; //printa menu
        cout << "1- Listar estado atual casino" << endl; //printa menu
        cout << "2- Relatorio" << endl; //printa menu
        cout << "3- Listar maquinas com probabilidade superiora X" << endl; //printa menu
        cout << "0- Sair" << endl; //printa menu
        cout << endl;
        cout << "Opcao: ";
        cin >> op;
        switch(op){
            case 1:
                system("cls"); //Limpa ecra
                Listar(F); //Chama a função Listar passando por parameto o ficheiro
                F.close();
                break;
            case 2:
                system("cls"); //Limpa ecra
                nome = devolveData(); //guarda o nome com a data atual
                Relatorio(nome); //Chama a função Relatorio passando por parametro o nome do ficheiro com a data atual
                break;
            case 3:
                system("cls"); //Limpa ecra
                ListarMaquinasProbabilidade(); //Chama a função ListarMaquinasProbabilidade
                break;
            case 0:
                break;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    }while (op != 0);
}// Fim Menu Gestão Casino

//Menu Gestão Maquinas
void Casino::gestaoMaquinas(){
    int op = 0,idMaquina = 0;
    do {
        cout << endl;
        cout << "Gestão Maquinas" << endl; //printa menu
        cout << "1- Listar maquinas" << endl; //printa menu
        cout << "2- Crud maquina" << endl; //printa menu
        cout << "3- Desligar maquina" << endl; //printa menu
        cout << "4- Estado maquina ID" << endl; //printa menu
        cout << "5- Listar maquinas do tipo" << endl; //printa menu
        cout << "6- Ranking mais fracos" << endl; //printa menu
        cout << "7- Ranking mais trabalhadores" << endl; //printa menu
        cout << "8- Maquinas avariadas" << endl; //printa menu
        cout << "0- Sair" << endl; //printa menu
        cout << endl;
        cout << "Opção: ";
        cin >> op;
        switch(op){
            case 1:
                system("cls"); //Limpa ecra
                ListarMaquinas(); //Chama função ListarMaquinas()
                break;
            case 2:
                system("cls"); //Limpa ecra
                menuCrudMaquina(); //Chama função menuCrudMaquina()
                break;
            case 3:
                system("cls"); //Limpa ecra
                cout<< "ID: ";
                cin >> idMaquina;
                Desligar(idMaquina); //Chama função Desligar() passando por parametro o id da maquina
                break;
            case 4:
                system("cls"); //Limpa ecra
                cout<< "Estado maquina ID" <<endl;
                cout<< "ID: ";
                cin >> idMaquina;
                cout  << " | Estado: " << estadoString(Get_Estado(idMaquina)) << endl;
                break;
            case 5:
                system("cls"); //Limpa ecra
                listarTipoMaquina(); //Chama função listarTipoMaquina()
                break;
            case 6:
                system("cls"); //Limpa ecra
                showRankingAvarias(); //Chama função showRankingAvarias()
                break;
            case 7:
                system("cls"); //Limpa ecra
                listarRankingMaisTrabalhadores(); //Chama função listarRankingMaisTrabalhadores()
                break;
            case 8:
                system("cls"); //Limpa ecra
                maquinaAvariada(); //Chama função maquinaAvariada()
                break;
            case 0:
                break;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    }while (op != 0);
}// Fim Menu Gestão Maquinas

//Listar maquinas
void Casino::ListarMaquinas() {
    cout << "Listar maquinas" << endl;
    cout << endl;
    cout << "|ID: | Nome: | Tipo: | Posição: (x ,y ) | Prêmio | ProbG:  | Estado: " << endl; //printa cabeçalho da lista
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        (*it)->informacaoMaquina(); //Mostra dados da maquina
    }
}

//Função complementar mostra maquinas avariadas
void Casino::maquinaAvariada(){
    char opReparar; //variavel do tipo char para guardar resposta
    bool maquinasAvariadas;
    do{
        maquinasAvariadas = mostrarMaquinas();
        if(maquinasAvariadas == true){
            cout << "Deseja reparar alguma maquina (S/N): " << endl; //printa se deseja repara alguam das maquinas que apareceu
            cin >> opReparar; //guarda resposta
            if(opReparar == 'S' || opReparar == 's'){ //verifica se escolheram reparar
                int id_maq; //variavel do tipo int para guardar  numero da maquina
                cout << "Numero Maquina: " ; //pede o numero da maquina
                cin >> id_maq; //guarda numero da maquina que pretendem reparar
                reparar(id_maq); //chama função complementar para reparar maquina
            }
        }else{
            cout << "Não existem maquinas para reparação" << endl;
        }
    }while (maquinasAvariadas && (opReparar != 'N' && opReparar != 'n'));
}

//função comlementar para reparar maquina
void Casino::reparar(int id_maq){
    bool reparada = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if ((*it)->getID() == id_maq) { //se o id da maquina for igual ao id da maquina desejada
            if ((*it)->getEstado() == AVARIADA){ //se o estado da maquina for igual a AVARIADA
                reparada = (*it)->repararMaquina(); //invoca função para reparar maquina
                if(reparada){
                    cout << "ID: " << (*it)->getID() << " | Maquina Reparada" << endl; //printa mensagem positiva apos reparação
                }else if(!reparada){
                    cout << "ID: " << (*it)->getID() << " | Maquina não foi reparada" << endl; //printa mensagem positiva apos reparação
                }
            }
            else if ((*it)->getEstado() == ON || (*it)->getEstado() == OFF){ //se a maquina não se encontrar AVARIADA
                cout << "ID: " << (*it)->getID() << " | Maquina sem avarias" << endl; //printa mensagem com mensagem
            }
        }
    }
}

bool Casino::mostrarMaquinas(){
    bool maquinasAvariadas = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if ((*it)->getEstado() ==  AVARIADA) { //se o estado da maquina for igual a AVARIADA
            cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << " | Estado: " << (*it)->getEstado() << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl; //mostra a maquina
            maquinasAvariadas = true;
        }
    }
    return maquinasAvariadas;
}

//Menu Gestão User
void Casino::gestaoUseres(){
    int op = 0;
    do {
        cout << "Gestão Useres" << endl;
        cout << "1- Jogadores mais frequentes" << endl;
        cout << "2- Jogadores com mais ganhos" << endl;
        cout << "0- Sair" << endl;
        cout << endl;
        cout << "Opção: ";
        cin >> op;
        switch(op){
            case 1:
                system("cls");
                cout << "Jogadores mais frequentes" << endl;
                listarJogadoresMaisFrequentes();
                break;
            case 2:
                system("cls");
                cout << "Jogadores com mais ganhos" << endl;
                listarJogadoresMaisGanhos(); //chama função gestaoCasino que irá exibir os jogadores que tem mais ganhos no casino
                break;
            case 0:
                break;

            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    }while (op != 0);
}// Fim Menu Gestão User

//memoria toral do programa
int Casino::Memoria_Total() {
    int mem = 0;
    mem = sizeof(*this);
    mem += nome.size();
    mem += LM.size() * sizeof(Maquina*);
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        //mem += sizeof(*it);
        mem += (*it)->Memoria();
    }
    // Adicione a memória associada a membros dinâmicos, se houver
    mem += sizeof(User*) * LU.size(); // Tamanho da lista de ponteiros de usuários
    // Itera sobre os usuários e adiciona a memória associada a cada um
    for (list<User*>::iterator it = LU.begin(); it != LU.end(); ++it){
        mem += (*it)->Memoria();
    }
    cout << "Memoria total Casino: " << mem << endl;
    return mem;
}


//Menu Crud Maquinas
void Casino::menuCrudMaquina(){

    int op = 0; //varivel do tipo int para guardar a opção escolhida
    int id_maq; //variavel do tipo int para guarda id maquina

    do {
        //system("cls"); //limpa ecra
        cout << "Crud Maquina" << endl; //printa menu
        cout << "1- Adicionar Maquina" << endl; //printa menu
        cout << "2- Remover Maquina" << endl; //printa menu
        cout << "3- Editar Maquina" << endl; //printa menu
        cout << "4- Mover Maquina" << endl; //printa menu
        cout << "0- Sair" << endl; //printa menu

        cout << "opcao: "; //printa qual é a decisão que pretende selecionar
        cin >> op; //guarda opção desejada

        switch(op){ //verifica opção
            case 1: //caso seja 1
                cout<< "Adicionar Maquina" <<endl; //printa escolha
                addMaquina(); //chama função para adicionar maquina
                break; //quebra codição
            case 2: //caso seja 2
                cout<< "Remover Maquina" <<endl; //printa escolha
                cout <<"ID Maquina: "; //pede o id de maquina pretendida
                cin >> id_maq; //guarda id maquina desejada
                removerMaquina(id_maq); //chama função para remover maquina passando lhe o id da maquina por parametro
                break; //quebra codição
            case 3: //caso seja 3
                cout<< "Editar Maquina: Introduza id número maquina" <<endl; //printa escolha
                cout <<"ID Maquina: "; //pede o id de maquina pretendida
                cin >> id_maq; //guarda id maquina desejada
                editarMaquina(id_maq); //chama função para editar maquina passando lhe o id da maquina por parametro
                break; //quebra codição
            case 4: //caso seja 4
                cout<< "Mover Maquina" <<endl; //printa escolha
                cout <<"ID Maquina: "; //pede o id de maquina pretendida
                cin >> id_maq; //guarda id maquina desejada
                moverMaquina(id_maq); //chama função para mover maquina passando lhe o id da maquina por parametro
                break; //quebra codição
            case 0:
                break; //quebra codição
            default: //se nenhuma das opções
                cout << "Opção inválida. Tente novamente." << endl; //printa mensagem de erro
        }
    }while (op != 0); //repete processo enquanto opção diferente a 0
}// Fim Menu Crud Maquinas

// Crud Maquinas
void Casino::addMaquina(){
    int premio, x, y; //variveis do tipo int para guardar premio, x e y
    string nome, tipo; //variveis do tipo string para guardar nome, tipo da maquina
    float prob; //variveis do tipo float para guardar probabilidade da maquina
    bool existe; //variveis do tipo bool para guardar premio, x e y ////////////////////
    cout << "Nome máquina: "; //pede nome da maquina
    cin >> nome; //guarda nome da maquina
    do {
        cout << "X máquina: "; //pede x da maquina
        cin >> x; //guarda x da maquina
        cout << "Y máquina: ";
        cin >> y;
        existe = false;
        for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
            if ((*it)->getX() == x && (*it)->getY() == y) {
                existe = true;
                cout << "Posição já ocupada. Escolha outra posição." << endl;
            }
        }
        if(!existe){
            cout << "Premio máquina: ";
            cin >> premio;
            bool probMaior = false;
            do{
                cout << "Probabilidade máquina: ";
                cin >> prob;
                if(prob <= 100){
                    probMaior = true;
                }else{
                    cout << "Probabilidade tem de ser menor que 100" << endl;
                    probMaior = false;
                }
            }while (probMaior == false);
            cout << "Tipo máquina: ";
            cin >> tipo;
            Maquina* m = nullptr;
            int id = LM.back()->getID()+1;
            if (tipo == "slot" || tipo == "Slot") {
                m = new MSlot(id, nome, x, y, premio, prob, tipo,5, this);
            } else if (tipo == "roleta" || tipo == "Roleta") {
                m = new MRoleta(id, nome, x, y, premio, prob, tipo,10, this);
            } else if (tipo == "poker" || tipo == "Poker") {
                m = new MPoker(id, nome, x, y, premio, prob, tipo,15, this);
            } else if (tipo == "blackjack" || tipo == "Blackjack") {
                m = new MBlackJack(id, nome, x, y, premio, prob, tipo,20, this);
            }
            if(Add(m)){
                cout << "Maquina foi adicionada ao Casino com sucesso!" << endl;
                cout << endl;
            }else{
                cout << "Erro: Esta maquina nao foi adicionada pois esta a nulo." << endl;
                cout << endl;
            }
        }
    }while (existe == true);
}

//remover maquina
bool Casino::removerMaquina(int id_maq) {
    removerVizinho(id_maq); //chama função para remover vizinhos da maquina a remover passando o id da maquina por parametro
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //percorre todas as maquinas do casino
        if ((*it)->getID() == id_maq) { //se id da maquina atual for igual ao id passado por parametro
            //delete *it;  // Libera a memória alocada pela máquina /////////
            (*it)->saemTodos();
            removerVizinho((*it)->getID());
            LM.erase(it); // Remove a máquina da lista
            cout << "Máquina removida." << endl;
            return true;
        }
    }
    cout << "Máquina não encontrada." << endl;
    return false;
}

//editar maquina
bool Casino::editarMaquina(int id_maq){
    cout << " |Dados maquina|" << id_maq << endl;
    int op = 0;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        if ((*it)->getID() == id_maq) {
            do {
                cout << " |Nome Maquina: " << (*it)->getNome() <<"| " << endl;
                cout << " |X: " << (*it)->getX() <<"| " << endl;
                cout << " |Y: " << (*it)->getY() <<"| " << endl;
                cout << " |Probabilidade: " << (*it)->getProb() <<"| " << endl;
                cout << " |Premio: " << (*it)->getPremio() <<"| " << endl;
                cout << "Selecione o dado a alterar:" <<endl;
                cout << "1 - Nome" <<endl;
                cout << "2 - Probabilidade" <<endl;
                cout << "3 - Premio" <<endl;
                cout << "0 - Sair" <<endl;
                string nome;
                int premio;
                float prob;
                cin >> op;
                switch(op){
                    case 1:
                        cout << "Introduza o novo nome: " << endl;
                        cin >> nome;
                        (*it)->setNome(nome);
                        break;
                    case 2:
                        cout << "Introduza o nova probabilidade: " << endl;
                        cin >> prob;
                        (*it)->setProb(prob);
                        break;
                    case 3:
                        cout << "Introduza o novo premio: " << endl;
                        cin >> premio;
                        (*it)->setPremio(premio);
                        break;
                    case 0:
                        break;
                    default:
                        cout << "Opção inválida. Tente novamente." << endl;
                }
                system("cls");
            }while (op != 0);
        }
    }
    cout << "Máquina não encontrada." << endl;
    return false;
}

//mover maquina
bool Casino::moverMaquina(int id_maq){
    int x = 0, y = 0;
    bool movido = false;
    cout << "Mover Máquina de lugar" << endl;
    // Procurar a máquina com o ID fornecido
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        if ((*it)->getID() == id_maq) {
        cout << " |Nome Maquina: " << (*it)->getNome() << "| " << endl;
        cout << " |X: " << (*it)->getX() << "| " << endl;
        cout << " |Y: " << (*it)->getY() << "| " << endl;
        cout << " |Probabilidade: " << (*it)->getProb() << "| " << endl;
        cout << " |Premio: " << (*it)->getPremio() << "| " << endl;
        cout << endl;
            do {
                // Obter a nova posição
                cout << "Novo valor de X: ";
                cin >> x;
                cout << "Novo valor de Y: ";
                cin >> y;
                // Verificar se a nova posição já está ocupada
                bool posicaoOcupada = false;
                for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
                    if ((*it)->getX() == x && (*it)->getY() == y) {
                        posicaoOcupada = true;
                        cout << "Posição já ocupada. Escolha outra posição." << endl;
                    }
                }
                if (!posicaoOcupada) {
                    // Atualizar a posição da máquina
                    (*it)->setX(x);
                    (*it)->setY(y);
                    movido = true;
                    cout << "Máquina movida para (" << x << ", " << y << ")" << endl;
                    //Atualizar vizinhos
                    ////Remover vizinho
                    (*it)->removerVizinhoTodos();
                    removerVizinho((*it)->getID());
                    ////Adicionar nvo vizinho
                    adicionarVizinho((*it));
                }
            } while (!movido);
        }
    }
    return movido;
}


//Funções Complementares

string Casino::devolveData(){
    time_t currentTime = time(nullptr); //obter o tempo atual
    struct tm * ptm = localtime(&currentTime); //converter a representação do tempo local
    char buffer[32]; //criar um buffer para armazenar a data formatada
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ptm); //formatar a data no estilo "YYYY-MM-DD" e armazenar no buffer
    string currentDate(buffer); //converter o buffer para uma string
    string dataHoje = "data_" + currentDate + ".xml"; //criar o nome do arquivo XML com base na data
    return dataHoje; //retornar o nome do arquivo
}

void Casino::avariar(int IDMaq){
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){ //Percorre todas as maquinas
        Sleep(100);
        if((*it)->getID() == IDMaq){ //verifica se o ud da maquina atual é igual ao passado na função
            if((*it)->getEstado() == ON){ //Verifica se amaquina está ligada
                //rand() %100, vai buscar um numero entre 0 e 99 e adiciona mais 1 par air buscar o número correto
                int nRandom1 = rand() % 100 + 1; //gera um numero random
                int nRandom2 = rand() % 100 + 1; //gera um numero random
                //cout << "nRandom1: " << nRandom1 << endl;
                //cout << "nRandom2: " << nRandom2 << endl;
                if(nRandom1 == nRandom2){ //se os numero random forem iguais então avaria maquina
                    (*it)->avariaMaquina(); //Maquina chama função avariaMaquina()
                }
            }
        }
    }
}

//Ligar todas as maquinas
void Casino::ligarTodasMaquinas() {
   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //perocorre todas as maquinas
        (*it)->Ligar(); //maquina chama função Ligar()
    }
}

bool Casino::adicionarVizinho(Maquina *m) { //recebe por parametro um ponteiro para um objecto do tipo Maquina
    bool estado = false; //iguala o estado a falso
    int distanciaMaxima = 3;  //Defina a distância máxima desejada
    for (auto it = LM.begin(); it != LM.end(); ++it) { //percorre todas as maquinas do casino
        if ((*it)->getID() != m->getID() && (*it)->getX() == m->getX()) {
            /*
            verifica se o id da maquina que esta a ser verificada não é igual ao id da maquina que chega por parametro e
            verifica também se esta na mesma fila de x
            */
            int distanciaY = abs((*it)->getY() - m->getY()); //diferença entre y´s das maquinas
            if (distanciaY <= distanciaMaxima) {
                // Adicionar novaMaquina à lista de vizinhos de maquina
                (*it)->adicionarVizinho(m);
                // Adicionar maquina à lista de vizinhos de novaMaquina
                m->adicionarVizinho(*it);
                estado = true; //iguala estado a true
            }
        }
    }
    return estado; //returna o estado
}

//Traduzir estado para string
string Casino::estadoString(estadoMaquina estado){ //Recebe um valor do tipo estadoMaquina
    string estadoString; //Variavel que guarda o valor como string
    switch (estado) { //Verifica estado
        case ON:
            // Se o estado for ON, atribui a string "ON" à variável estadoString
            estadoString = "ON";
            break;
        case OFF:
            //Se o estado for OFF, atribui a string "OFF" à variável estadoString
            estadoString = "OFF";
            break;
        case AVARIADA:
            //Se o estado for AVARIADA, atribui a string "AVARIADA" à variável estadoString
            estadoString = "AVARIADA";
            break;
        default:
            //Se o estado não corresponder a nenhum dos casos anteriores, atribui a string "Unknown" à variável estadoString
            estadoString = "Unknown";
            break;
    }
    return estadoString;
}

//remover vizinhos
void Casino::removerVizinho(int id_maq) { //recebe por parametro o id da maquina da qual os vizinhos serão apagados
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //percorre todas as maquinas do casino
        (*it)->removerVizinho(id_maq); //chama função para remover vizinhos da maquina que será remvida
    }
    cout << "Eleminei tudo." << endl; //printa mensagem
}

//jogador entra no casino
User* Casino::userEntraCasino(const string &nomeArquivo){
    int numeroLinhas = ContarLinhas(nomeArquivo), id, idade;
    string linha, nome, sobrenome, morada;
    User* novoUser = nullptr;
    bool entrou = false;
    if (numeroLinhas > 0) {
        do{
            int linhaAleatoria = rand() % numeroLinhas + 1; //Gera um número aleatório dentro do intervalo de linhas do arquivo
            ifstream F(nomeArquivo); //Abra o arquivo
            if (!F.is_open()) {
                cerr << "Erro ao abrir o arquivo." << endl;
                return nullptr;
            }else{
                //Leitura das linhas até chegar à linha desejada
                for (int i = 0; i < linhaAleatoria; ++i) {
                    getline(F, linha);
                }
                stringstream ss(linha); //Use um stringstream para processar a linha
                //Leitura dos dados do usuário da linha
                ss >> id;
                ss.ignore(); //Ignorar o espaço em branco
                getline(ss, nome, '\t');
                getline(ss, morada, '\t');
                ss >> idade;
                if(idade >= 18){ //Se o User escolhido maior de idade
                    F.close(); //Feche o arquivo
                    // Crie um objeto User com base nos dados lidos
                    novoUser = new User(id, nome, morada, idade, this);
                    entrou = true;
                }else{
                    entrou = false;
                }
            }
        }while (!entrou);
    }
    return novoUser; //Returna user
}

//contar linhas de um ficheiro
int Casino::ContarLinhas(const string& nomeArquivo) {
    int numeroLinhas = 0;
    string linha;
    ifstream F(nomeArquivo); //Abre ficheiro fornecido
    if (!F.is_open()){ //Se ficheiro foi aberto com sucesso
        cout << "Erro ao abrir o ficheiro." << endl;
        return -1; // Indicativo de erro
    }else{
        while (getline(F, linha)){ //Loop para ler linhas do ficheiro
            numeroLinhas++;
        }
        //cout << "numero de linahs " << numeroLinhas << endl;
        F.close(); //Fecha ficheiro
    }
    return numeroLinhas;
}

//jogador entra no casino
bool Casino::entrarJogador(){
    bool entrada = false;
    srand (time(NULL)); //Inicia o gerador de numeros aleatorios com o tempo atual
    int nRandom1 = rand() % 100 + 1; //Gera numero aleatorio entre 1 e 100
    cout << "nRandom1: " << nRandom1 << endl;
    if(nRandom1 < 50){ //Se numero gerado menor que 50
        entrada = true;
    }
    return entrada;
}
