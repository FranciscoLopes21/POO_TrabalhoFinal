#ifndef CASINO_H
#define CASINO_H
#include <iostream>
using namespace std;
#include <string>
#include <list>

#include "Maquina.h"
#include "User.h"

class Casino
{
    string nome;
    int maxJogadores;
    int probabilidadeUser;
    int horaAbertura;
    int horaFecho;
    list<Maquina *> LM;
    list<Maquina *> LM_Avariadas;
    list<User *> LU;
    list<int> Lt;


    public:
        Casino(string _nome);
        void CarregarDados(int _maxJogadores, int _probabilidadeUser,int _horaAbertura, int _horaFecho);
        void dadosCasino();
        virtual ~Casino();
        string getNome();
        void Run();
        bool Add(Maquina *M);
        void Menu();
        void maquinaAvariada();
        void maquinaGanhos();
        void reparar();
        void registarMaquina();

    protected:

    private:
};

#endif // CASINO_H
