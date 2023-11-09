#include "Maquina.h"

Maquina::Maquina(int _nM, int _prob, string _nome)
{
    //ctor
    temperaturaSensor = 0;
    nMaquina = _nM;
    prob = _prob;
    nome = _nome;
}

Maquina::~Maquina()
{
    //dtor
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
    return estado = "bom";
}

