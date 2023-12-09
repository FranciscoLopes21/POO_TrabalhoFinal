#ifndef MPOKER_H
#define MPOKER_H

#include <Maquina.h>


class MPoker : public Maquina
{

    public:
        MPoker(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta);
        virtual ~MPoker();

    protected:

    private:
};

#endif // MPOKER_H
