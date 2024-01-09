#include "Casino.h"
#include <conio.h>
#include <chrono>
#include <thread>

#include <iostream>
#include <fstream>

using namespace std;
#include <string>
#include <list>
#include "pugixml.hpp"
#include "XMLReader.h"
#include "MBlackJack.h"
#include "MPoker.h"
#include "MRoleta.h"
#include "MSlot.h"

#include <cmath>

#include <list>
#include <string>
#include <cctype>

#include "XMLWriter.h"


#include <cstdlib>
#include <ctime>
#include <cstdlib>  // Para a fun��o sleep
#include <iomanip>

#include <sstream>  // Inclua este cabe�alho para std::stringstream

#include <random>
#include <ctime>

// sleep function
#include <iostream>

// Library effective with Windows
#include <windows.h>
/*TODO
    Casino::Casino (string nome) *
    Fazer fun��o de reparar m�quina
    Listar Maquinas avariadas
    Listar maquina com mais ganhos
    Fazer fun��o de registar xml
    Fazer fun��o de criar maquina
    Melhorar fun��o run()
    Fazer fun��o estouAvaridado
    Menu
    dadosCasino();
    //Add();
    //maquinaAvariada();
    ListMachines();
    //maquinaAvariada();
    //maquinaGanhos();
    //reparar();
    //registarMaquina();
    int nMaquina;
    cin >> nMaquina;
    Desligar(nMaquina);
    cout << "Pesquisar m�quina" << endl;
    int idMaquina;
    cin >> idMaquina;
    estadoString(Get_Estado(idMaquina));
    ListarMaquinasProbabilidade();
    menuCrudMaquina();
*/
Casino::Casino(string _nome)
{
    //ctor
    nome = _nome;
}

Casino::~Casino()
{
    //dtor
    //destui Maquinas
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it)
           delete (*it);
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
    totalPremios=0;
    totalDinheiroDado=0;
}

void Casino::Listar(ostream &f){

    system("cls");

    cout << endl;
    cout << "Listar estado atual do Casino" << endl;
    cout << endl;
    cout << "| ID Maquina | Nome Maquina | Probabilidade | Estado | temperatura |" << endl;

    f << "Estado Casino " << nome << endl;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {

        f << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: " << estadoString((*it)->getEstado()) << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        if((*it)->getEstado()== ON){
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;32m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== OFF){
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;33m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== AVARIADA){
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;31m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }

    }

    cout << endl;
    cout << "Jogadores no casino: " << jogadoresNoCasino << endl;
    cout << "Numero total de jogadores: " << LU.size() << endl;

    cout << endl;
}




bool Casino::LoadMachinesFromXML(const string& filename) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        cerr << "Erro ao carregar o arquivo XML: " << result.description() << endl;
        return false;
    }


    pugi::xml_node listaMaq = doc.child("DADOS").child("LISTA_MAQ");

    for (pugi::xml_node maquina = listaMaq.child("MAQUINA"); maquina; maquina = maquina.next_sibling("MAQUINA")) {
        int id = maquina.child("ID").text().as_int();
        string nome = maquina.child("NOME").text().get();
        int x = maquina.child("X").text().as_int();
        int y = maquina.child("Y").text().as_int();
        int premio = maquina.child("PREMIO").text().as_int();
        int probG = maquina.child("PROB_G").text().as_int();
        string tipo = maquina.child("TIPO").text().get();

        Maquina* m = nullptr;

        if (tipo == "slot") {
            m = new MSlot(id, nome, x, y, premio, probG, tipo,10,this);
        } else if (tipo == "poker") {
            m = new MPoker(id, nome, x, y, premio, probG, tipo,15,this);
        }

        // Adiciona a m�quina ao vetor do Casino
        Add(m);

    }

    return true;
}


