#ifndef CASINO_H
#define CASINO_H
#include <iostream>
using namespace std;
#include <string>
#include <list>

#include "Maquina.h"

class Casino
{
    string nome;
    list<Maquina *> LM;
    list<Maquina *> LM_Avariadas;
    list<int> Lt;


    public:
        Casino(string _nome = "Casino Solverde");
        virtual ~Casino();
        string getNome();
        void Run();
        void Menu();
        void maquinaAvariada();
        void maquinaGanhos();
        void reparar();
        void registarMaquina();

    protected:

    private:
};

#endif // CASINO_H
