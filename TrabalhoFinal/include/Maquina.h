#ifndef MAQUINA_H
#define MAQUINA_H
#include <iostream>
using namespace std;
#include <string>
#include <list>

#include "User.h"

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
    int aposta;

    bool utilizacao;


    string tipo;

    //Casino *Ptr_Casino;

    bool verificaEstado();

    int nAvarias;

    User *user;


    public:
        Maquina(int _nM, string _nome, int _x, int _y, int _premio, float _prob, string _tipo);
        virtual ~Maquina();
        virtual void Run();
        virtual void DisplayInfo() const;
        void Desligar();
        void Ligar();
        void avariaMaquina();


        int getID(){ return nMaquina;}

        int getPremio(){ return premio;}
        int setPremio(int _premio){ premio = _premio;}

        float getProb(){ return prob;}
        float setProb(float _prob){ prob = _prob ;}

        string getTipo(){ return tipo;}
        string setTipo(string _tipo){ tipo = _tipo;}

        string getNome(){ return nome;}
        string setNome(string _nome){ nome = _nome;}

        int getY(){ return y;}
        int setY(int _y){ y = _y;}

        int getX(){ return x;}
        int setX(int _x){ x = _x;}

        int getnAvarias(){return nAvarias;}
        int setnAvarias(int _nAvarias){nAvarias = _nAvarias;}

        estadoMaquina getEstado(){return estado;}


    protected:
        float temperaturaSensor;
        estadoMaquina estado;


    private:
};

#endif // MAQUINA_H