//Menu
void Casino::Menu(){

    int op = 0;
    do {

        cout << "||||||||||||||||||||||" << endl;
        cout << "|        Menu        |" << endl;
        cout << "|   1- Dados Casino  |" << endl;
        cout << "|  2- Gest�o Casino  |" << endl;
        cout << "| 3- Gest�o Maquinas |" << endl;
        cout << "|   4- Gest�o User   |" << endl;
        cout << "|  5- Memoria Total  |" << endl;
        cout << "|       0- Sair      |" << endl;
        cout << "||||||||||||||||||||||" << endl;
        cout << endl;
        cout << "Op��o: ";
        cin >> op;

        switch(op){

        case 1:
            system("cls");
            cout<< "Dados Casino" <<endl;
            dadosCasino();
            break;
        case 2:
            system("cls");
            cout<< "Gest�o Casino" <<endl;
            gestaoCasino();
            break;
        case 3:
            system("cls");
            cout<< "Gest�o Maquinas" <<endl;
            gestaoMaquinas();
            break;
        case 4:
            system("cls");
            cout<< "Gest�o User" <<endl;
            Jogadores_Mais_Ganhos();
            break;
        case 5:
            system("cls");
            cout<< "Memoria Total" <<endl;
            Memoria_Total();
            break;
        case 0:
            break;

        default:
            cout << "Op��o inv�lida. Tente novamente." << endl;
        }
    }
    while (op != 0);

}

//Menu Gest�o Casino
void Casino::gestaoCasino(){

    int op = 0;
    string nome;
    ofstream F("estadoAtualCasino.txt");

    do {

        cout << "Gest�o Casino" << endl;
        cout << "1- Listar estado atual casino" << endl;
        cout << "2- Relatorio" << endl;
        cout << "3- Listar maquinas com probabilidade superiora X" << endl;
        cout << "0- Sair" << endl;

        cout << endl;
        cout << "Op��o: ";
        cin >> op;

        switch(op){

        case 1:
            cout<< "Listar estado atual casino" <<endl;
            Listar(F);
            break;
        case 2:
            int id_maq;
            cout<< "Relatorio" <<endl;
            nome = devolveData();
            Relatorio(nome);
            break;
        case 3:
            system("cls");
            cout<< "Listar maquinas com probabilidade superiora X" <<endl;
            ListarMaquinasProbabilidade();
            break;
        case 0:
            break;

        default:
            cout << "Op��o inv�lida. Tente novamente." << endl;
        }
    }
    while (op != 0);
}// Fim Menu Gest�o Casino

string Casino::devolveData(){

    time_t currentTime = time(nullptr);  // Obter o tempo atual
    struct tm * ptm = localtime(&currentTime);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ptm);
    string currentDate(buffer);

    // Criar o nome do arquivo XML com base na data
    string dataHoje = "data_" + currentDate + ".xml";

    return dataHoje;

}

//Menu Gest�o Maquinas
void Casino::gestaoMaquinas(){


    int op = 0;
    int idMaquina;

    do {

        cout << "Gest�o Maquinas" << endl;
        cout << "1- Listar maquinas" << endl;
        cout << "2- Crud maquina" << endl;
        cout << "3- Desligar maquina" << endl;
        cout << "4- Estado maquina ID" << endl;
        cout << "5- Listar maquinas do tipo" << endl;
        cout << "6- Ranking mais fracos" << endl;
        cout << "7- Ranking mais trabalhadores" << endl;
        cout << "0- Sair" << endl;

        cout << endl;
        cout << "Op��o: ";
        cin >> op;

        switch(op){

        case 1:
            system("cls");
            cout << "Listar maquinas" << endl;
            ListMachines();
            break;
        case 2:
            system("cls");
            cout << "Crud maquina" << endl;
            menuCrudMaquina();
            break;
        case 3:
            cout<< "Desligar maquina" <<endl;
            cout<< "ID: ";
            cin >> idMaquina;
            Desligar(idMaquina);
            break;
        case 4:
            cout<< "Estado maquina ID" <<endl;
            cout<< "ID: ";
            cin >> idMaquina;
            cout  << " | Estado: " << estadoString(Get_Estado(idMaquina)) << endl;
            break;
        case 5:
            cout << "Listar maquinas do tipo" << endl;
            listarTipoMaquina();
            break;
        case 6:
            cout << "Ranking mais fracos" << endl;
            showRankingAvarias();
            break;
        case 7:
            cout << "Ranking mais trabalhadores" << endl;
            Ranking_Das_Mais_Trabalhadores();
            break;
        case 0:
            break;

        default:
            cout << "Op��o inv�lida. Tente novamente." << endl;
        }
    }
    while (op != 0);
}// Fim Menu Gest�o Maquinas


