#ifndef MAQUINA_H
#define MAQUINA_H
#include <iostream>
using namespace std;
#include <string>
#include <list>

class Casino;

enum estadoMaquina{ON, OFF, AVARIADA};

class Maquina
{
    int nMaquina;
    string nome;
    string x;
    string y;
    int ganhos;
    int prob;

    bool estado;
    int temperaturaSensor;

    Casino *Ptr_Casino;

    bool verificaEstado();
    void Run();
    public:
        Maquina(int _nM, int _prob, string _nome);
        virtual ~Maquina();

    protected:

    private:
};

#endif // MAQUINA_H
