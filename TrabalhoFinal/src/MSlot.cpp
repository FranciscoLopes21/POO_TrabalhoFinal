#include "MSlot.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Casino.h"

MSlot::MSlot(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino)
    : Maquina(_id, _nome, _x, _y, _premio, _prob, _tipo, _aposta, _casino)
{
    //ctor
}

MSlot::~MSlot()
{
    //dtor
}

void MSlot::Run() {

    Maquina::Run();  // Chama o m�todo Run da classe base se necess�rio

    if(getEstado() == ON){
            if(getUserAtual() != nullptr){
                float temp = getTemperaturaSensor();
                setTemperaturaSensor(temp += 0.5) ;
                if(getTemperaturaSensor() >= 78.00){
                    cout << "A maquina " << getID() << " esta a sobreaquecer: " << getTemperaturaSensor() << endl;
                    estado = AVARIADA;
                } //Avidar que maquina que vai desligar, retirar user e desligar ate x grau e ligar denovo
                cout << "MSlot - Temperatura aumentada: " << getTemperaturaSensor() << "   " << getID() << endl;
            }else if(getUserAtual() == nullptr){
                float temp = getTemperaturaSensor();
                setTemperaturaSensor(temp += 0.1) ;
                if(getTemperaturaSensor() >= 78.00){
                    cout << "A maquina " << getID() << " esta a sobreaquecer: " << getTemperaturaSensor() << endl;
                    estado = AVARIADA;
                } //Avidar que maquina que vai desligar, retirar user e desligar ate x grau e ligar denovo
                cout << "MSlot - Temperatura aumentada: " << getTemperaturaSensor() << "   " << getID() << endl;
            }
    }

    //Maquina::Run();

}




