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
    //Inicializa a variavel nome da classe com o valor passado por parametro
    nome = _nome;
}

Casino::~Casino()
{//dtor
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
    int maxJogadores, horaAbertura, minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho;
    //Chama XMLReader::LoadCasinoData para obter os dados do XML
    bool result = XMLReader::LoadCasinoData("XML_Projecto.xml", maxJogadores, horaAbertura,minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho);
    if (result) { //Se a operação de leitura do XML for bem sucedida
        //Chama CarregarDados passando por parametros os dados obtidos
        CarregarDados(maxJogadores,horaAbertura,minutosAbertura, segundosAbertura, horaFecho, minutosFecho, segundosFecho);
    } else {
        cout << "Falha ao carregar as configurações do Casino a partir do XML." << endl;
    }
    return true;
}

//Segundo construtor para carregar restantes dados do casino
void Casino::CarregarDados(int _maxJogadores, int _horaAbertura, int _minutosAbertura, int _segundosAbertura,
                            int _horaFecho, int _minutosFecho, int _segundosFecho)
{
    maxJogadores = _maxJogadores;
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

//Adicionar jogador/user
bool Casino::Add(User *ut){ //Recebe por parametro um ponteiro para um objecto do tipo User
    bool resultado = false;
    if (ut == nullptr) { //Se o ponteiro do User for nulo
        resultado = false;
    }else{
        //Adiciona o User à lista de users do casino
        LU.push_back(ut);
        resultado = true;
    }
    return resultado;
}

//Adicionar máquina
bool Casino::Add(Maquina *m){ //Recebe por parametro um ponteiro para um objecto do tipo Maquina
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

//Listar estado atual do Casino
void Casino::Listar(ostream &f = std::cout){

    cout << "Listar estado atual do Casino" << endl; //Printa ação da função
    cout << endl; //escreve nova linha
    cout << "| ID Maquina | Nome Maquina | Probabilidade | Estado | temperatura |" << endl; //Printa campos que vão ser mostrados

    f << "Estado Casino " << nome << endl; //Escreve no ficheiro
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre todas as maquinas do casino
        //Escreve no ficheiro os campos - id, nome, probabilidade, estado, temperatura
        f << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: " << estadoString((*it)->getEstado()) << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        if((*it)->getEstado()== ON){ //Verifica se o estado é ON para ajustar a cor do estado quando printa
            //Printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;32m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== OFF){ //Verifica se o estado é ON para ajustar a cor do estado quando printa
            //Printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;33m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== AVARIADA){ //Verifica se o estado é ON para ajustar a cor do estado quando printa
            //Printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << " | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;31m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }
    }
    cout << endl;
    cout << "Numero de Maquinas: " << LM.size() << endl; //Maximo de jogadores no casino
    cout << endl; //Adiciona linha
    cout << "Jogadores no casino: " << jogadoresNoCasino << endl; //Printa a quantidade de jogadores no casino atualmente
    cout << "Numero total de jogadores: " << LU.size() << endl; //Printa a quantidade de jogadores que já passaram pelo casino
    cout << endl; //Adiciona linha
    cout << "Dinheiro em caixa: " << totalCaixa << endl; //Maximo de jogadores no casino
    cout << "Dinheiro dado: " << totalDinheiroDado << endl; //Hora de abertura
}

//Desligar maquina
void Casino::Desligar(int id_maq) { //Recebe o id da maquina que pretende desligar
    bool desligada = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre todas as maquinas do casino
        if((*it)->getID() == id_maq){ //Se id maquina for igual ao id da maquina que pretendem desligar
            (*it)->Desligar(); //Chama função para desligar maquina
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
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Perocorre todas as maquinas
        desligada = (*it)->Desligar(); //Maquina chama função Desligar()
        if(desligada == false){ //Verifica se a maquina desejada foi desligada
            cout << "Máquina " << (*it)->getID() << " não foi desligada." << endl; //Se não encontrar a maquina desejada mostra mensagem de "erro"
        }
    }
}

//Saber o estado de uma dada Máquina dado o seu ID
estadoMaquina Casino::Get_Estado(int id_maq) { //Recebe id da maquina desejada
    bool maquina = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre todas as maquinas do casino
        if((*it)->getID() == id_maq){ //Se id maquina for igual ao id da maquina que pretendem desligar
            maquina = true;
        }
        if(maquina){
            return (*it)->getEstado(); //Returna estado da maquina
        }
    }
    if(!maquina){
        cout << "Máquina não encontrada." << endl; //Se não encontrar a maquina desejada mostra mensagem de "erro"
        return estadoMaquina::OFF; //Retorna estado OFF
    }
    return estadoMaquina::OFF; //Retorna estado OFF
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
    //Lista para armazenar as informações das máquinas avariadas ordenadas por número de avarias
    list<string>* rankingAvariadas = new list<string>;
    //Criar uma cópia da lista principal
    list<Maquina*> maquinasCopy(LM.begin(), LM.end());
    //Ordenar a lista de máquinas por número de avarias (do maior para o menor)
    maquinasCopy.sort([](Maquina* a, Maquina* b) {
        return a->getnAvarias() > b->getnAvarias();
    });
    //Preenche a lista de strings com informações das máquinas ordenadas
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
    //Chama a função para obter o ranking das máquinas avariadas
    list<string>* rankingAvariadas = Ranking_Dos_Fracos();
    if (rankingAvariadas->size() <= 0) { //Verificar se a lista não está vazia antes de tentar acessar os elementos
        cout << "Nenhuma máquina avariada encontrada." << endl;
    } else {
        //Percorre a lista e exibe as informações das máquinas no ranking
        for (list<string>::iterator it = rankingAvariadas->begin(); it != rankingAvariadas->end(); ++it) {
            cout << (*it) << endl;
        }
    }
    delete rankingAvariadas; //Liberta memoria alocada pela lista
}