//Fun��es

//Fun��o Run Casino
void Casino::Run(){
    char key;
    bool maquinasJaLigadas = false;
    while(true){
        cout<< "Menu" <<endl;
        time_t now = time(nullptr);
            tm* current_time = localtime(&now);

        int currentHour = current_time->tm_hour;//Hora atual
        int currentMinute = current_time->tm_min;//Minuto atual
        int currentSecond = current_time->tm_sec;//Segundo atual

        int openingTimeInSeconds = horaAbertura * 3600 + minutosAbertura * 60 + segundosAbertura;// Converte o hor�rio de abertura para segundos
        int closingTimeInSeconds = horaFecho * 3600 + minutosFecho * 60 + segundosFecho;// Converte o hor�rio de fechamento para segundos
        int currentTimeInSeconds = currentHour * 3600 + currentMinute * 60 + currentSecond;// Converte o hor�rio atual para segundos

        if (currentTimeInSeconds >= openingTimeInSeconds && currentTimeInSeconds <= closingTimeInSeconds) {

                    // O casino est� aberto!
                    cout << "O casino est� aberto!" << endl;
                    if (!maquinasJaLigadas) { //Verifica se j� tem todas as maquinas ligadas
                        ligarTodasMaquinas(); //Liga todas as maquinas
                        maquinasJaLigadas = true; //Guarda estado como j� as ligou
                    }

                    // Todas as maquinas existentes passam pela fun��o avariar e corre maquina
                    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){
                        avariar((*it)->getID()); //Fun��o avariar
                        (*it)->Run(); //Corre maquina
                    }

                    // Verifica se o Casino est� cheio  ou se podem entrar jogadores
                    if(jogadoresNoCasino < maxJogadores){
                            if(entrarJogador()){ //Verifica se entrou algum jogador

                                int randomIndex = rand() % LM.size(); //Vai buscar um numero random entre 0 e tamanho da lista
                                auto it = next(LM.begin(), randomIndex); //o next faz com que se obtenha a posi��o correta da maquina
                                //o auto it serve para ir buscar o tipo correto de dados que neste caso � m�quina
                                Maquina *maquina = *it; //guarda no apontador maquina o ponteiro it referente ao objeto amquina radnom
                                cout << "Maquina escolhida: " <<  maquina->getNome()  << endl;

                                jogadoresNoCasino ++; //Incrementa mais um jogador
                                User *user = userEntraCasino("pessoas.txt"); //Guarda no apontador user o user retornado pela fun��o userEntraCasino
                                Add(user); //Adiciona User


                                if(maquina->getUserAtual()!=nullptr){ //Verifica se a maquina escolhida anteriormente est� a ser usada
                                    user->entrarFilaEspera(maquina); //Se estiver o user entra na fila de espera
                                }else{
                                    user->associarMaquina(maquina); //se n�o estiver o user associa-se a uma maquina
                                }

                                cout << "User " <<  user->getCarteira()  << endl;
                                cout << "entrou " << jogadoresNoCasino << endl;

                            }
                    }

                    //Se existirem mais do que 1 user percorre a lista de useres
                    if(LU.size()>=1){
                        for (list<User *>::iterator it = LU.begin(); it != LU.end(); ++it){
                                (*it)->Run(); //correr user
                        }
                    }

                    // Verifica se saiu algum user
                    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
                        if ((*it)->getUserAtual()==nullptr) { //verifica se a maqina atual n�o possui nenhum jogador
                            if (!(*it)->getFilaEspera().empty()) { //verifica se lista de espera da maquina n�o esta vazia
                                User *useruser = (*it)->getFilaEspera().front(); //Vai buscar o user a frente na lista de espera da maquina
                                useruser->associarMaquina(*it); //user associa-se a maquina
                                (*it)->removerUsuarioFilaEspera(useruser); //e � retirado da lista de espera

                                cout << "User: " <<  useruser->getNome() << " saiu da fila de espera e sentou-se na maquina" << endl;

                            }else{
                                (*it)->setUserAtual(nullptr); //se n�o houver ninguem na lisat de espera o useratual da maquinafica a null
                            }
                        }
                    }

            } else {
                cout << "O casino est� fechado. Aguardando o hor�rio de abertura." << endl;
                if (maquinasJaLigadas) { //Se casino fechado ent�o verifica se as amquina ainda est�o ligadas
                    desligarTodasMaquinas(); //Desliga todas as maquina
                    maquinasJaLigadas = false; //Guarda estado como j� desligou
                }
            }

            if (_kbhit()) { //Fun��o de c++ que verifica se foi pressionada alguma tecla
                key = _getch(); //guarda tecka pressionada
                if (key == 'm' || key == 'M') { //verifica se � m ou M
                    Menu(); //abre Menu
                }
            }

    }

}

