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
    int premio;
    int prob;
    estadoMaquina estado;
    int temperaturaSensor;
    string tipo;
    Casino *Ptr_Casino;

    bool verificaEstado();

    void Run();
    public:
        Maquina(int _nM, string _nome, string _x, string _y, int _premio, int _prob, string _tipo);
        virtual ~Maquina();
        virtual void DisplayInfo() const;
        void Desligar();
        int getID(){ return nMaquina;}
        estadoMaquina getEstado(){return estado;}

    protected:

    private:
};

#endif // MAQUINA_H
