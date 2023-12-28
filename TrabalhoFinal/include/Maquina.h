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
    float premio;
    float prob;
    float temperaturaSensor;
    int aposta;

    //bool utilizacao;
    list<User *> filaEspera;
    User *userAtual;


    string tipo;

    //Casino *Ptr_Casino;

    bool verificaEstado();

    int nAvarias;

    int nJogos;


    public:
        Maquina(int _nM, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta);
        virtual ~Maquina();
        virtual void Run();
        virtual void DisplayInfo() const;
        void Desligar();
        void Ligar();
        void avariaMaquina();
        void entrarFilaEspera(User* user);
        void associarUser(User *user);
        void rodadas(User *user);
        void userSaiu();


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

        //bool getUtilizacao(){return utilizacao;}
        //bool setUtilizacao(bool _utilizacao){utilizacao = _utilizacao;}

        int getAposta(){return aposta;}
        int setAposta(int _aposta){aposta = _aposta;}

        int getNJogos(){return nJogos;}
        int setNJogos(int _nJogos){nJogos=_nJogos;}

        float getTemperaturaSensor(){return temperaturaSensor;}
        float setTemperaturaSensor(float _temperaturaSensor){temperaturaSensor = _temperaturaSensor;}

        bool repararMaquina();

        list<User *> getFilaEspera(){return filaEspera;}
        void removerUsuarioFilaEspera(User* user) {
            filaEspera.remove(user);
        }


        estadoMaquina getEstado(){return estado;}

        User * getUserAtual(){return userAtual;}
        User * setUserAtual(User *user){userAtual = user;}



    protected:

        estadoMaquina estado;


    private:
};

#endif // MAQUINA_H
