#include "Casino.h"
#include <conio.h>

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

void Casino::dadosCasino() {
    cout << "Nome do Casino: " << nome << endl;
    cout << "M�ximo de Jogadores: " << maxJogadores << endl;
    cout << "Probabilidade de Usu�rios: " << probabilidadeUser << endl;
    cout << "Hora de Abertura: " << horaAbertura << endl;
    cout << "Hora de Encerramento: " << horaFecho << endl;
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


//Fun��es

void Casino::Run(){

    int ciclo =1;
    char key;
    while(true){
        cout<< "Menu" <<endl;
        if (_kbhit()) { // Corre��o: Chame _kbhit como uma fun��o
            key = _getch();
            if (key == 'm' || key == 'M') {
                Menu();
            }
        }
            //corre o resto do programa
    }

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
        }
    }

}


void Casino::Menu(){

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
        case 0:
            break;

        default:
            cout << "Op��o inv�lida. Tente novamente." << endl;
        }
    }
    while (op != 20);

}
void Casino::ListarMaquinasProbabilidade() {
    cout << "Listar m�quinas com probabilidade de maior que x" << endl;
    float xProbabilidade;
    cin >> xProbabilidade;
    ofstream F("ListaProbX.txt");
    Listar(xProbabilidade, F);
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


//Outras Fun��es

//getNome
string Casino::getNome(){
    return nome;
}
