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

void Casino::CarregarDados(int _maxJogadores, int _probabilidadeUser, int _horaAbertura, int _horaFecho)
{

    maxJogadores = _maxJogadores;
    probabilidadeUser = _probabilidadeUser;
    horaAbertura = _horaAbertura;
    horaFecho = _horaFecho;

    jogadoresNoCasino=0;
    totalPremios=0;
    totalDinheiroDado=0;
}

void Casino::Listar(ostream &f){

    //Compor listagem para listar o estado atual do casino

    system("cls");

    cout << endl;
    cout << "Listar estado atual do Casino" << endl;
    cout << endl;
    cout << "| ID Maquina | Nome Maquina | Probabilidade | Estado | temperatura |" << endl;

    f << "Estado Casino " << nome << endl;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {

            f << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: " << estadoString((*it)->getEstado()) << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: " << estadoString((*it)->getEstado()) << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;

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

    return true; // Indica que a leitura foi bem-sucedida
}


//Menu
void Casino::Menu(){

    //system("cls");
    int op = 0;
    do {
        // code block to be executed
        cout<< "||||||||||||||||||||||" <<endl;
        cout<< "|        Menu        |" <<endl;
        cout<< "|   1- Dados Casino  |" <<endl;
        cout<< "|  2- Gest�o Casino  |" <<endl;
        cout<< "| 3- Gest�o Maquinas |" <<endl;
        cout<< "|   4- Gest�o User   |" <<endl;
        cout<< "|  5- Memoria Total  |" <<endl;
        cout<< "|       0- Sair      |" <<endl;
        cout<< "||||||||||||||||||||||" <<endl;

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
            //ContarLinhas("pessoas.txt");
            break;
        case 5:
            system("cls");
            cout<< "Memoria Total" <<endl;
            //Memoria();//Implementar total memoria
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
    ofstream F("estadoAtualCasino.txt");

    do {
        // code block to be executed
        cout << "Gest�o Casino" << endl;
        cout << "1- Listar estado atual casino" << endl;
        cout << "2- Relatorio" << endl;
        cout << "3- Subir probabilidade" << endl;
        cout << "4- Listar maquinas com probabilidade superiora X" << endl;
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
            maquinaAvariada();
            break;
        case 3:
            cout<< "Subir probabilidade" <<endl;
            break;
        case 4:
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

//Menu Gest�o Maquinas
void Casino::gestaoMaquinas(){


    int op = 0;

    do {
        // code block to be executed
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
            int nMaquina;
            cout<< "ID: ";
            cin >> nMaquina;
            Desligar(nMaquina);
            break;
        case 4:
            cout<< "Estado maquina ID" <<endl;
            int idMaquina;
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

//Menu
/*
system("cls");
    int op = 0;
    do {
        // code block to be executed
        cout<< "Menu" <<endl;
        cout<< "1- Dados Casino" <<endl;
        cout<< "2- Adicionar M�quina" <<endl;
        cout<< "3- Maquinas Avariadas" <<endl;
        cout<< "4- Maquina mais ganhos" <<endl;
        cout<< "5- Reparar" <<endl;
        cout<< "7- Desligar m�quina" <<endl;
        cout<< "8- Estado M�quina" <<endl;

        cin >> op;

        switch(op){

        case 1:
            cout<< "Dados Casino" <<endl;
            dadosCasino();
            //Add();
            //maquinaAvariada();
            break;
        case 2:
            cout<< "Maquinas Avariadas" <<endl;
            ListMachines();
            //maquinaAvariada();
            break;
        case 3:
            cout<< "Maquina mais ganhos" <<endl;
            //maquinaGanhos();
            break;
        case 4:
            cout<< "Reparar" <<endl;
            //reparar();
            break;
        case 5:
            cout<< "Registar Maquina" <<endl;
            //registarMaquina();
            break;
        case 7:
            cout<< "Desligar M�quina" <<endl;
            int nMaquina;
            cin >> nMaquina;
            Desligar(nMaquina);
            break;
        case 8:
            cout << "Pesquisar m�quina" << endl;
            int idMaquina;
            cin >> idMaquina;
            estadoString(Get_Estado(idMaquina));
            break; // Este break � necess�rio para corrigir o erro
        case 9:
            ListarMaquinasProbabilidade();
            break; // Este break � necess�rio para corrigir o erro
        case 10:
            cout << "Crud maquina" << endl;
            menuCrudMaquina();
            break; // Este break � necess�rio para corrigir o erro
        case 0:
            break;

        default:
            cout << "Op��o inv�lida. Tente novamente." << endl;
        }
    }
    while (op != 20);
*/

//Fun��es


//Fun��o Run Casino
void Casino::Run(){
    char key;
    bool maquinasJaLigadas = false;
    while(true){
        cout<< "Menu" <<endl;
        time_t now = time(nullptr);
            tm* current_time = localtime(&now);

            int currentHour = current_time->tm_hour;

            if (currentHour >= horaAbertura && currentHour < horaFecho) {
                cout << "O casino est� aberto!" << endl;
                if (!maquinasJaLigadas) {
                    ligarTodasMaquinas();
                    maquinasJaLigadas = true;
                }

                for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){
                    avariar((*it)->getID());
                    (*it)->Run();
                }

                if(jogadoresNoCasino < maxJogadores){
                        if(entrarUser()){

                            int randomIndex = std::rand() % LM.size();
                            auto it = std::next(LM.begin(), randomIndex);
                            Maquina *maquina = *it;
                            cout << "Maquinaaaaaaaaaaaaaaaaa " <<  maquina->getNome()  << endl;

                            jogadoresNoCasino ++;
                            User *user = userEntraCasino("pessoas.txt");
                            Add(user);


                            if(maquina->getUserAtual()!=nullptr){
                                user->entrarFilaEspera(maquina);
                            }else{
                                user->associarMaquina(maquina);
                            }

                            /*if(maquina->getUtilizacao()){//Se estiver em utiliza��ouser->entrarFilaEspera(maquina);}else{user->associarMaquina(maquina);}*/

                            cout << "Maquinaaaaaaaaaaaaaaaaa " <<  user->getCarteira()  << endl;
                            cout << "entrou " << jogadoresNoCasino << endl;

                        }
                }


                if(LU.size()>=1){
                    for (list<User *>::iterator it = LU.begin(); it != LU.end(); ++it){
                            (*it)->Run();
                    }
                }

                /*if (LU.size()>= 1){for (list<User *>::iterator it = LU.begin(); it != LU.end(); ++it){(*it)->Run();}}*/

                // Verifica se saiu algum user
                for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
                    if ((*it)->getUserAtual()==nullptr) {
                        if (!(*it)->getFilaEspera().empty()) {
                            User *useruser = (*it)->getFilaEspera().front();
                            useruser->associarMaquina(*it);
                            (*it)->removerUsuarioFilaEspera(useruser);

                            cout << "User: " <<  useruser->getNome() << " saiu da fila de espera e sentou-se na maquina" << endl;

                            // Antes de associar a m�quina, certifique-se de que ela n�o est� sendo usada//(*it)->setUtilizacao(true); // Marca a m�quina como em uso//iniciarJogo(user); // Inicia o jogo para o usu�rio associado � m�quina//else {// Se a m�quina estiver em uso, o usu�rio entra na fila de espera//user->entrarFilaEspera(*it);//}

                        }else{
                            (*it)->setUserAtual(nullptr);
                        }
                    }
                }

                //Jogadores vao jogar/*for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){avariar((*it)->getID());(*it)->Run();}*/


            } else {
                cout << "O casino est� fechado. Aguardando o hor�rio de abertura." << endl;
                if (maquinasJaLigadas) {
                    desligarTodasMaquinas();
                    maquinasJaLigadas = false;
                }
            }
            if (_kbhit()) {
                key = _getch();
                if (key == 'm' || key == 'M') {
                    Menu();
                }
            }
            //corre o resto do programa
    }

}

void Casino::avariar(int nMaq){

    /* initialize random seed: */
    srand (time(NULL));

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){
        Sleep(500);
        if((*it)->getID()==nMaq){

            if((*it)->getEstado()==ON){
                int nRandom1 = rand() % 100 + 1;
                int nRandom2 = rand() % 100 + 1;

                cout << "nRandom1: " << nRandom1 << endl;
                cout << "nRandom2: " << nRandom2 << endl;

                if(nRandom1 == nRandom2){
                    (*it)->avariaMaquina();
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
        string info = "ID: " + to_string(maquina->getID()) +
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

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        (*it)->Desligar();
    }
}

//Ligar todas as maquinas
void Casino::ligarTodasMaquinas() {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        (*it)->Ligar();
    }
}

/**/

//Mostrar dados do casino
void Casino::dadosCasino() {
    cout << "Nome do Casino: " << nome << endl;
    cout << "M�ximo de Jogadores: " << maxJogadores << endl;
    cout << "Probabilidade de Usu�rios: " << probabilidadeUser << endl;
    cout << "Hora de Abertura: " << horaAbertura << endl;
    cout << "Hora de Encerramento: " << horaFecho << endl;
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
            (*it)->repararMaquina();
            cout << "ID: " << (*it)->getID() << " | Maquina Reparada" << endl;
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

bool Casino::entrarUser(){

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



void Casino::SubirProbabilidadeVizinhas(Maquina *M_ganhou, float R, list<Maquina *> &lmvizinhas) {

    for (list<Maquina *>::iterator it = lmvizinhas.begin(); it != lmvizinhas.end(); ++it) {
        int distanciaY = abs((*it)->getY() - M_ganhou->getY());
        if (distanciaY <= R) {
            float probAt = (*it)->getProb();
            (*it)->setProb(probAt + 1.0);
        }
    }

}

