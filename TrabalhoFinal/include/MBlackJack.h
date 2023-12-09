#ifndef MBLACKJACK_H
#define MBLACKJACK_H

#include <Maquina.h>


class MBlackJack : public Maquina
{

    public:
        MBlackJack(int _id, string _nome, int _x, int _y, int _premio, float _prob, string _tipo, int _aposta);
        virtual ~MBlackJack();

    protected:

    private:
};

#endif // MBLACKJACK_H
