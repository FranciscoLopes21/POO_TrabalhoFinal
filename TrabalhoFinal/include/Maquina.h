#ifndef MAQUINA_H
#define MAQUINA_H
#include <iostream>
using namespace std;
#include <string>
#include <list>

class Casino;

class Maquina
{
    string nome;
    string tipo;
    string x;
    string y;
    int ganhos;

    bool estado;
    int temperaturaSensor;

    Casino *Ptr_Casino;

    bool verificaEstado();
    void Run();
    public:
        Maquina();
        virtual ~Maquina();

    protected:

    private:
};

#endif // MAQUINA_H