void Casino::avariar(int IDMaq){

    //srand (time(NULL));

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){ //Percorre todas as maquinas
        //Sleep(500);
        if((*it)->getID() == IDMaq){ //verifica se o ud da maquina atual � igual ao passado na fun��o

            if((*it)->getEstado() == ON){ //Verifica se amaquina est� ligada
                //rand() %100, vai buscar um numero entre 0 e 99 e adiciona mais 1 par air buscar o n�mero correto
                int nRandom1 = rand() % 100 + 1; //gera um numero random
                int nRandom2 = rand() % 100 + 1; //gera um numero random

                cout << "nRandom1: " << nRandom1 << endl;
                cout << "nRandom2: " << nRandom2 << endl;

                if(nRandom1 == nRandom2){ //se os numero random forem iguais ent�o avaria maquina
                    (*it)->avariaMaquina(); //Maquina chama fun��o avariaMaquina()
                }

            }
        }

    }

}


list<string> * Casino::Ranking_Dos_Fracos(){

     // Lista para armazenar as informa��es das m�quinas avariadas ordenadas por n�mero de avarias
    list<string>* rankingAvariadas = new list<string>;

    // Criar uma c�pia da lista principal
    list<Maquina*> maquinasCopy(LM.begin(), LM.end());

    // Ordenar a lista de m�quinas por n�mero de avarias (do maior para o menor)
    maquinasCopy.sort([](Maquina* a, Maquina* b) {
        return a->getnAvarias() > b->getnAvarias();
    });

    // Preencher a lista de strings com informa��es das m�quinas ordenadas
    for (auto maquina : maquinasCopy) {
        string info =   " |ID: " + to_string(maquina->getID()) +
                        " | Nome: " + maquina->getNome() +
                        " | Avarias: " + to_string(maquina->getnAvarias());
        rankingAvariadas->push_back(info);
    }

    return rankingAvariadas;

}

void Casino:: showRankingAvarias(){

    // Chamar a fun��o para obter o ranking das m�quinas avariadas
    list<std::string>* rankingAvariadas = Ranking_Dos_Fracos();

    // Verificar se a lista n�o est� vazia antes de tentar acessar os elementos
    if (rankingAvariadas->empty()) {
        cout << "Nenhuma m�quina avariada encontrada." << endl;
    } else {
        // Percorrer e exibir as informa��es das m�quinas no ranking
        for (const auto& info : *rankingAvariadas) {
            cout << info << endl;
        }
    }

    // N�o se esque�a de liberar a mem�ria alocada para a lista
    delete rankingAvariadas;

}

//Desligar todas as maquinas
void Casino::desligarTodasMaquinas() {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //perocorre todas as maquinas
        (*it)->Desligar(); //maquina chama fun��o Desligar()
    }
}

//Ligar todas as maquinas
void Casino::ligarTodasMaquinas() {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //perocorre todas as maquinas
        (*it)->Ligar(); //maquina chama fun��o Ligar()
    }
}

