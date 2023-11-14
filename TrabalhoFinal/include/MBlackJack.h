#ifndef MBLACKJACK_H
#define MBLACKJACK_H

#include <Maquina.h>


class MBlackJack : public Maquina
{

    public:
        MBlackJack(int _nM, const std::string& _nome, const std::string& _x, const std::string& _y, int _premio, float _prob, const std::string& _tipo);
        virtual ~MBlackJack();

    protected:

    private:
};

#endif // MBLACKJACK_H
