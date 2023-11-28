#include "MSlot.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

MSlot::MSlot(int _id, string _nome, int _x, int _y, int _premio, float _prob, string _tipo)
    : Maquina(_id, _nome, _x, _y, _premio, _prob, _tipo)
{
    //ctor
}

MSlot::~MSlot()
{
    //dtor
}

void MSlot::Run() {

    Maquina::Run();  // Chama o método Run da classe base se necessário

    if(getEstado() == ON){
        temperaturaSensor += 0.15;
        cout << "MSlot - Temperatura aumentada: " << temperaturaSensor << "   " << getID() << endl;
    }

    //Maquina::Run();

}




