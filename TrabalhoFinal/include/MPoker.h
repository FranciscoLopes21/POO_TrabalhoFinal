#ifndef MPOKER_H
#define MPOKER_H

#include <Maquina.h>


class MPoker : public Maquina
{

    public:
        MPoker(int _id, string _nome, int _x, int _y, int _premio, float _prob, string _tipo);
        virtual ~MPoker();

    protected:

    private:
};

#endif // MPOKER_H
