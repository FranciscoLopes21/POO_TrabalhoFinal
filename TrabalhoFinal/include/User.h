#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Maquina;
class Casino;
class User
{

    int nUser, idade, jogadas;
    float carteira, ganhos;
    string nome, morada;
    bool aJogar;
    time_t horaEntrada, horaSaida, tempoCasino;
    Maquina * maquinaAssociada;
    Casino* casino;

    public:
        User(int _nUser, string _nome, string _morada, int _idade, Casino* _casino);
        virtual ~User();
        void Run();

        void associarMaquina(Maquina* maquina);
        void entrarFilaEspera(Maquina* maquina);
        void userSaiCasino();
        int Memoria();
        void mostrarDados();

        //get
        int getNUser(){ return nUser;}
        int getJogadas(){return jogadas;}
        float getCarteira(){ return carteira;}
        float getGanhos(){return ganhos;}
        string getNome(){ return nome;}
        bool getAJogar(){return aJogar;}
        Maquina * getMaquinaAssociada(){return maquinaAssociada;}
        time_t getTempoCasino(){return tempoCasino;}

        //set
        void setNUser(int _nUser){ nUser = _nUser;}
        void setJogadas(int _jogadas){ jogadas = _jogadas ;}
        void setCarteira(float _carteira){ carteira = _carteira ;}
        void setGanhos(int _ganhos){ ganhos = _ganhos ;}
        void setNome(string _nome){ nome = _nome ;}
        void setAJogar(bool _aJogar){aJogar = _aJogar;}
        void setMaquinaAssociada(Maquina *maquina){maquinaAssociada=maquina;}

    private:
        void jogarNaMaquina();

    protected:

};
#endif // USER_H
