#ifndef MBLACKJACK_H
#define MBLACKJACK_H

#include <Maquina.h>
class Casino;


class MBlackJack : public Maquina
{

    public:
        MBlackJack(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino);
        virtual ~MBlackJack();

    protected:

    private:
};

#endif // MBLACKJACK_H