//Ranking dos mais trabalhadores - as que são mais usadas
list<Maquina *> *Casino::Ranking_Das_Mais_Trabalhadores() {
    //Crie uma cópia da lista de máquinas
    list<Maquina *> * copiaMaquinas = new list<Maquina *>(LM.begin(), LM.end());
    //Ordene a lista usando a função de comparação
    copiaMaquinas->sort([](Maquina* a, Maquina* b) {
        return a->getNJogos() > b->getNJogos();
    });
    return copiaMaquinas;
}
 //Função complementar para mostrar ranking das mais trabalhadoras
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

//Função complementar para mostrar jogadores masi frequentes
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

//Função complementar para jogadores com mais ganhos
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

//Função complementar para mostrar maquinas afetadas pela subida de probabilidade
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
    if (!F.is_open()) { //Se ficheiro não foi aberto corretamente
        cout << "Erro ao abrir o arquivo MaquinasTipo.txt" << endl;
    }else{
        Listar(xProbabilidade, F); //Chama função Listar() que irá escrever no ficheiro as maquinas com probabilidade superior a X
        F.close(); //Fechar ficheiro
    }

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
        int currentHour = current_time->tm_hour; //Hora atual
        int currentMinute = current_time->tm_min; //Minuto atual
        int currentSecond = current_time->tm_sec; //Segundo atual
        int openingTimeInSeconds = horaAbertura * 3600 + minutosAbertura * 60 + segundosAbertura; //Converte o horário de abertura para segundos
        int closingTimeInSeconds = horaFecho * 3600 + minutosFecho * 60 + segundosFecho; //Converte o horário de fechamento para segundos
        int currentTimeInSeconds = currentHour * 3600 + currentMinute * 60 + currentSecond; //Converte o horário atual para segundos

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
    int op = 0; //Variavel que irá guardar a opção
    do {
        cout << "||||||||||||||||||||||" << endl; //Printa menu
        cout << "|        Menu        |" << endl; //Printa menu
        cout << "|   1- Dados Casino  |" << endl; //Printa menu
        cout << "|  2- Gestao Casino  |" << endl; //Printa menu
        cout << "| 3- Gestao Maquinas |" << endl; //Printa menu
        cout << "|   4- Gestao User   |" << endl; //Printa menu
        cout << "|  5- Memoria Total  |" << endl; //Printa menu
        cout << "|       0- Sair      |" << endl; //Printa menu
        cout << "||||||||||||||||||||||" << endl; //Printa menu
        cout << endl; //Adiciona linha
        cout << "Opcao: "; //Pergunta por opção desejada
        cin >> op; //Guarda opção desejada
        switch(op){ //Verifica opção
            case 1: //Caso seja 1
                system("cls"); //Limpa ecra
                dadosCasino(); //Chama função que exibe dados sobre o casino
                break; //Quebra verificação
            case 2: //Caso seja 2
                system("cls"); //Limpa ecrã
                gestaoCasino(); //Chama função gestaoCasino que irá exibir um menu para gerir casino
                break; //Quebra verificação
            case 3: //Caso seja 3
                system("cls"); //Limpa ecrã
                gestaoMaquinas(); //Chama função gestaoCasino que irá exibir um menu para gerir maquinas
                break; //Quebra verificação
            case 4:
                system("cls"); //Limpa ecrã
                gestaoUseres();
                break; //Quebra verificação
            case 5:
                system("cls"); //Limpa ecrã
                Memoria_Total(); //Chama função gestaoCasino que irá exibir a memoria total usada pelo programa
                break; //Quebra verificação
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

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //Percorre todas as maquinas do casino

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
    cout << "Nome do Casino: " << nome << endl; //Nome casino
    cout << "Máximo de Jogadores: " << maxJogadores << endl; //Maximo de jogadores no casino
    cout << "Hora de Abertura: " << horaAbertura << ":" << minutosAbertura << ":" << segundosAbertura << endl; //Hora de abertura
    cout << "Hora de Encerramento: " << horaFecho << ":" << minutosFecho << ":" << segundosFecho << endl; //Hora de fecho
    cout << "Dinheiro em caixa: " << totalCaixa << endl; //Maximo de jogadores no casino
    cout << "Dinheiro dado: " << totalDinheiroDado << endl; //Hora de abertura
    cout << endl;
    cout << "/////Maquinas/////" << endl; //Cabeçalho dados Maquina
    cout << "Numero de Maquinas: " << LM.size() << endl; //Numero total de maquinas
    cout << "Numero de Maquinas Slot: " << slot << endl; //Numero de maquinas do tipo Slot
    cout << "Numero de Maquinas Roleta: " << roleta << endl; //Numero de maquinas do tipo Roleta
    cout << "Numero de Maquinas Poker: " << poker << endl; //Numero de maquinas do tipo Poker
    cout << "Numero de Maquinas BlackJack: " << blackjack << endl; //Numero de maquinas do tipo Blackjack
    cout << endl;
    cout << "/////Users/////" << endl; //Cabeçalho dados Users
    cout << "Numero de jogadores que passaram no casino: " << LU.size() << endl; //Numero de jogadores que ja passaram no casino
    cout << "Numero de jogadores no casino: " << jogadoresNoCasino << endl; //Numero de jogadoresque estão atualmente no casino
    cout << endl;
}

//Menu Gestão Casino
void Casino::gestaoCasino(){
    int op = 0;
    string nome;
    ofstream F("estadoAtualCasino.txt"); //Cria objeto ofstream para escrever no arquivo "estadoAtualCasino.txt"
    system("cls"); //Limpa ecra
    do {
        cout << endl;
        cout << "Gestao Casino" << endl; //Printa menu
        cout << "1- Listar estado atual casino" << endl; //Printa menu
        cout << "2- Relatorio" << endl; //printa menu
        cout << "3- Listar maquinas com probabilidade superiora X" << endl; //Printa menu
        cout << "0- Sair" << endl; //Printa menu
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
                nome = devolveData(); //Guarda o nome com a data atual
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
        cout << "Gestão Maquinas" << endl; //Printa menu
        cout << "1- Listar maquinas" << endl; //Printa menu
        cout << "2- Crud maquina" << endl; //Printa menu
        cout << "3- Desligar maquina" << endl; //Printa menu
        cout << "4- Estado maquina ID" << endl; //Printa menu
        cout << "5- Listar maquinas do tipo" << endl; //Printa menu
        cout << "6- Ranking mais fracos" << endl; //Printa menu
        cout << "7- Ranking mais trabalhadores" << endl; //Printa menu
        cout << "8- Maquinas avariadas" << endl; //Printa menu
        cout << "0- Sair" << endl; //Printa menu
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
    cout << "|ID: | Nome: | Tipo: | Posição: (x ,y ) | Prêmio | ProbG:  | Estado: " << endl; //Printa cabeçalho da lista
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre todas as maquinas do casino
        (*it)->informacaoMaquina(); //Mostra dados da maquina
    }
}

//Função complementar mostra maquinas avariadas
void Casino::maquinaAvariada(){
    char opReparar; //Variavel do tipo char para guardar resposta
    bool maquinasAvariadas;
    do{
        maquinasAvariadas = mostrarMaquinas();
        if(maquinasAvariadas == true){
            cout << "Deseja reparar alguma maquina (S/N): " << endl; //Printa se deseja repara alguam das maquinas que apareceu
            cin >> opReparar; //Guarda resposta
            if(opReparar == 'S' || opReparar == 's'){ //Verifica se escolheram reparar
                int id_maq; //Variavel do tipo int para guardar  numero da maquina
                cout << "Numero Maquina: " ; //Pede o numero da maquina
                cin >> id_maq; //Guarda numero da maquina que pretendem reparar
                reparar(id_maq); //Chama função complementar para reparar maquina
            }
        }else{
            cout << "Não existem maquinas para reparação" << endl;
        }
    }while (maquinasAvariadas && (opReparar != 'N' && opReparar != 'n'));
}

//Função comlementar para reparar maquina
void Casino::reparar(int id_maq){
    bool reparada = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre todas as maquinas do casino
        if ((*it)->getID() == id_maq) { //Se o id da maquina for igual ao id da maquina desejada
            if ((*it)->getEstado() == AVARIADA){ //Se o estado da maquina for igual a AVARIADA
                reparada = (*it)->repararMaquina(); //Invoca função para reparar maquina
                if(reparada){
                    cout << "ID: " << (*it)->getID() << " | Maquina Reparada" << endl; //Printa mensagem positiva apos reparação
                }else if(!reparada){
                    cout << "ID: " << (*it)->getID() << " | Maquina não foi reparada" << endl; //Printa mensagem positiva apos reparação
                }
            }
            else if ((*it)->getEstado() == ON || (*it)->getEstado() == OFF){ //Se a maquina não se encontrar AVARIADA
                cout << "ID: " << (*it)->getID() << " | Maquina sem avarias" << endl; //Printa mensagem com mensagem
            }
        }
    }
}

