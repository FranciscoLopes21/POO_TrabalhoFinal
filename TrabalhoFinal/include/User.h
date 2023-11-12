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
    int carteira;
    float ganhos;

    public:
        User();
        virtual ~User();

    protected:

    private:
};

#endif // USER_H
