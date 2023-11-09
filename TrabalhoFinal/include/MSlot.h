#ifndef MSLOT_H
#define MSLOT_H

#include <Maquina.h>

class MSlot : public Maquina
{
    int nMaquina;
    int prob;
    public:
        MSlot(int _nM, int _prob);
        virtual ~MSlot();

        void setNMaquina(int nM);

    protected:

    private:
};

#endif // MSLOT_H
