#include "MPoker.h"
#include "Casino.h"

MPoker::MPoker(int _id, string _nome, int _x, int _y, float _premio, float _prob, string _tipo, int _aposta, Casino* _casino)
    : Maquina(_id, _nome, _x, _y, _premio, _prob, _tipo, _aposta, _casino)
{
    //ctor
}

MPoker::~MPoker()
{
    //dtor
}

//Metodo Run
void MPoker::Run() {
    float temp;
    Maquina::Run(); //Chama metodo Run da classe base
    if(getEstado() == ON){ //Se maquina estiver ON
            if(getUserAtual() == nullptr){ //Se maquina estiver livre
                temp = getTemperaturaSensor();
                setTemperaturaSensor(temp += 0.05) ; //Aumenta 0.05 a temperatura
                cout << "Temperatura aumentou " << 0.05 << endl;
                if(getTemperaturaSensor() >= 35.00){ //Se temperatura foi maior ou igual a 35.00
                    cout << "A maquina " << getID() << " esta a sobreaquecida: " << getTemperaturaSensor() << endl; //Printa aviso
                    setQuente(true);
                    setEstado(AVARIADA); //Troca estado para AVARIADA
                }
                cout << "Temperatura: " << getTemperaturaSensor() << endl;
            }else if(getUserAtual() != nullptr){ //Se maquina ocupada
                temp = getTemperaturaSensor();
                setTemperaturaSensor(temp += 0.1) ; //Aumenta 0.1 a temperatura
                cout << "Temperatura aumentou " << 0.1 << endl;
                if(getTemperaturaSensor() >= 35.00){ //Se temperatura foi maior ou igual a 35.00
                    cout << "A maquina " << getID() << " esta a sobreaquecida: " << getTemperaturaSensor() << endl;
                    setQuente(true);
                    setEstado(AVARIADA); //Troca estado para AVARIADA
                }
                cout << "Temperatura: " << getTemperaturaSensor() << endl;
            }
    }
    else if(getEstado() == AVARIADA){ //Se maquina estiver AVARIADA
        temp = getTemperaturaSensor();
        setTemperaturaSensor(temp - 0.50);  //Arrefecimento 0.50
        cout << "Temperatura diminuida " << 0.50 << endl;
        if (getTemperaturaSensor() <= 25.0 && getQuente() == true) {  //Verifica se a avaria é de quente ou se esta avariada
            setEstado(ON); //Troca estado para ON
            setQuente(false);
            cout << "A máquina " << getID() << " foi ligada novamente." << endl;
        }
        cout << "Temperatura: " << getTemperaturaSensor() << "°C" << endl;
    }
    else if(getEstado() == OFF){ //Se maquina estiver OFF
        temp = getTemperaturaSensor();
        setTemperaturaSensor(temp - 0.90);  //Arrefecimento 0.90
        if (getTemperaturaSensor() <= 25.0) {  //Verifica se temperatura é menor ou igual a 25.00
            setEstado(ON); //Troca estado para ON, para manter temperatura
            setQuente(false);
            cout << "A máquina " << getID() << " foi ligada novamente." << endl;
        }
        cout << "Temperatura: " << getTemperaturaSensor() << "°C" << endl;
    }
}

//Calcular memoria
int MPoker::Memoria() {
    //Obtem memoria da classe base Maquina
    int mem = Maquina::Memoria();
    return mem;
}



