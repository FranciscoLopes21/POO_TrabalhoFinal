#ifndef MBLACKJACK_H
#define MBLACKJACK_H

#include <Maquina.h>

class MBlackJack : public Maquina
{
    int nMaquina;
    int prob;
    public:
        MBlackJack(int _nM, int _prob);
        virtual ~MBlackJack();

    protected:

    private:
};

#endif // MBLACKJACK_H