//Mostrar dados do casino
void Casino::dadosCasino() {
    cout << "Nome do Casino: " << nome << endl;
    cout << "M�ximo de Jogadores: " << maxJogadores << endl;
    cout << "Probabilidade de Usu�rios: " << probabilidadeUser << endl;
    cout << "Hora de Abertura: " << horaAbertura << ":" << minutosAbertura << ":" << segundosAbertura << endl;
    cout << "Hora de Encerramento: " << horaFecho << ":" << minutosFecho << ":" << segundosFecho << endl;
}

bool Casino::Add(Maquina *m){

    adicionarVizinho(m);
    LM.push_back(m);

    return true;
}

bool Casino::adicionarVizinho(Maquina *m) {

    bool estado = false;
    int distanciaMaxima = 3;  // Defina a dist�ncia m�xima desejada

    for (auto it = LM.begin(); it != LM.end(); ++it) {
        if ((*it)->getID() != m->getID() && std::abs((*it)->getX() - m->getX()) <= distanciaMaxima) {
            int distanciaY = std::abs((*it)->getY() - m->getY());

            if (distanciaY <= distanciaMaxima) {
                // Adicionar novaMaquina � lista de vizinhos de maquina
                (*it)->adicionarVizinho(m);

                // Adicionar maquina � lista de vizinhos de novaMaquina
                m->adicionarVizinho(*it);

                estado = true;
            }
        }
    }

    return estado;
}

void Casino::Desligar(int id_maq) {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if((*it)->getID() == id_maq){
            (*it)->Desligar();
            cout << "M�quina " << (*it)->getNome() << " desligada." << endl;
            return;
        }

    }
    cout << "M�quina n�o encontrada." << endl;
}

estadoMaquina Casino::Get_Estado(int id_maq) {

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if((*it)->getID() == id_maq){
            return (*it)->getEstado();
        }

    }
    cout << "M�quina n�o encontrada." << endl;
    return estadoMaquina::OFF;
}

void Casino::Listar(float X, std::ostream &f) {

    f << "M�quinas com probabilidade " << X << " maior que de ganhar:" << endl;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if ((*it)->getProb() > X) {
            f << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl;
            cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl;
        }
    }

}

void Casino::ListarMaquinasProbabilidade() {

    try {
        // Listar m�quinas com probabilidade de maior que x
        float xProbabilidade;
        cout << "Probabilidade: ";
        cin >> xProbabilidade;

        // Tentar abrir o arquivo
        ofstream F("ListaProbX.txt");
        Listar(xProbabilidade, F);

        // Fechar o arquivo ap�s o uso
        F.close();
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
    }

}

string Casino::estadoString(estadoMaquina estadoma){
    string estadoString;
    switch (estadoma) {
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

    return estadoString;
}


void Casino::maquinaAvariada(){

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if ((*it)->getEstado() ==  AVARIADA) {
            cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl;
        }
    }

    cout << "Deseja reparar alguma maquina (S/N): " << endl;
    char repar;
    cin >> repar;

    if(repar == 'S' || repar == 's'){
        int id_maq;
        cout << "Numero Maquina: " ;
        cin >> id_maq;
        reparar(id_maq);
    }


}
void Casino::maquinaGanhos(){


}
void Casino::reparar(int id_maq){

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if ((*it)->getID() == id_maq) {
            if ((*it)->getEstado() == AVARIADA){
                (*it)->repararMaquina();
                cout << "ID: " << (*it)->getID() << " | Maquina Reparada" << endl;
            }
            else if ((*it)->getEstado() == ON || (*it)->getEstado() == OFF){
                cout << "ID: " << (*it)->getID() << " | Maquina sem avarias" << endl;
            }
        }
    }


}
void Casino::registarMaquina(){


}

void Casino::ListMachines() const {
    cout << "|ID: | Nome: | Posi��o: (x ,y ) | Pr�mio | ProbG:  | Estado: " << endl;


    for (const auto& maquina : LM) {
        maquina->DisplayInfo();
    }
}

/*
// Listar m�quinas com probabilidade de maior que x
        float xProbabilidade;
        cout << "Probabilidade: ";
        cin >> xProbabilidade;

        // Tentar abrir o arquivo
        ofstream F("ListaProbX.txt");
        Listar(xProbabilidade, F);

        // Fechar o arquivo ap�s o uso
        F.close();
        */

