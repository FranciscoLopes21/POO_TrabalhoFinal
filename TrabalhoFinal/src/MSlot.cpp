#include "MSlot.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

MSlot::MSlot(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta)
    : Maquina(_id, _nome, _x, _y, _premio, _prob, _tipo, _aposta)
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
        temperaturaSensor += 0.5;
        if(temperaturaSensor >= 78.00){
            cout << "A maquina " << getID() << " esta a sobreaquecer: " << temperaturaSensor << endl;
            estado = AVARIADA;
        } //Avidar que maquina que vai desligar, retirar user e desligar ate x grau e ligar denovo
        cout << "MSlot - Temperatura aumentada: " << temperaturaSensor << "   " << getID() << endl;
    }

    //Maquina::Run();

}