//Função complementar para mostrar todas as maquinas AVARIADAS
bool Casino::mostrarMaquinas(){
    bool maquinasAvariadas = false;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Percorre todas as maquinas do casino
        if ((*it)->getEstado() ==  AVARIADA) { //Se o estado da maquina for igual a AVARIADA
            cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << " | Estado: " << (*it)->getEstado() << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl; //Mostra a maquina
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
                listarJogadoresMaisFrequentes(); //Chama função listarJogadoresMaisFrequentes que irá exibir os jogadores que mais tempo passaram no casino a jogar
                break;
            case 2:
                system("cls");
                cout << "Jogadores com mais ganhos" << endl;
                listarJogadoresMaisGanhos(); //chama função listarJogadoresMaisGanhos que irá exibir os jogadores que mais dinheiro ganharam no casino
                break;
            case 0:
                break;

            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    }while (op != 0);
}// Fim Menu Gestão User

//Memoria toral do programa
int Casino::Memoria_Total() {
    int mem = 0;
    mem = sizeof(*this);
    mem += nome.size();
    mem += LM.size() * sizeof(Maquina*);
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        mem += (*it)->Memoria();
    }
    //Adicione a memória associada a membros dinâmicos, se houver
    mem += sizeof(User*) * LU.size(); // Tamanho da lista de ponteiros de usuários
    //Itera sobre os usuários e adiciona a memória associada a cada um
    for (list<User*>::iterator it = LU.begin(); it != LU.end(); ++it){
        mem += (*it)->Memoria();
    }
    cout << "Memoria total Casino: " << mem << endl;
    return mem;
}


