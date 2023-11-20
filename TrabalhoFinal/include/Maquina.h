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


    public:
        Maquina(int _nM, string _nome, int _x, int _y, int _premio, float _prob, string _tipo);
        virtual ~Maquina();
        void Run();
        virtual void DisplayInfo() const;
        void Desligar();
        void Ligar();
        int getID(){ return nMaquina;}
        float getProb(){ return prob;}
        string getTipo(){ return tipo;}
        string getNome(){ return nome;}
        int getY(){ return y;}
        int getX(){ return x;}
        estadoMaquina getEstado(){return estado;}

    protected:

    private:
};

#endif // MAQUINA_H
