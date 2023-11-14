#ifndef MSLOT_H
#define MSLOT_H

#include <Maquina.h>


class MSlot : public Maquina
{

    public:
        MSlot(int _id, const std::string& _nome, int _x, int _y, int _premio, float _prob, const std::string& _tipo);
        virtual ~MSlot();

        void setNMaquina(int nM);

    protected:

    private:
};

#endif // MSLOT_H