//Outras Fun��es

void Casino::listarTipoMaquina(){
    string Tipo = "";
    cin >> Tipo;

    // Tentar abrir o arquivo
    ofstream F("MaquinasTipo.txt");
    Listar_Tipo(Tipo, F);

    // Fechar o arquivo ap�s o uso
    F.close();
}

list<Maquina *> Casino::Listar_Tipo(string Tipo, std::ostream &f) {
    list<Maquina *> maquinasDoTipo;



    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if ((*it)->getTipo() == Tipo) {
            maquinasDoTipo.push_back((*it));
            f << "ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << endl;
        }
    }

    return maquinasDoTipo;


}




//Menus
//Menu Crud Maquinas
void Casino::menuCrudMaquina(){


    int op = 0;

    do {
            //system("cls");
        // code block to be executed
        cout<< "Crud Maquina" <<endl;
        cout<< "1- Adicionar Maquina" <<endl;
        cout<< "2- Remover Maquina" <<endl;
        cout<< "3- Editar Maquina" <<endl;
        cout<< "4- Mover Maquina" <<endl;
        cout<< "0- Sair" <<endl;

        cin >> op;

        switch(op){

        case 1:
            cout<< "Adicionar Maquina" <<endl;
            addMaquina();
            break;
        case 2:
            int id_maq;
            cout<< "Remover Maquina" <<endl;
            cin >> id_maq;
            removerMaquina(id_maq);
            break;
        case 3:
            int id_maqq;
            cout<< "Editar Maquina: Introduza id n�mero maquina" <<endl;
            cin >> id_maqq;
            editarMaquina(id_maqq);
            break;
        case 4:
            int id_maqqq;
            cout <<"ID Maquina: ";
            cin >> id_maqqq;
            moverMaquina(id_maqqq);
            break;
        case 0:
            break;

        default:
            cout << "Op��o inv�lida. Tente novamente." << endl;
        }
    }
    while (op != 0);
}// Fim Menu Crud Maquinas

// Crud Maquinas
void Casino::addMaquina(){

    int nMaquina, premio, x, y, temperaturaSensor;
    int xMaquina, yMaquina;
    string nome, tipo;
    float prob;
    bool existe = false;
    Casino *Ptr_Casino;

    /*int getY(){ return y;}
        int getX()*/
    cout << "Nome m�quina" << endl;
    cin >> nome;
    do {
        cout << "X m�quina" << endl;
        cin >> x;
        for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
            if ((*it)->getX() == x) {
                cout << "Y m�quina" << endl;
                cin >> y;
                if ((*it)->getY() == y) {
                    existe = true;
                }else{
                    existe = false;
                }
            }
        }
    }
    while (existe == true);

    cout << "Premio m�quina" << endl;
    cin >> premio;
    cout << "Probabilidade m�quina" << endl;
    cin >> prob;
    cout << "Tipo m�quina" << endl;
    cin >> tipo;

    Maquina* m = nullptr;

    int id = LM.size()+1;

    if (tipo == "slot" || tipo == "Slot") {
        m = new MSlot(id, nome, x, y, premio, prob, tipo,10, this);
    } else if (tipo == "poker" || tipo == "Poker") {
        m = new MPoker(id, nome, x, y, premio, prob, tipo,15, this);
    }

    Add(m);
}

bool Casino::removerMaquina(int id_maq) {

    removerVizinho(id_maq);

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        if ((*it)->getID() == id_maq) {
            //delete *it;  // Libera a mem�ria alocada pela m�quina
            LM.erase(it); // Remove a m�quina da lista
            cout << "M�quina removida." << endl;
            return true;
        }
    }

    cout << "M�quina n�o encontrada." << endl;
    return false;
}

void Casino::removerVizinho(int id_maq) {

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {

        //eleminar vizinho das maquinas
        (*it)->removerVizinho(id_maq);


    }
    cout << "Eleminei tudo." << endl;

}

bool Casino::editarMaquina(int id_maq){
    cout << " |Dados maquina|" << id_maq << endl;
    int op;
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
                        cout << "Op��o inv�lida. Tente novamente." << endl;
                }
                system("cls");
            }

        while (op != 0);
        }

    }


    cout << "M�quina n�o encontrada." << endl;
    return false;
}

