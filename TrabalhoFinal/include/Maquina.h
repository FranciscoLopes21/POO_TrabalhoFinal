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
    int x;
    int y;
    int premio;
    float prob;
    estadoMaquina estado;
    int temperaturaSensor;
    string tipo;
    Casino *Ptr_Casino;

    bool verificaEstado();

    void Run();
    public:
        Maquina(int _nM, string _nome, int _x, int _y, int _premio, float _prob, string _tipo);
        virtual ~Maquina();
        virtual void DisplayInfo() const;
        void Desligar();
        int getID(){ return nMaquina;}
        int getProb(){ return prob;}
        estadoMaquina getEstado(){return estado;}

    protected:

    private:
};

#endif // MAQUINA_H
