#ifndef MROLETA_H
#define MROLETA_H

#include <Maquina.h>

class MRoleta : public Maquina
{
    int nMaquina;
    int prob;
    public:
        MRoleta(int _nM, int _prob);
        virtual ~MRoleta();

    protected:

    private:
};

#endif // MROLETA_H
