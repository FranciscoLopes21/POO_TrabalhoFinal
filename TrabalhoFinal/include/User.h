#ifndef USER_H
#define USER_H
#include <iostream>
using namespace std;
#include <string>

class Maquina;

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

    public:
        User(int _nUser, string _nome, string _morada, int _idade);
        virtual ~User();

        void Run();
        void associarMaquina(Maquina* maquina);
        void entrarFilaEspera(Maquina* maquina);

        string getNome(){ return nome;}
        string setNome(string _nome){ nome = _nome ;}

        float getCarteira(){ return carteira;}
        float setCarteira(float _carteira){ carteira = _carteira ;}

        int getJogadas(){return jogadas;}
        int setJogadas(int _jogadas){ jogadas = _jogadas ;}



    protected:

    private:
};

#endif // USER_H
