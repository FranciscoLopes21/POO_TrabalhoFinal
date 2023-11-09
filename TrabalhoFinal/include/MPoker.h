#ifndef MPOKER_H
#define MPOKER_H

#include <Maquina.h>

class MPoker : public Maquina
{
    int nMaquina;
    int prob;
    public:
        MPoker(int _nM, int _prob);
        virtual ~MPoker();

    protected:

    private:
};

#endif // MPOKER_H