//Menu Crud Maquinas
void Casino::menuCrudMaquina(){
    int op = 0; //Varivel do tipo int para guardar a opção escolhida
    int id_maq; //Variavel do tipo int para guarda id maquina
    do {
        cout << "Crud Maquina" << endl; //Printa menu
        cout << "1- Adicionar Maquina" << endl; //Printa menu
        cout << "2- Remover Maquina" << endl; //Printa menu
        cout << "3- Editar Maquina" << endl; //Printa menu
        cout << "4- Mover Maquina" << endl; //Printa menu
        cout << "0- Sair" << endl; //Printa menu

        cout << "opcao: "; //Printa qual é a decisão que pretende selecionar
        cin >> op; //Guarda opção desejada

        switch(op){ //Verifica opção
            case 1: //Caso seja 1
                cout<< "Adicionar Maquina" <<endl; //Printa escolha
                addMaquina(); //Chama função para adicionar maquina
                break; //Quebra codição
            case 2: //Caso seja 2
                cout<< "Remover Maquina" <<endl; //Printa escolha
                cout <<"ID Maquina: "; //Pede o id de maquina pretendida
                cin >> id_maq; //Guarda id maquina desejada
                removerMaquina(id_maq); //Chama função para remover maquina passando lhe o id da maquina por parametro
                break; //Quebra codição
            case 3: //Caso seja 3
                cout<< "Editar Maquina: Introduza id número maquina" <<endl; //Printa escolha
                cout <<"ID Maquina: "; //Pede o id de maquina pretendida
                cin >> id_maq; //Guarda id maquina desejada
                editarMaquina(id_maq); //Chama função para editar maquina passando lhe o id da maquina por parametro
                break; //Quebra codição
            case 4: //Caso seja 4
                cout<< "Mover Maquina" <<endl; //Printa escolha
                cout <<"ID Maquina: "; //Pede o id de maquina pretendida
                cin >> id_maq; //Guarda id maquina desejada
                moverMaquina(id_maq); //Chama função para mover maquina passando lhe o id da maquina por parametro
                break; //Quebra codição
            case 0:
                break; //Quebra codição
            default: //Se nenhuma das opções
                cout << "Opção inválida. Tente novamente." << endl; //Printa mensagem de erro
        }
    }while (op != 0); //Repete processo enquanto opção diferente a 0
}// Fim Menu Crud Maquinas

