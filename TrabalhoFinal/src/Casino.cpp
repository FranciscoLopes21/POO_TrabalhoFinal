#include "Casino.h"
#include <conio.h>

#include <iostream>
using namespace std;
#include <string>
#include <list>

/*TODO
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

//Fun��es

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

void Casino::Menu(){

    int op = 0;

    do {
        // code block to be executed
        cout<< "Menu" <<endl;
        cout<< "1- Maquinas Avariadas" <<endl;
        cout<< "2- Maquina mais ganhos" <<endl;
        cout<< "3- Reparar" <<endl;
        cout<< "4- Registar Maquina" <<endl;

        cin >> op;

        switch(op){

        case 1:
            cout<< "Maquinas Avariadas" <<endl;
            //maquinaAvariada();
            break;
        case 2:
            cout<< "Maquina mais ganhos" <<endl;
            //maquinaGanhos();
            break;
        case 3:
            cout<< "Reparar" <<endl;
            //reparar();
            break;
        case 4:
            cout<< "Registar Maquina" <<endl;
            //registarMaquina();
            break;
        case 0:
            break;

        }
    }
    while (op != 0);

}

void Casino::maquinaAvariada(){


}
void Casino::maquinaGanhos(){


}
void Casino::reparar(){


}
void Casino::registarMaquina(){


}


//Outras Fun��es

//getNome
string Casino::getNome(){
    return nome;
}
