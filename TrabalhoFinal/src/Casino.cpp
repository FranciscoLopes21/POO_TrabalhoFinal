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

#include <iostream>
#include <ctime>
#include <cstdlib>  // Para a fun��o sleep
#include <iomanip>

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
            m = new MSlot(id, nome, x, y, premio, probG, tipo);
        } else if (tipo == "poker") {
            m = new MPoker(id, nome, x, y, premio, probG, tipo);
        }

        // Adiciona a m�quina ao vetor do Casino
        Add(m);

    }

    return true; // Indica que a leitura foi bem-sucedida
}


//Menu
void Casino::Menu(){

    system("cls");
    int op = 0;
    do {
        // code block to be executed
        cout<< "Menu" <<endl;
        cout<< "1- Dados Casino" <<endl;
        cout<< "2- Gest�o Casino" <<endl;
        cout<< "3- Gest�o Maquinas" <<endl;
        cout<< "4- Gest�o User" <<endl;
        cout<< "5- Memoria Total" <<endl;

        cin >> op;

        switch(op){

        case 1:
            system("cls");
            cout<< "Dados Casino" <<endl;
            dadosCasino();
            break;
        case 2:
            cout<< "Gest�o Casino" <<endl;
            gestaoCasino();
            break;
        case 3:
            cout<< "Gest�o Maquinas" <<endl;
            gestaoMaquinas();
            break;
        case 4:
            cout<< "Gest�o User" <<endl;
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

    do {
        // code block to be executed
        cout<< "Gest�o Casino" <<endl;
        cout<< "1- Listar estado casino" <<endl;
        cout<< "2- Relatorio" <<endl;
        cout<< "3- Subir probabilidade" <<endl;
        cout<< "4- Listar maquinas com probabilidade superiora X" <<endl;
        cout<< "0- Sair" <<endl;

        cin >> op;

        switch(op){

        case 1:
            cout<< "Listar estado casino" <<endl;
            break;
        case 2:
            int id_maq;
            cout<< "Relatorio" <<endl;
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
        cout<< "Gest�o Casino" <<endl;
        cout<< "1- Listar maquinas" <<endl;
        cout<< "2- Crud maquina" <<endl;
        cout<< "3- Desligar maquina" <<endl;
        cout<< "4- Estado maquina ID" <<endl;
        cout<< "5- Listar maquinas do tipo" <<endl;
        cout<< "6- Ranking mais fracos" <<endl;
        cout<< "7- Ranking mais trabalhadores" <<endl;
        cout<< "0- Sair" <<endl;

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
            cin >> nMaquina;
            Desligar(nMaquina);
            break;
        case 4:
            cout<< "Estado maquina ID" <<endl;
            int idMaquina;
            cin >> idMaquina;
            estadoString(Get_Estado(idMaquina));
            break;
        case 5:
            cout << "Listar maquinas do tipo" << endl;
            listarTipoMaquina();
            break;
        case 6:
            cout << "Ranking mais fracos" << endl;
            break;
        case 7:
            cout << "Ranking mais trabalhadores" << endl;
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
                for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it)
                    (*it)->Run();
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

void Casino::dadosCasino() {
    cout << "Nome do Casino: " << nome << endl;
    cout << "M�ximo de Jogadores: " << maxJogadores << endl;
    cout << "Probabilidade de Usu�rios: " << probabilidadeUser << endl;
    cout << "Hora de Abertura: " << horaAbertura << endl;
    cout << "Hora de Encerramento: " << horaFecho << endl;
}

bool Casino::Add(Maquina *m){

    LM.push_back(m);
    return true;
}

void Casino::Desligar(int id_maq) {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if((*it)->getID() == id_maq){
            (*it)->Desligar();
            cout << "M�quina desligada." << endl;
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

    std::cout  << " | Estado: " << estadoString << std::endl;
}


void Casino::maquinaAvariada(){


}
void Casino::maquinaGanhos(){


}
void Casino::reparar(){


}
void Casino::registarMaquina(){


}

void Casino::ListMachines() const {
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

    for (auto maquina : LM) {
        if (maquina->getTipo() == Tipo) {
            maquinasDoTipo.push_back(maquina);
            f << "ID: " << maquina->getID() << " | Nome: " << maquina->getNome() << " | Tipo: " << maquina->getTipo() << endl;
        }
    }

    return maquinasDoTipo;

}



//getNome
string Casino::getNome(){
    return nome;
}

//Menus
//Menu Crud Maquinas
void Casino::menuCrudMaquina(){


    int op = 0;

    do {
            system("cls");
        // code block to be executed
        cout<< "Crud Maquina" <<endl;
        cout<< "1- Adicionar Maquina" <<endl;
        cout<< "2- Remover Maquina" <<endl;
        cout<< "3- Editar Maquina" <<endl;
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
            cout<< "Editar Maquina" <<endl;
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
        m = new MSlot(id, nome, x, y, premio, prob, tipo);
    } else if (tipo == "poker" || tipo == "Poker") {
        m = new MPoker(id, nome, x, y, premio, prob, tipo);
    }

    Add(m);
}

bool Casino::removerMaquina(int id_maq) {
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        if ((*it)->getID() == id_maq) {
            delete *it;  // Libera a mem�ria alocada pela m�quina
            LM.erase(it); // Remove a m�quina da lista
            cout << "M�quina removida." << endl;
            return true;
        }
    }

    cout << "M�quina n�o encontrada." << endl;
    return false;
}

bool Casino::editarMaquina(int id_maq){
    /*cout << "Dados maquina" << id_maq << endl;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        if ((*it)->getID() == id_maq) {
            cout << "|Nome Maquina: " << (*it)->getNome() <<"| " << end;
            cout << " |X: " << (*it)->getX() <<"| " << end;
            cout << "Probabilidade de Usu�rios: " << probabilidadeUser << endl;
            cout << "Hora de Abertura: " << horaAbertura << endl;
            cout << "Hora de Encerramento: " << horaFecho << endl;
        }
    }

    cout << "M�quina n�o encontrada." << endl;*/
    return false;
}

// Fim Crud Maquinas
