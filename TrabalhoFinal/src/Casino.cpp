#include "Casino.h"
#include <conio.h>
Casino::Casino()
{
    //ctor
}

Casino::~Casino()
{
    //dtor
}



void Casino::Run(){

    char key;
    while(true){
            cout << "Hello world!" << endl;
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

    char op = 0;

    do {
        // code block to be executed
        cout<< "Menu" <<endl;
        cout<< "1- asfdf" <<endl;
        cout<< "1- asfdf" <<endl;
        cout<< "1- asfdf" <<endl;
        cout<< "o- Sair" <<endl;
    }
    while (op != 0);

}
