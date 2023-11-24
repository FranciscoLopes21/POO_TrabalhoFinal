#include "Maquina.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Maquina::Maquina(int _nM, string _nome, int _x, int _y, int _premio, float _prob, string _tipo) {

    nMaquina = _nM;
    nome = _nome;
    x = _x;
    y = _y;
    premio = _premio;
    prob = _prob;
    tipo = _tipo;


    estado = OFF;
    temperaturaSensor = 0;
    utilizacao = false;

}

Maquina::~Maquina()
{
    //dtor
}

void Maquina::DisplayInfo() const {
    string estadoString;
    switch (estado) {
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

    std::cout << "ID: " << nMaquina << " | Nome: " << nome << " | Posição: (" << x << ", " << y << ") | Prêmio: " << premio << " | ProbG: " << prob << " | Estado: " << estadoString << std::endl;
}

void Maquina::Run(){
    cout << "Eu Máquina: " << nMaquina << " Estou ligada" << endl;
    //a temperatura aumenta quando liga e vai aumentado se estiver a ser utilizada
    //temperaturaSensor += 1;

    verificaEstado();


}




bool Maquina::verificaEstado()
{

    //estado = "bom";
    if(estado==ON){
        cout << "\033[1;32mMaquina Ligada\033[0m"<< nMaquina << "\n";
    }else if(estado==OFF){
        cout << "\033[1;33mMaquina Desligada\033[0m\n";
    }else if(estado==AVARIADA){
        cout << "\033[1;31mMaquina Avariada\033[0m\n";
    }
    return true;
}

void Maquina::Desligar() {
    estado = OFF; // Altera o estado da máquina para OFF
}

void Maquina::Ligar() {
    estado = ON; // Altera o estado da máquina para ON
}

void Maquina::avariaMaquina(){
    estado = AVARIADA;
}