bool Casino::moverMaquina(int id_maq){

    int x, y, xAnt;
    bool movido = false;

    cout << "Mover M�quina de lugar" << endl;

    // Procurar a m�quina com o ID fornecido
    list<Maquina *>::iterator itFirst = LM.begin();
    while (itFirst != LM.end() && (*itFirst)->getID() != id_maq) {
        ++itFirst;
    }

    if (itFirst != LM.end()) {
        cout << " |Nome Maquina: " << (*itFirst)->getNome() << "| " << endl;
        cout << " |X: " << (*itFirst)->getX() << "| " << endl;
        cout << " |Y: " << (*itFirst)->getY() << "| " << endl;
        cout << " |Probabilidade: " << (*itFirst)->getProb() << "| " << endl;
        cout << " |Premio: " << (*itFirst)->getPremio() << "| " << endl;
        cout << endl;

        do {
            // Obter a nova posi��o
            cout << "Novo valor de X: ";
            cin >> x;
            cout << "Novo valor de Y: ";
            cin >> y;

            // Verificar se a nova posi��o j� est� ocupada
            bool posicaoOcupada = false;
            list<Maquina *>::iterator it = LM.begin();
            while (it != LM.end()) {
                if ((*it)->getX() == x && (*it)->getY() == y) {
                    posicaoOcupada = true;
                    cout << "Posi��o j� ocupada. Escolha outra posi��o." << endl;
                    break;
                }
                ++it;
            }

            if (!posicaoOcupada) {
                // Atualizar a posi��o da m�quina
                (*itFirst)->setX(x);
                (*itFirst)->setY(y);
                movido = true;
                cout << "M�quina movida para (" << x << ", " << y << ")" << endl;

                //Atualizar vizinhos
                ////Remover vizinho
                (*itFirst)->removerVizinhoTodos();
                removerVizinho((*itFirst)->getID());

                ////Adicionar nvo vizinho
                adicionarVizinho((*itFirst));


            }

        } while (!movido);
    } else {
        cout << "M�quina com ID " << id_maq << " n�o encontrada." << endl;
    }

    return movido;

}

// Fim Crud Maquinas



User* Casino::userEntraCasino(const string &nomeArquivo){

    int numeroLinhas = ContarLinhas(nomeArquivo);
    if (numeroLinhas != -1) {
        cout << "O arquivo tem " << numeroLinhas << " linhas." << endl;
    }

    // Gere um n�mero aleat�rio dentro do intervalo de linhas do arquivo
    int linhaAleatoria = rand() % numeroLinhas + 1;

    // Abra o arquivo
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return nullptr;
    }

    // Leitura das linhas at� chegar � linha desejada
    string linha;
    for (int i = 0; i < linhaAleatoria; ++i) {
        getline(arquivo, linha);
    }

    // Use um stringstream para processar a linha
    stringstream ss(linha);


    // Declare as vari�veis para armazenar os dados do usu�rio
    int id;
    string nome, sobrenome, morada;
    int idade;

    // Leitura dos dados do usu�rio da linha
    // Leitura dos dados do usu�rio da linha
    ss >> id;
    ss.ignore(); // Ignora o espa�o em branco
    getline(ss, nome, '\t');
    getline(ss, morada, '\t');
    ss >> idade;

    cout << "C�digo: " << id << endl;
    cout << "Nome: " << nome << endl;
    cout << "Morada: " << morada << endl;
    cout << "Idade: " << idade << endl;

    // Feche o arquivo
    arquivo.close();

    // Crie um objeto User com base nos dados lidos
    User* newUser = new User(id, nome, morada, idade, this);

    jogadoresNoCasino +1;

    // Adicione o usu�rio ao casino
    return newUser;
}


bool Casino::Add(User *ut){

    if (ut == nullptr) {
        cerr << "Erro: Tentativa de adicionar um usu�rio nulo." << endl;
        return false;
    }

    LU.push_back(ut);

    cout << "Usu�rio adicionado com sucesso!" << endl;

    return true;

}

