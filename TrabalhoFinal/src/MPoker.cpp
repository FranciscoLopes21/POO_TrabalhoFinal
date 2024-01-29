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

void MPoker::Run() {
    float temp;
    Maquina::Run();  // Chama o método Run da classe base se necessário
    if(getEstado() == ON){
            if(getUserAtual() != nullptr){
                temp = getTemperaturaSensor();
                setTemperaturaSensor(temp += 0.05) ;
                cout << "Aumentoooooooooooooooooooooooooo" << endl;
                if(getTemperaturaSensor() >= 35.00){
                    cout << "A maquina " << getID() << " esta a sobreaquecida: " << getTemperaturaSensor() << endl;
                    setQuente(true);
                    setEstado(AVARIADA);
                } //Avidar que maquina que vai desligar, retirar user e desligar ate x grau e ligar denovo
                cout << "MSlot - Temperatura aumentada: " << getTemperaturaSensor() << "   " << getID() << endl;
            }else if(getUserAtual() == nullptr){
                temp = getTemperaturaSensor();
                setTemperaturaSensor(temp += 0.1) ;
                cout << "Aumentoooooooooooooooooooooooooo" << endl;
                if(getTemperaturaSensor() >= 35.00){
                    cout << "A maquina " << getID() << " esta a sobreaquecida: " << getTemperaturaSensor() << endl;
                    setQuente(true);
                    setEstado(AVARIADA);
                } //Avidar que maquina que vai desligar, retirar user e desligar ate x grau e ligar denovo
                cout << "MSlot - Temperatura aumentada: " << getTemperaturaSensor() << "   " << getID() << endl;
            }
    }
    else if(getEstado() == AVARIADA){
        temp = getTemperaturaSensor();
        setTemperaturaSensor(temp - 0.50);  // Arrefecimento mais lento quando desligada
        if (getTemperaturaSensor() <= 25.0 && getQuente() == true) {  //para verifica se a avaria é de quente ou se esta avariada
            setEstado(ON);
            setQuente(false);
            cout << "A máquina " << getID() << " foi ligada novamente." << endl;
        }
        cout << "Máquina - Temperatura diminuída: " << getTemperaturaSensor() << "°C   " << getID() << endl;
    }
    else if(getEstado() == OFF){
        temp = getTemperaturaSensor();
        setTemperaturaSensor(temp - 0.90);  // Arrefecimento mais lento quando desligada
        if (getTemperaturaSensor() <= 25.0) {  // Valor mais realista para arrefecimento
            setEstado(ON); //para ajudar a manter a temperatura da maquina
            setQuente(false);
            cout << "A máquina " << getID() << " foi ligada novamente." << endl;
        }
        cout << "Máquina - Temperatura diminuída: " << getTemperaturaSensor() << "°C   " << getID() << endl;

    }
}