// Crud Maquinas
void Casino::addMaquina(){
    int premio, x, y; //Variveis do tipo int para guardar premio, x e y
    string nome, tipo; //Variveis do tipo string para guardar nome, tipo da maquina
    float prob; //Variveis do tipo float para guardar probabilidade da maquina
    bool existe; //Variveis do tipo bool para guardar premio, x e y ////////////////////
    cout << "Nome máquina: "; //Pede nome da maquina
    cin >> nome; //Guarda nome da maquina
    do {
        cout << "X máquina: "; //Pede x da maquina
        cin >> x; //Guarda x da maquina
        cout << "Y máquina: ";
        cin >> y;
        existe = false;
        for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //Percorre lista de maquinas
            if ((*it)->getX() == x && (*it)->getY() == y) { //Verifica se a posição desejada esta ocupada
                existe = true;
                cout << "Posição já ocupada. Escolha outra posição." << endl;
            }
        }
        if(!existe){ //Se não estiver ocupada
            cout << "Premio máquina: ";
            cin >> premio; //Guarda premio
            bool probMaior = false;
            do{
                cout << "Probabilidade máquina: ";
                cin >> prob;
                if(prob <= 100){ //Se a probabilidade for menor ao igual a 100
                    probMaior = true; //Probabilidade OK
                }else{ //Caso seja maior
                    cout << "Probabilidade tem de ser menor que 100" << endl;
                    probMaior = false; //Probabilidade NOTOK
                }
            }while (probMaior == false); //Equanto probabilidade NOTOK
            cout << "Tipo máquina: ";
            cin >> tipo; //Guarda tipo
            Maquina* m = nullptr;
            int id = LM.back()->getID()+1;
            //Processo de criar maquina consoante do seu tipo
            if (tipo == "slot" || tipo == "Slot") {
                m = new MSlot(id, nome, x, y, premio, prob, tipo,5, this);
            } else if (tipo == "roleta" || tipo == "Roleta") {
                m = new MRoleta(id, nome, x, y, premio, prob, tipo,10, this);
            } else if (tipo == "poker" || tipo == "Poker") {
                m = new MPoker(id, nome, x, y, premio, prob, tipo,15, this);
            } else if (tipo == "blackjack" || tipo == "Blackjack") {
                m = new MBlackJack(id, nome, x, y, premio, prob, tipo,20, this);
            }
            if(Add(m)){ //Se maquina foi adicionada corretamente
                cout << "Maquina foi adicionada ao Casino com sucesso!" << endl;
                cout << endl;
            }else{
                cout << "Erro: Esta maquina nao foi adicionada pois esta a nulo." << endl;
                cout << endl;
            }
        }
    }while (existe == true);
}

