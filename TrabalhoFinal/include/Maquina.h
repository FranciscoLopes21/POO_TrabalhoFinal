#ifndef MAQUINA_H
#define MAQUINA_H
#include <iostream>
#include <string>
#include <list>
#include "User.h"

using namespace std;
enum estadoMaquina{ON, OFF, AVARIADA};

class Casino;
class Maquina
{

    int idMaquina, x, y, aposta, nAvarias, nJogos;
    float premio, prob, temperaturaSensor;
    string nome, tipo;
    bool quente;
    estadoMaquina estado;
    list<User *> filaEspera;
    list<Maquina*> vizinhos;
    Casino* casino;
    User *userAtual;

    public:
        Maquina(int _idM, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino);
        virtual ~Maquina();
        virtual void Run();

        void informacaoMaquina();
        void Ligar();
        bool Desligar();
        void adicionarVizinho(Maquina* vizinho);
        void saemTodos();
        void avariaMaquina();
        bool removerVizinho(int id_maq);
        bool removerVizinhoTodos();
        void entrarFilaEspera(User* user);
        void associarUser(User *user);
        void removerUsersFilaEspera(User* user) {filaEspera.remove(user);}
        bool repararMaquina();
        void rodadas(User *user);
        void userSaiu();
        int Memoria();

        //get
        int getID(){ return idMaquina;}
        int getX(){ return x;}
        int getY(){ return y;}
        int getAposta(){return aposta;}
        int getnAvarias(){return nAvarias;}
        int getNJogos(){return nJogos;}
        float getPremio(){ return premio;}
        float getProb(){ return prob;}
        float getTemperaturaSensor(){return temperaturaSensor;}
        string getTipo(){ return tipo;}
        string getNome(){ return nome;}
        bool getQuente(){return quente;}
        list<User *> getFilaEspera(){return filaEspera;}
        list<Maquina*> getVizinhos(){return vizinhos;}
        estadoMaquina getEstado(){return estado;}
        User * getUserAtual(){return userAtual;}

        //set
        void setID(int _idMaquina){idMaquina = _idMaquina;}
        void setX(int _x){ x = _x;}
        void setY(int _y){ y = _y;}
        void setAposta(int _aposta){aposta = _aposta;}
        void setnAvarias(int _nAvarias){nAvarias = _nAvarias;}
        void setNJogos(int _nJogos){nJogos=_nJogos;}
        void setPremio(float _premio){ premio = _premio;}
        void setProb(float _prob){ prob = _prob ;}
        void setTemperaturaSensor(float _temperaturaSensor){temperaturaSensor = _temperaturaSensor;}
        void setTipo(string _tipo){ tipo = _tipo;}
        void setNome(string _nome){ nome = _nome;}
        void setQuente(bool _quente){ quente=_quente;}
        void setEstado(estadoMaquina _estado){estado = _estado;}
        void setUserAtual(User *user){userAtual = user;}

    private:
        bool verificaEstado();
        void subirProbabilidade();

    protected:

};
#endif // MAQUINA_H
