#include "Maquina.h"

Maquina::Maquina(int _nM, string _nome, string _x, string _y, int _premio, float _prob, string _tipo)
    : nMaquina(_nM), nome(_nome), x(_x), y(_y), premio(_premio), prob(_prob), tipo(_tipo), estado(estadoMaquina::ON), temperaturaSensor(0), Ptr_Casino(nullptr) {
    // Outras inicializações e lógica do construtor, se necessário
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
    cout << "Eu Máquina: " << 1 << " Estou ligada" << endl;
    //a temperatura aumenta quando liga e vai aumentado se estiver a ser utilizada
    temperaturaSensor += 1;
    if (verificaEstado())
    {
        cout << "Estou Avariado---> AVISAR Gestor------------" << endl;
        //Casino->TenhoAvariado(this);
    }

}




bool Maquina::verificaEstado()
{
    //estado = "bom";
    return true;
}

void Maquina::Desligar() {
    cout << "CHEGOU" << endl;
    estado = OFF; // Altera o estado da máquina para OFF
}

