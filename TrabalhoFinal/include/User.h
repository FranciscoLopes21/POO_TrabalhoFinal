#ifndef USER_H
#define USER_H
#include <iostream>
using namespace std;
#include <string>

class Maquina;
class Casino;

class User
{
    int nUser;
    string nome;
    string morada;
    int idade;

    float carteira;
    float ganhos;
    int jogadas;

    bool aJogar;

    Maquina * maquinaAssociada;

    Casino* casino;

    public:
        User(int _nUser, string _nome, string _morada, int _idade, Casino* _casino);
        virtual ~User();

        void Run();
        void associarMaquina(Maquina* maquina);
        void entrarFilaEspera(Maquina* maquina);
        void jogarNaMaquina();
        void userSaiCasino();

        int getNUser(){ return nUser;}
        int setNUser(int _nUser){ nUser = _nUser ;}

        string getNome(){ return nome;}
        string setNome(string _nome){ nome = _nome ;}

        float getCarteira(){ return carteira;}
        float setCarteira(float _carteira){ carteira = _carteira ;}

        int getJogadas(){return jogadas;}
        int setJogadas(int _jogadas){ jogadas = _jogadas ;}


        float getGanhos(){return ganhos;}
        float setGanhos(int _ganhos){ ganhos = _ganhos ;}

        bool getAJogar(){return aJogar;}
        bool setAJogar(bool _aJogar){aJogar = _aJogar;}

        Maquina * getMaquinaAssociada(){return maquinaAssociada;}
        Maquina * setMaquinaAssociada(Maquina *maquina){maquinaAssociada=maquina;}




    protected:

    private:
};

#endif // USER_H
