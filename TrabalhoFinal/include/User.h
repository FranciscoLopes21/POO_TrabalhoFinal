#ifndef USER_H
#define USER_H
#include <iostream>
using namespace std;
#include <string>

class User
{
    int nUser;
    string nome;
    string morada;
    char idade;

    float carteira;
    float ganhos;

    public:
        User(int _nUser, string _nome, string _morada, char _idade);
        virtual ~User();

        void Run();

    protected:

    private:
};

#endif // USER_H
