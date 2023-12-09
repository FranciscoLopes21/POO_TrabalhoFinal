#ifndef MSLOT_H
#define MSLOT_H

#include <Maquina.h>


class MSlot : public Maquina
{

    public:
        MSlot(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta);
        virtual ~MSlot();
        void Run(); // Override do m�todo Run da classe base

        void setNMaquina(int nM);

    protected:

    private:
};

#endif // MSLOT_H
