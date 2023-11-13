#include "Casino.h"
#include <conio.h>

#include <iostream>
using namespace std;
#include <string>
#include <list>
#include "pugixml.hpp"

/*TODO
    Casino::Casino (string nome) *
    Fazer função de reparar máquina
    Listar Maquinas avariadas
    Listar maquina com mais ganhos
    Fazer função de registar xml
    Fazer função de criar maquina
    Melhorar função run()
    Fazer função estouAvaridado
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
    cout << "Máximo de Jogadores: " << maxJogadores << endl;
    cout << "Probabilidade de Usuários: " << probabilidadeUser << endl;
    cout << "Hora de Abertura: " << horaAbertura << endl;
    cout << "Hora de Encerramento: " << horaFecho << endl;
}

//Funções

void Casino::Run(){

    int ciclo =1;
    char key;
    while(true){

        if(_kbhit){
            key = _getch();
            if(key =='m' || key == 'M'){

                //openmenu();
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

<<<<<<< Updated upstream
=======
void Casino::Desligar(int id_maq) {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if((*it)->getID() == id_maq){
            (*it)->Desligar();
            cout << "Máquina desligada." << endl;
            return;
        }

    }
    cout << "Máquina não encontrada." << endl;
}

estadoMaquina Casino::Get_Estado(int id_maq) {

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if((*it)->getID() == id_maq){
            return (*it)->getEstado();
        }

    }
    cout << "Máquina não encontrada." << endl;
    return estadoMaquina::OFF;
}


>>>>>>> Stashed changes
void Casino::Menu(){

    int op = 0;

    do {
        // code block to be executed
        cout<< "Menu" <<endl;
        cout<< "1- Dados Casino" <<endl;
        cout<< "2- Adicionar Máquina" <<endl;
        cout<< "3- Maquinas Avariadas" <<endl;
        cout<< "4- Maquina mais ganhos" <<endl;
        cout<< "5- Reparar" <<endl;

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
<<<<<<< Updated upstream
=======
        case 7:
            cout<< "Desligar Máquina" <<endl;
            int nMaquina;
            cin >> nMaquina;
            Desligar(nMaquina);
            break;
        case 8:
            cout << "Pesquisar máquina" << endl;
            int idMaquina;
            cin >> idMaquina;
            estadoString(Get_Estado(idMaquina));
            break; // Este break é necessário para corrigir o erro
        case 9:
            break; // Este break é necessário para corrigir o erro
>>>>>>> Stashed changes
        case 0:
            break;

        }
    }
    while (op != 9);

}

<<<<<<< Updated upstream
=======
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

    cout  << " | Estado: " << estadoString << endl;
}


>>>>>>> Stashed changes
void Casino::maquinaAvariada(){


}
void Casino::maquinaGanhos(){


}
void Casino::reparar(){


}
void Casino::registarMaquina(){


}


//Outras Funções

//getNome
string Casino::getNome(){
    return nome;
}