int Casino::ContarLinhas(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return -1; // Indicativo de erro
    }

    int numeroLinhas = 0;
    string linha;

    while (getline(arquivo, linha)) {
        numeroLinhas++;
    }

    cout << "numero de linahs    " << numeroLinhas << endl;
    arquivo.close();
    return numeroLinhas;
}

bool Casino::entrarJogador(){

    bool entrada = false;

    srand (time(NULL));

    int nRandom1 = rand() % 100 + 1;
    cout << "nRandom1: " << nRandom1 << endl;

    if(nRandom1 < 50){
        entrada = true;
    }

    return entrada;

}


list<Maquina *> *Casino::Ranking_Das_Mais_Trabalhadores() {
    // Crie uma c�pia da lista de m�quinas
    list<Maquina *> *copiaMaquinas = new list<Maquina *>(LM.begin(), LM.end());

    copiaMaquinas->sort([](Maquina* a, Maquina* b) {
        return a->getNJogos() > b->getNJogos();
    });

    // Ordene a lista usando a fun��o de compara��o
    //copiaMaquinas->sort(compararNjogos);

    for (auto maquina : *copiaMaquinas) {

        cout << "ID: " << maquina->getID() << " | Nome: " << maquina->getNome() << " | Tipo: " << maquina->getNJogos() << endl;

    }


    return copiaMaquinas;
}

bool compararNjogos(Maquina *maquina1, Maquina *maquina2) {
    // Compare o n�mero de vezes que as m�quinas foram utilizadas
    return maquina1->getNJogos() > maquina2->getNJogos();
}




list<User *> * Casino::Jogadores_Mais_Ganhos () {

    // Crie uma c�pia da lista de m�quinas
    list<User *> *copiaUser = new list<User *>(LU.begin(), LU.end());

    copiaUser->sort([](User* a, User* b) {
        return a->getGanhos() > b->getGanhos();
    });

    // Ordene a lista usando a fun��o de compara��o
    //copiaMaquinas->sort(compararNjogos);

    for (auto user : *copiaUser) {

        cout << "ID: " << user->getNUser() << " | Nome: " << user->getNome() << " | ganhos: " << user->getGanhos() << endl;

    }


    return copiaUser;


}

list<User *> * Casino::Jogadores_Mais_Frequentes (){


    // Crie uma c�pia da lista de m�quinas
    list<User *> *copiaUser = new list<User *>(LU.begin(), LU.end());

    copiaUser->sort([](User* a, User* b) {
        return a->getTempoCasino() > b->getTempoCasino();
    });

    // Ordene a lista usando a fun��o de compara��o
    //copiaMaquinas->sort(compararNjogos);

    for (auto user : *copiaUser) {

        cout << "ID: " << user->getNUser() << " | Nome: " << user->getNome() << " | tempo casino: " << user->getTempoCasino() << endl;

    }


    return copiaUser;


}

void Casino::Relatorio(string fich_xml) {

    cout<< "Nome ficheiro: " << fich_xml <<endl;

    XMLWriter XX;
    XX.WriteStartDocument(fich_xml);
    XX.WriteStartElement("RELATORIO");

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {

        XX.WriteStartElement("Maquina"); // Abre o Elemento �Maquina�
            XX.WriteElementString("ID",to_string((*it)->getID()));
            XX.WriteElementString("ESTADO",estadoString((*it)->getEstado()));
        XX.WriteEndElement(); // Fecha o Elemento �Maquina�

    }

    XX.WriteEndElement(); // Fecha o Elemento �RELATORIO�
}



void Casino::SubirProbabilidadeVizinhas(Maquina *M_ganhou, float R, list<Maquina *> &lmvizinhas) {

    for (list<Maquina *>::iterator it = lmvizinhas.begin(); it != lmvizinhas.end(); ++it) {
        int distanciaY = abs((*it)->getY() - M_ganhou->getY());
        if (distanciaY <= R) {
            float probAt = (*it)->getProb();
            (*it)->setProb(probAt + 1.0);
        }
    }

}

int Casino::Memoria_Total() {

    int mem = 0;


    return mem;

}