//Remover maquina
bool Casino::removerMaquina(int id_maq) {
    removerVizinho(id_maq); //Chama função para remover vizinhos da maquina a remover passando o id da maquina por parametro
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //Percorre todas as maquinas do casino
        if ((*it)->getID() == id_maq) { //Se id da maquina atual for igual ao id passado por parametro
            //Processo de saida e remoção de vizinhos
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

//Editar maquina
bool Casino::editarMaquina(int id_maq){
    cout << " |Dados maquina|" << id_maq << endl;
    int op = 0;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //Percorre lista de maquinas
        if ((*it)->getID() == id_maq) {
            do {
                cout << " |Nome Maquina: " << (*it)->getNome() <<"| " << endl; //Printa dados
                cout << " |X: " << (*it)->getX() <<"| " << endl; //Printa dados
                cout << " |Y: " << (*it)->getY() <<"| " << endl; //Printa dados
                cout << " |Probabilidade: " << (*it)->getProb() <<"| " << endl; //Printa dados
                cout << " |Premio: " << (*it)->getPremio() <<"| " << endl; //Printa dados
                cout << "Selecione o dado a alterar:" <<endl; //Printa menu
                cout << "1 - Nome" <<endl; //Printa menu
                cout << "2 - Probabilidade" <<endl; //Printa menu
                cout << "3 - Premio" <<endl; //Printa menu
                cout << "0 - Sair" <<endl; //Printa menu
                string nome;
                int premio;
                float prob;
                cin >> op; //Guarda opção
                switch(op){
                    case 1:
                        cout << "Introduza o novo nome: " << endl;
                        cin >> nome;
                        (*it)->setNome(nome); //Altera nome da maquina
                        break;
                    case 2:
                        cout << "Introduza o nova probabilidade: " << endl;
                        cin >> prob;
                        (*it)->setProb(prob); //Altera probabilidade
                        break;
                    case 3:
                        cout << "Introduza o novo premio: " << endl;
                        cin >> premio;
                        (*it)->setPremio(premio); //Altera Premio
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

//Mover maquina
bool Casino::moverMaquina(int id_maq){
    int x = 0, y = 0;
    bool movido = false;
    cout << "Mover Máquina de lugar" << endl;
    // Procurar a máquina com o ID fornecido
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //Percorre lista de maquinas do casino
        if ((*it)->getID() == id_maq) {
        cout << " |Nome Maquina: " << (*it)->getNome() << "| " << endl; //Printa dados
        cout << " |X: " << (*it)->getX() << "| " << endl; //Printa dados
        cout << " |Y: " << (*it)->getY() << "| " << endl; //Printa dados
        cout << " |Probabilidade: " << (*it)->getProb() << "| " << endl; //Printa dados
        cout << " |Premio: " << (*it)->getPremio() << "| " << endl; //Printa dados
        cout << endl;
            do {
                //Obter a nova posição
                cout << "Novo valor de X: ";
                cin >> x;
                cout << "Novo valor de Y: ";
                cin >> y;
                //Verificar se a nova posição já está ocupada
                bool posicaoOcupada = false;
                for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
                    if ((*it)->getX() == x && (*it)->getY() == y) {
                        posicaoOcupada = true;
                        cout << "Posição já ocupada. Escolha outra posição." << endl;
                    }
                }
                if (!posicaoOcupada) {
                    //Atualizar a posição da máquina
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
    time_t currentTime = time(nullptr); //Obter o tempo atual
    struct tm * ptm = localtime(&currentTime); //Converter a representação do tempo local
    char buffer[32]; //Criar um buffer para armazenar a data formatada
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ptm); //Formatar a data no estilo "YYYY-MM-DD" e armazenar no buffer
    string currentDate(buffer); //Converter o buffer para uma string
    string dataHoje = "data_" + currentDate + ".xml"; //Criar o nome do arquivo XML com base na data
    return dataHoje; //Retornar o nome do arquivo
}

void Casino::avariar(int IDMaq){
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){ //Percorre todas as maquinas
        Sleep(100);
        if((*it)->getID() == IDMaq){ //Verifica se o ud da maquina atual é igual ao passado na função
            if((*it)->getEstado() == ON){ //Verifica se amaquina está ligada
                //rand() %100, vai buscar um numero entre 0 e 99 e adiciona mais 1 par air buscar o número correto
                //cout << "nRandom1: " << nRandom1 << endl;
                //cout << "nRandom2: " << nRandom2 << endl;
                int nRandom1 = rand() % 100 + 1; //Gera um numero random
                int nRandom2 = rand() % 100 + 1; //Gera um numero random
                if(nRandom1 == nRandom2){ //Se os numero random forem iguais então avaria maquina
                    (*it)->avariaMaquina(); //Maquina chama função avariaMaquina()
                }
            }
        }
    }
}

//Ligar todas as maquinas
void Casino::ligarTodasMaquinas() {
   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //Perocorre todas as maquinas
        (*it)->Ligar(); //Maquina chama função Ligar()
    }
}

//Função complementar para adicionar vizinhos
bool Casino::adicionarVizinho(Maquina *m) { //Recebe por parametro um ponteiro para um objecto do tipo Maquina
    bool estado = false; //Iguala o estado a falso
    int distanciaMaxima = 3;  //Defina a distância máxima desejada
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //Percorre todas as maquinas do casino
        if ((*it)->getID() != m->getID() && (*it)->getX() == m->getX()) {
            /*
            Verifica se o id da maquina que esta a ser verificada não é igual ao id da maquina que chega por parametro e
            Verifica também se esta na mesma fila de x
            */
            int distanciaY = abs((*it)->getY() - m->getY()); //Diferença entre y´s das maquinas
            if (distanciaY <= distanciaMaxima) {
                //Adicionar novaMaquina à lista de vizinhos de maquina
                (*it)->adicionarVizinho(m);
                //Adicionar maquina à lista de vizinhos de novaMaquina
                m->adicionarVizinho(*it);
                estado = true; //Iguala estado a true
            }
        }
    }
    return estado; //Returna o estado
}

//Traduzir estado para string
string Casino::estadoString(estadoMaquina estado){ //Recebe um valor do tipo estadoMaquina
    string estadoString; //Variavel que guarda o valor como string
    switch (estado) { //Verifica estado
        case ON:
            //Se o estado for ON, atribui a string "ON" à variável estadoString
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

//Remover vizinhos
void Casino::removerVizinho(int id_maq) { //Recebe por parametro o id da maquina da qual os vizinhos serão apagados
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //Percorre todas as maquinas do casino
        (*it)->removerVizinho(id_maq); //Chama função para remover vizinhos da maquina que será remvida
    }
    cout << "Eleminei tudo." << endl; //Printa mensagem
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
                    //Crie um objeto User com base nos dados lidos
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

//Contar linhas de um ficheiro
int Casino::ContarLinhas(const string& nomeArquivo) {
    int numeroLinhas = 0;
    string linha;
    ifstream F(nomeArquivo); //Abre ficheiro fornecido
    if (!F.is_open()){ //Se ficheiro foi aberto com sucesso
        cout << "Erro ao abrir o ficheiro." << endl;
        return -1; //Indicativo de erro
    }else{
        while (getline(F, linha)){ //Loop para ler linhas do ficheiro
            numeroLinhas++;
        }
        F.close(); //Fecha ficheiro
    }
    return numeroLinhas;
}

//Jogador entra no casino
bool Casino::entrarJogador(){
    bool entrada = false;
    srand (time(NULL)); //Inicia o gerador de numeros aleatorios com o tempo atual
    int nRandom1 = rand() % 100 + 1; //Gera numero aleatorio entre 1 e 100
    //cout << "nRandom1: " << nRandom1 << endl;
    if(nRandom1 < 50){ //Se numero gerado menor que 50
        entrada = true;
    }
    return entrada;
}
