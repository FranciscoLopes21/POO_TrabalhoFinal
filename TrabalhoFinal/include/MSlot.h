#ifndef MSLOT_H
#define MSLOT_H

#include <Maquina.h>
class Casino;


class MSlot : public Maquina
{

    public:
        MSlot(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino);
        virtual ~MSlot();
        void Run(); // Override do método Run da classe base

        void setNMaquina(int nM);

    protected:

    private:
};

#endif // MSLOT_H
