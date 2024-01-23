#include <iostream> // sleep function
#include <sstream>  // Inclua este cabeçalho para std::stringstream
#include <fstream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <cstdlib>  // Para a função sleep
#include <iomanip>
#include <random>
#include <cmath>
#include <cctype>
#include <ctime>
#include <string>
#include <list>

////////////////////////
#include "Casino.h"
#include "pugixml.hpp"
#include "XMLReader.h"
#include "MBlackJack.h"
#include "MPoker.h"
#include "MRoleta.h"
#include "MSlot.h"
#include "XMLWriter.h"
#include <windows.h> // Library effective with Windows

using namespace std;

/*TODO
    Casino::Casino (string nome) *
    Fazer função de reparar máquina
    Listar Maquinas avariadas
    Listar maquina com mais ganhos
    Fazer função de registar xml
    Fazer função de criar maquina
    Melhorar função run()
    Fazer função estouAvaridado
    Menu
    dadosCasino();
    //Add();
    //maquinaAvariada();
    ListMachines();
    //maquinaAvariada();
    //maquinaGanhos();
    //reparar();
    //registarMaquina();
    int nMaquina;
    cin >> nMaquina;
    Desligar(nMaquina);
    cout << "Pesquisar máquina" << endl;
    int idMaquina;
    cin >> idMaquina;
    estadoString(Get_Estado(idMaquina));
    ListarMaquinasProbabilidade();
    menuCrudMaquina();
*/
Casino::Casino(string _nome)
{
    //ctor
    nome = _nome;
}

Casino::~Casino()
{
    //dtor
    //destui Maquinas
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it)
           delete (*it);
}

void Casino::CarregarDados(int _maxJogadores, int _probabilidadeUser, int _horaAbertura, int _minutosAbertura, int _segundosAbertura,
                            int _horaFecho, int _minutosFecho, int _segundosFecho)
{

    maxJogadores = _maxJogadores;
    probabilidadeUser = _probabilidadeUser;
    horaAbertura = _horaAbertura;
    minutosAbertura = _minutosAbertura;
    segundosAbertura = _segundosAbertura;
    horaFecho = _horaFecho;
    minutosFecho = _minutosFecho;
    segundosFecho = _segundosFecho;

    jogadoresNoCasino=0;
    totalPremios=0;
    totalDinheiroDado=0;
}

void Casino::Listar(ostream &f){

    system("cls"); //limpa ecra

    cout << endl; //escreve nova linha
    cout << "Listar estado atual do Casino" << endl; //printa ação da função
    cout << endl; //escreve nova linha
    cout << "| ID Maquina | Nome Maquina | Probabilidade | Estado | temperatura |" << endl; //printa campos que vão ser mostrados

    f << "Estado Casino " << nome << endl; //escreve no ficheiro
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino

        //escreve no ficheiro os campos - id, nome, probabilidade, estado, temperatura
        f << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: " << estadoString((*it)->getEstado()) << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        if((*it)->getEstado()== ON){ //verifica se o estado é ON para ajustar a cor do estado quando printa
            //printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;32m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== OFF){ //verifica se o estado é ON para ajustar a cor do estado quando printa
            //printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;33m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }else if ((*it)->getEstado()== AVARIADA){ //verifica se o estado é ON para ajustar a cor do estado quando printa
            //printa dados
            cout << "| ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() <<" | Probabilidade: " << (*it)->getProb() << " | Estado: \033[1;31m"<<estadoString((*it)->getEstado())<<"\033[0m" << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl;
        }

    }

    cout << endl; //adiciona linha
    cout << "Jogadores no casino: " << jogadoresNoCasino << endl; //printa a quantidade de jogadores no casino atualmente
    cout << "Numero total de jogadores: " << LU.size() << endl; //printa a quantidade de jogadores que já passaram pelo casino
    cout << endl; //adiciona linha

}




bool Casino::LoadMachinesFromXML(const string& filename) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        cerr << "Erro ao carregar o arquivo XML: " << result.description() << endl;
        return false;
    }


    pugi::xml_node listaMaq = doc.child("DADOS").child("LISTA_MAQ");

    for (pugi::xml_node maquina = listaMaq.child("MAQUINA"); maquina; maquina = maquina.next_sibling("MAQUINA")) {
        int id = maquina.child("ID").text().as_int();
        string nome = maquina.child("NOME").text().get();
        int x = maquina.child("X").text().as_int();
        int y = maquina.child("Y").text().as_int();
        int premio = maquina.child("PREMIO").text().as_int();
        int probG = maquina.child("PROB_G").text().as_int();
        string tipo = maquina.child("TIPO").text().get();

        Maquina* m = nullptr;

        if (tipo == "slot") {
            m = new MSlot(id, nome, x, y, premio, probG, tipo,10,this);
        } else if (tipo == "poker") {
            m = new MPoker(id, nome, x, y, premio, probG, tipo,15,this);
        }

        // Adiciona a máquina ao vetor do Casino
        Add(m);

    }

    return true;
}


//Menu
void Casino::Menu(){

    int op = 0; //variavel que irá guardar a opção
    do {

        cout << "||||||||||||||||||||||" << endl; //printa menu
        cout << "|        Menu        |" << endl; //printa menu
        cout << "|   1- Dados Casino  |" << endl; //printa menu
        cout << "|  2- Gestão Casino  |" << endl; //printa menu
        cout << "| 3- Gestão Maquinas |" << endl; //printa menu
        cout << "|   4- Gestão User   |" << endl; //printa menu
        cout << "|  5- Memoria Total  |" << endl; //printa menu
        cout << "|       0- Sair      |" << endl; //printa menu
        cout << "||||||||||||||||||||||" << endl; //printa menu
        cout << endl; //adiciona linha
        cout << "Opção: "; //pergunta por opção desejada
        cin >> op; //guarda opção desejada

        switch(op){ //verifica opção

        case 1: //caso seja 1
            system("cls"); //limpa ecra
            cout<< "Dados Casino" <<endl; //printa opção
            dadosCasino(); //chama função que exibe dados sobre o casino
            break; //quebra verificação
        case 2: //caso seja 2
            system("cls"); //limpa ecrã
            cout<< "Gestão Casino" <<endl; //printa opção
            gestaoCasino(); //chama função gestaoCasino que irá exibir um menu para gerir casino
            break; //quebra verificação
        case 3: //caso seja 3
            system("cls"); //limpa ecrã
            cout<< "Gestão Maquinas" <<endl; //printa opção
            gestaoMaquinas(); //chama função gestaoCasino que irá exibir um menu para gerir maquinas
            break; //quebra verificação
        case 4:
            system("cls"); //limpa ecrã
            cout<< "Gestão User" <<endl; //printa opção
            gestaoUseres();
            break; //quebra verificação
        case 5:
            system("cls"); //limpa ecrã
            cout<< "Memoria Total" <<endl; //printa opção
            Memoria_Total(); //chama função gestaoCasino que irá exibir a memoria total usada pelo programa
            break; //quebra verificação
        case 0:
            break;

        default:
            cout << "Opção inválida. Tente novamente." << endl;
        }
    }
    while (op != 0);

}

//Menu Gestão Casino
void Casino::gestaoCasino(){

    int op = 0;
    string nome;
    ofstream F("estadoAtualCasino.txt");

    do {

        cout << "Gestão Casino" << endl;
        cout << "1- Listar estado atual casino" << endl;
        cout << "2- Relatorio" << endl;
        cout << "3- Listar maquinas com probabilidade superiora X" << endl;
        cout << "0- Sair" << endl;

        cout << endl;
        cout << "Opção: ";
        cin >> op;

        switch(op){

        case 1:
            cout<< "Listar estado atual casino" <<endl;
            Listar(F);
            break;
        case 2:
            int id_maq;
            cout<< "Relatorio" <<endl;
            nome = devolveData();
            Relatorio(nome);
            break;
        case 3:
            system("cls");
            cout<< "Listar maquinas com probabilidade superiora X" <<endl;
            ListarMaquinasProbabilidade();
            break;
        case 0:
            break;

        default:
            cout << "Opção inválida. Tente novamente." << endl;
        }
    }
    while (op != 0);
}// Fim Menu Gestão Casino

string Casino::devolveData(){

    time_t currentTime = time(nullptr); //obter o tempo atual
    struct tm * ptm = localtime(&currentTime); //converter a representação do tempo local
    char buffer[32]; //criar um buffer para armazenar a data formatada
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ptm); //formatar a data no estilo "YYYY-MM-DD" e armazenar no buffer
    string currentDate(buffer); //converter o buffer para uma string

    string dataHoje = "data_" + currentDate + ".xml"; //criar o nome do arquivo XML com base na data

    return dataHoje; //retornar o nome do arquivo

}

//Menu Gestão Maquinas
void Casino::gestaoMaquinas(){

    int op = 0;
    int idMaquina;

    do {

        cout << "Gestão Maquinas" << endl;
        cout << "1- Listar maquinas" << endl;
        cout << "2- Crud maquina" << endl;
        cout << "3- Desligar maquina" << endl;
        cout << "4- Estado maquina ID" << endl;
        cout << "5- Listar maquinas do tipo" << endl;
        cout << "6- Ranking mais fracos" << endl;
        cout << "7- Ranking mais trabalhadores" << endl;
        cout << "8- Maquinas avariadas" << endl;
        cout << "0- Sair" << endl;

        cout << endl;
        cout << "Opção: ";
        cin >> op;

        switch(op){

        case 1:
            system("cls");
            cout << "Listar maquinas" << endl;
            ListarMaquinas();
            break;
        case 2:
            system("cls");
            cout << "Crud maquina" << endl;
            menuCrudMaquina();
            break;
        case 3:
            cout<< "Desligar maquina" <<endl;
            cout<< "ID: ";
            cin >> idMaquina;
            Desligar(idMaquina);
            break;
        case 4:
            cout<< "Estado maquina ID" <<endl;
            cout<< "ID: ";
            cin >> idMaquina;
            cout  << " | Estado: " << estadoString(Get_Estado(idMaquina)) << endl;
            break;
        case 5:
            cout << "Listar maquinas do tipo" << endl;
            listarTipoMaquina();
            break;
        case 6:
            cout << "Ranking mais fracos" << endl;
            showRankingAvarias();
            break;
        case 7:
            cout << "Ranking mais trabalhadores" << endl;
            listarRankingMaisTrabalhadores();
            break;
        case 8:
            cout << "Maquinas avariadas" << endl;
            maquinaAvariada();
            break;
        case 0:
            break;

        default:
            cout << "Opção inválida. Tente novamente." << endl;
        }
    }
    while (op != 0);
}// Fim Menu Gestão Maquinas

//Menu Gestão User
void Casino::gestaoUseres(){

    int op = 0;

    do {

        cout << "Gestão Useres" << endl;
        cout << "1- Jogadores mais frequentes" << endl;
        cout << "2- Jogadores com mais ganhos" << endl;
        cout << "0- Sair" << endl;

        cout << endl;
        cout << "Opção: ";
        cin >> op;

        switch(op){

        case 1:
            system("cls");
            cout << "Jogadores mais frequentes" << endl;
            listarJogadoresMaisFrequentes();
            break;
        case 2:
            system("cls");
            cout << "Jogadores com mais ganhos" << endl;
            listarJogadoresMaisGanhos(); //chama função gestaoCasino que irá exibir os jogadores que tem mais ganhos no casino
            break;
        case 0:
            break;

        default:
            cout << "Opção inválida. Tente novamente." << endl;
        }
    }
    while (op != 0);
}// Fim Menu Gestão User

//Funções

//Função Run Casino
void Casino::Run(){
    char key;
    bool maquinasJaLigadas = false;
    bool casinoEncerrado = false;
    while(true){
        cout<< "Menu" <<endl;
        time_t now = time(nullptr);
            tm* current_time = localtime(&now);

        int currentHour = current_time->tm_hour;//Hora atual
        int currentMinute = current_time->tm_min;//Minuto atual
        int currentSecond = current_time->tm_sec;//Segundo atual

        int openingTimeInSeconds = horaAbertura * 3600 + minutosAbertura * 60 + segundosAbertura;// Converte o horário de abertura para segundos
        int closingTimeInSeconds = horaFecho * 3600 + minutosFecho * 60 + segundosFecho;// Converte o horário de fechamento para segundos
        int currentTimeInSeconds = currentHour * 3600 + currentMinute * 60 + currentSecond;// Converte o horário atual para segundos

        if (currentTimeInSeconds >= openingTimeInSeconds && currentTimeInSeconds <= closingTimeInSeconds) {

                    // O casino está aberto!
                    cout << "O casino está aberto!" << endl;
                    casinoEncerrado = false;
                    if (!maquinasJaLigadas) { //Verifica se já tem todas as maquinas ligadas
                        ligarTodasMaquinas(); //Liga todas as maquinas
                        maquinasJaLigadas = true; //Guarda estado como já as ligou
                    }

                    // Todas as maquinas existentes passam pela função avariar e corre maquina
                    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){
                        avariar((*it)->getID()); //Função avariar
                        (*it)->Run(); //Corre maquina
                    }

                    // Verifica se o Casino está cheio  ou se podem entrar jogadores
                    if(jogadoresNoCasino < maxJogadores){
                            if(entrarJogador()){ //Verifica se entrou algum jogador

                                int randomIndex = rand() % LM.size(); //Vai buscar um numero random entre 0 e tamanho da lista
                                auto it = next(LM.begin(), randomIndex); //o next faz com que se obtenha a posição correta da maquina
                                //o auto it serve para ir buscar o tipo correto de dados que neste caso é máquina
                                Maquina *maquina = *it; //guarda no apontador maquina o ponteiro it referente ao objeto amquina radnom
                                cout << "Maquina escolhida: " <<  maquina->getNome()  << endl;

                                jogadoresNoCasino ++; //Incrementa mais um jogador
                                User *user = userEntraCasino("pessoas.txt"); //Guarda no apontador user o user retornado pela função userEntraCasino
                                Add(user); //Adiciona User


                                if(maquina->getUserAtual()!=nullptr){ //Verifica se a maquina escolhida anteriormente está a ser usada
                                    user->entrarFilaEspera(maquina); //Se estiver o user entra na fila de espera
                                }else{
                                    user->associarMaquina(maquina); //se não estiver o user associa-se a uma maquina
                                }

                                cout << "User " <<  user->getCarteira()  << endl;
                                cout << "entrou " << jogadoresNoCasino << endl;

                            }
                    }

                    //Se existirem mais do que 1 user percorre a lista de useres
                    if(LU.size()>=1){
                        for (list<User *>::iterator it = LU.begin(); it != LU.end(); ++it){
                                (*it)->Run(); //correr user
                        }
                    }

                    // Verifica se saiu algum user
                    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
                        if ((*it)->getUserAtual()==nullptr) { //verifica se a maqina atual não possui nenhum jogador
                            if (!(*it)->getFilaEspera().empty()) { //verifica se lista de espera da maquina não esta vazia
                                User *useruser = (*it)->getFilaEspera().front(); //Vai buscar o user a frente na lista de espera da maquina
                                useruser->associarMaquina(*it); //user associa-se a maquina
                                (*it)->removerUsuarioFilaEspera(useruser); //e é retirado da lista de espera

                                cout << "User: " <<  useruser->getNome() << " saiu da fila de espera e sentou-se na maquina" << endl;

                            }else{
                                (*it)->setUserAtual(nullptr); //se não houver ninguem na lisat de espera o useratual da maquinafica a null
                            }
                        }
                    }

            } else {
                cout << "O casino está fechado. Aguardando o horário de abertura." << endl;

                if (!casinoEncerrado){
                    string nome = devolveData();
                    Relatorio(nome);
                    casinoEncerrado = true;
                    if (maquinasJaLigadas) { //Se casino fechado então verifica se as amquina ainda estão ligadas
                        desligarTodasMaquinas(); //Desliga todas as maquina
                        maquinasJaLigadas = false; //Guarda estado como já desligou
                    }

                }

            }

            if (_kbhit()) { //Função de c++ que verifica se foi pressionada alguma tecla
                key = _getch(); //guarda tecka pressionada
                if (key == 'm' || key == 'M') { //verifica se é m ou M
                    Menu(); //abre Menu
                }
            }

    }

}

void Casino::avariar(int IDMaq){

    //srand (time(NULL));

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it){ //Percorre todas as maquinas
        //Sleep(500);
        if((*it)->getID() == IDMaq){ //verifica se o ud da maquina atual é igual ao passado na função

            if((*it)->getEstado() == ON){ //Verifica se amaquina está ligada
                //rand() %100, vai buscar um numero entre 0 e 99 e adiciona mais 1 par air buscar o número correto
                int nRandom1 = rand() % 100 + 1; //gera um numero random
                int nRandom2 = rand() % 100 + 1; //gera um numero random

                cout << "nRandom1: " << nRandom1 << endl;
                cout << "nRandom2: " << nRandom2 << endl;

                if(nRandom1 == nRandom2){ //se os numero random forem iguais então avaria maquina
                    (*it)->avariaMaquina(); //Maquina chama função avariaMaquina()
                }

            }
        }

    }

}


list<string> * Casino::Ranking_Dos_Fracos(){

     // Lista para armazenar as informações das máquinas avariadas ordenadas por número de avarias
    list<string>* rankingAvariadas = new list<string>;

    // Criar uma cópia da lista principal
    list<Maquina*> maquinasCopy(LM.begin(), LM.end());

    // Ordenar a lista de máquinas por número de avarias (do maior para o menor)
    maquinasCopy.sort([](Maquina* a, Maquina* b) {
        return a->getnAvarias() > b->getnAvarias();
    });

    // Preencher a lista de strings com informações das máquinas ordenadas
    for (list<Maquina *>::iterator it = maquinasCopy.begin(); it != maquinasCopy.end(); ++it) {
        string info =   " |ID: " + to_string((*it)->getID()) +
                        " | Nome: " + (*it)->getNome() +
                        " | Avarias: " + to_string((*it)->getnAvarias());
        rankingAvariadas->push_back(info);
    }

    return rankingAvariadas;

}

void Casino:: showRankingAvarias(){

    // Chamar a função para obter o ranking das máquinas avariadas
    list<string>* rankingAvariadas = Ranking_Dos_Fracos();

    // Verificar se a lista não está vazia antes de tentar acessar os elementos
    if (rankingAvariadas->size() <= 0) {
        cout << "Nenhuma máquina avariada encontrada." << endl;
    } else {
        // Percorrer e exibir as informações das máquinas no ranking
        for (list<string>::iterator it = rankingAvariadas->begin(); it != rankingAvariadas->end(); ++it) {
            cout << (*it) << endl;
        }

    }

    // Não se esqueça de liberar a memória alocada para a lista
    delete rankingAvariadas;

}

//Desligar todas as maquinas
void Casino::desligarTodasMaquinas() {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //perocorre todas as maquinas
        (*it)->Desligar(); //maquina chama função Desligar()
    }
}

//Ligar todas as maquinas
void Casino::ligarTodasMaquinas() {

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //perocorre todas as maquinas
        (*it)->Ligar(); //maquina chama função Ligar()
    }
}

//Mostrar dados do casino
void Casino::dadosCasino() {
    cout << "Nome do Casino: " << nome << endl; //nome casino
    cout << "Máximo de Jogadores: " << maxJogadores << endl; //maximo de jogadores no casino
    cout << "Hora de Abertura: " << horaAbertura << ":" << minutosAbertura << ":" << segundosAbertura << endl; //hora de abertura
    cout << "Hora de Encerramento: " << horaFecho << ":" << minutosFecho << ":" << segundosFecho << endl; //hora de fecho
}

//Adicionar máquina
bool Casino::Add(Maquina *m){ //recebe por parametro um ponteiro para um objecto do tipo Maquina

    adicionarVizinho(m); //Função para adicionar vizinho
    LM.push_back(m); //Adicionar maquina a lisat de maquinas do casino

    return true;
}

bool Casino::adicionarVizinho(Maquina *m) { //recebe por parametro um ponteiro para um objecto do tipo Maquina

    bool estado = false; //iguala o estado a falso
    int distanciaMaxima = 3;  //Defina a distância máxima desejada

    for (auto it = LM.begin(); it != LM.end(); ++it) { //percorre todas as maquinas do casino
        if ((*it)->getID() != m->getID() && (*it)->getX() == m->getX()) {
            /*
            verifica se o id da maquina que esta a ser verificada não é igual ao id da maquina que chega por parametro e
            verifica também se esta na mesma fila de x
            */

            int distanciaY = abs((*it)->getY() - m->getY()); //diferença entre y´s das maquinas

            if (distanciaY <= distanciaMaxima) {
                // Adicionar novaMaquina à lista de vizinhos de maquina
                (*it)->adicionarVizinho(m);

                // Adicionar maquina à lista de vizinhos de novaMaquina
                m->adicionarVizinho(*it);

                estado = true; //iguala estado a true
            }
        }
    }

    return estado; //returna o estado
}

void Casino::Desligar(int id_maq) { //recebe o id da maquina que pretendem desligar

   for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if((*it)->getID() == id_maq){ //se id maquina for igual ao id da maquina que pretendem desligar
            (*it)->Desligar(); //chama função para desligar maquina
            cout << "Máquina " << (*it)->getNome() << " desligada." << endl;
            return; //ao desligar a maquina desejada a função encerra
        }

    }
    cout << "Máquina não encontrada." << endl; //se não encontrar a maquina desejada mostra mensagem de "erro"
}

estadoMaquina Casino::Get_Estado(int id_maq) { //recebe id da maquina desejada

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if((*it)->getID() == id_maq){ //se id maquina for igual ao id da maquina que pretendem desligar
            return (*it)->getEstado(); //returna estado da maquina
        }

    }
    cout << "Máquina não encontrada." << endl; //se não encontrar a maquina desejada mostra mensagem de "erro"
    return estadoMaquina::OFF; //e retorna estado OFF
}


//Função complementar para lisatgem de maquina com probabilidade de ganhar superior a X
void Casino::ListarMaquinasProbabilidade() {

    try {
        float xProbabilidade; //variavel do tipo float para guardar probabilidade desejada
        cout << "Probabilidade: ";
        cin >> xProbabilidade; //guardar probabilidade

        //Abrir ficheiro
        ofstream F("ListaProbX.txt");
        Listar(xProbabilidade, F);

        //Fechar ficheiro
        F.close();
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
    }

}

//Listar maquinas com probabilidade de ganhar maior que X
void Casino::Listar(float X, ostream &f) {
    //recebere um float com a probabilidade desejada e uma referência a um objeto de fluxo de saída

    f << "Máquinas com probabilidade " << X << " maior que de ganhar:" << endl; //escreve no ficheiro
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if ((*it)->getProb() > X) { //se a probabilidade maquina for superior a probabilidade desejada
            f << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl; //escreve no ficheiro
            cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl; //printa no ecra
            /*Mostra o numero da maquina e a probabilidade da mesma*/
        }
    }

}

//traduzir estado para string
string Casino::estadoString(estadoMaquina estado){ //recebe um valor do tipo estadoMaquina
    string estadoString; //variavel que guarda o valor como string
    switch (estado) { //verifica estado
        case ON:
            // Se o estado for ON, atribui a string "ON" à variável estadoString
            estadoString = "ON";
            break;
        case OFF:
            // Se o estado for OFF, atribui a string "OFF" à variável estadoString
            estadoString = "OFF";
            break;
        case AVARIADA:
            // Se o estado for AVARIADA, atribui a string "AVARIADA" à variável estadoString
            estadoString = "AVARIADA";
            break;
        default:
            // Se o estado não corresponder a nenhum dos casos anteriores, atribui a string "Unknown" à variável estadoString
            estadoString = "Unknown";
            break;
    }

    return estadoString;
}

//Função complementar mostra maquinas avariadas
void Casino::maquinaAvariada(){

    char opReparar; //variavel do tipo char para guardar resposta
    bool maquinasAvariadas;

    do{

        maquinasAvariadas = mostrarMaquinas();

        if(maquinasAvariadas == true){
            cout << "Deseja reparar alguma maquina (S/N): " << endl; //printa se deseja repara alguam das maquinas que apareceu
            cin >> opReparar; //guarda resposta
            if(opReparar == 'S' || opReparar == 's'){ //verifica se escolheram reparar
                int id_maq; //variavel do tipo int para guardar  numero da maquina
                cout << "Numero Maquina: " ; //pede o numero da maquina
                cin >> id_maq; //guarda numero da maquina que pretendem reparar
                reparar(id_maq); //chama função complementar para reparar maquina
            }
        }else{
            cout << "Não existem maquinas para reparação" << endl;
        }

    }while (maquinasAvariadas && (opReparar != 'N' && opReparar != 'n'));

}

bool Casino::mostrarMaquinas(){

    bool maquinasAvariadas = false;

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if ((*it)->getEstado() ==  AVARIADA) { //se o estado da maquina for igual a AVARIADA
            cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << " | Estado: " << (*it)->getEstado() << " | Temperatura: " << (*it)->getTemperaturaSensor() << endl; //mostra a maquina
            maquinasAvariadas = true;
        }
    }

    return maquinasAvariadas;
}

//função comlementar para reparar maquina
void Casino::reparar(int id_maq){

    bool reparada = false;

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        if ((*it)->getID() == id_maq) { //se o id da maquina for igual ao id da maquina desejada
            if ((*it)->getEstado() == AVARIADA){ //se o estado da maquina for igual a AVARIADA
                reparada = (*it)->repararMaquina(); //invoca função para reparar maquina
                if(reparada){
                    cout << "ID: " << (*it)->getID() << " | Maquina Reparada" << endl; //printa mensagem positiva apos reparação
                }else if(!reparada){
                    cout << "ID: " << (*it)->getID() << " | Maquina não foi reparada" << endl; //printa mensagem positiva apos reparação
                }

            }
            else if ((*it)->getEstado() == ON || (*it)->getEstado() == OFF){ //se a maquina não se encontrar AVARIADA
                cout << "ID: " << (*it)->getID() << " | Maquina sem avarias" << endl; //printa mensagem com mensagem
            }
        }
    }


}

//Listar maquinas
void Casino::ListarMaquinas() {

    cout << "|ID: | Nome: | Posição: (x ,y ) | Prêmio | ProbG:  | Estado: " << endl; //printa cabeçalho da lista

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) { //percorre todas as maquinas do casino
        (*it)->informacaoMaquina(); //Mostra dados da maquina
    }

}


//Outras Funções



//Função complementar para listar maquinas de um dado tipo
void Casino::listarTipoMaquina(){

    string Tipo; //varivel do tipo string para guardar o tipo de maquina desejada
    cout << "Tipo de maquina: "; //pede o tipo de maquina pretendida
    cin >> Tipo; //guarda tipo de maquina desejada

    ofstream F("MaquinasTipo.txt"); //abrir fecheiro
    list<Maquina*>* maquinasDoTipo = Listar_Tipo(Tipo, F); //invoca função para listar maquinas de um dado tipo

    for (list<Maquina *>::iterator it = maquinasDoTipo->begin(); it != maquinasDoTipo->end(); it++) {
        if ((*it)->getTipo() == Tipo) {
            cout << "ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << endl;
        }
    }

    delete maquinasDoTipo;

    F.close(); //fechar ficheiro

}

//listar maquinas de um dado tipo
list<Maquina *> * Casino::Listar_Tipo(string Tipo, std::ostream &f) {

    list<Maquina *> * maquinasDoTipo = new list<Maquina*>;

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {
        if ((*it)->getTipo() == Tipo) {
            maquinasDoTipo->push_back((*it));
            f << "ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Tipo: " << (*it)->getTipo() << endl;
        }
    }


    return maquinasDoTipo;

}
/*void liberarMemoria(list<Maquina*>* listas) {
    for (list<Maquina*>::iterator it = maquinasDoTipo->begin(); it != maquinasDoTipo->end(); it++) {
        delete *it;  // Deleta cada objeto apontado pela lista
    }

    delete maquinasDoTipo;  // Deleta a lista em si
}*/



//Menus
//Menu Crud Maquinas
void Casino::menuCrudMaquina(){

    int op = 0; //varivel do tipo int para guardar a opção escolhida
    int id_maq; //variavel do tipo int para guarda id maquina

    do {
        //system("cls"); //limpa ecra
        cout << "Crud Maquina" << endl; //printa menu
        cout << "1- Adicionar Maquina" << endl; //printa menu
        cout << "2- Remover Maquina" << endl; //printa menu
        cout << "3- Editar Maquina" << endl; //printa menu
        cout << "4- Mover Maquina" << endl; //printa menu
        cout << "0- Sair" << endl; //printa menu

        cout << "opcao: "; //printa qual é a decisão que pretende selecionar
        cin >> op; //guarda opção desejada

        switch(op){ //verifica opção

        case 1: //caso seja 1
            cout<< "Adicionar Maquina" <<endl; //printa escolha
            addMaquina(); //chama função para adicionar maquina
            break; //quebra codição
        case 2: //caso seja 2
            cout<< "Remover Maquina" <<endl; //printa escolha
            cout <<"ID Maquina: "; //pede o id de maquina pretendida
            cin >> id_maq; //guarda id maquina desejada
            removerMaquina(id_maq); //chama função para remover maquina passando lhe o id da maquina por parametro
            break; //quebra codição
        case 3: //caso seja 3
            cout<< "Editar Maquina: Introduza id número maquina" <<endl; //printa escolha
            cout <<"ID Maquina: "; //pede o id de maquina pretendida
            cin >> id_maq; //guarda id maquina desejada
            editarMaquina(id_maq); //chama função para editar maquina passando lhe o id da maquina por parametro
            break; //quebra codição
        case 4: //caso seja 4
            cout<< "Mover Maquina" <<endl; //printa escolha
            cout <<"ID Maquina: "; //pede o id de maquina pretendida
            cin >> id_maq; //guarda id maquina desejada
            moverMaquina(id_maq); //chama função para mover maquina passando lhe o id da maquina por parametro
            break; //quebra codição
        case 0:
            break; //quebra codição

        default: //se nenhuma das opções
            cout << "Opção inválida. Tente novamente." << endl; //printa mensagem de erro
        }
    }
    while (op != 0); //repete processo enquanto opção diferente a 0
}// Fim Menu Crud Maquinas

// Crud Maquinas
void Casino::addMaquina(){

    int premio, x, y; //variveis do tipo int para guardar premio, x e y
    string nome, tipo; //variveis do tipo string para guardar nome, tipo da maquina
    float prob; //variveis do tipo float para guardar probabilidade da maquina
    bool existe; //variveis do tipo bool para guardar premio, x e y ////////////////////

    cout << "Nome máquina: "; //pede nome da maquina
    cin >> nome; //guarda nome da maquina
    do {
        cout << "X máquina: "; //pede x da maquina
        cin >> x; //guarda x da maquina
        cout << "Y máquina: ";
        cin >> y;
        existe = false;
        for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
            if ((*it)->getX() == x && (*it)->getY() == y) {
                existe = true;
            }
        }

        if(!existe){
            cout << "Premio máquina: ";
            cin >> premio;
            cout << "Probabilidade máquina: ";
            cin >> prob;
            cout << "Tipo máquina: ";
            cin >> tipo;

            Maquina* m = nullptr;

            int id = LM.back()->getID()+1;

            if (tipo == "slot" || tipo == "Slot") {
                m = new MSlot(id, nome, x, y, premio, prob, tipo,10, this);
            } else if (tipo == "poker" || tipo == "Poker") {
                m = new MPoker(id, nome, x, y, premio, prob, tipo,15, this);
            }

            Add(m);
        }
    }
    while (existe == true);


}

//remover maquina
bool Casino::removerMaquina(int id_maq) {

    removerVizinho(id_maq); //chama função para remover vizinhos da maquina a remover passando o id da maquina por parametro

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //percorre todas as maquinas do casino
        if ((*it)->getID() == id_maq) { //se id da maquina atual for igual ao id passado por parametro
            //delete *it;  // Libera a memória alocada pela máquina /////////
            LM.erase(it); // Remove a máquina da lista
            cout << "Máquina removida." << endl;
            return true;
        }
    }

    cout << "Máquina não encontrada." << endl;
    return false;
}

//remover vizinhos
void Casino::removerVizinho(int id_maq) { //recebe por parametro o id da maquina da qual os vizinhos serão apagados

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) { //percorre todas as maquinas do casino

        (*it)->removerVizinho(id_maq); //chama função para remover vizinhos da maquina que será remvida

    }
    cout << "Eleminei tudo." << endl; //printa mensagem

}

//editar maquina
bool Casino::editarMaquina(int id_maq){

    cout << " |Dados maquina|" << id_maq << endl;
    int op;
    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        if ((*it)->getID() == id_maq) {
            do {
                cout << " |Nome Maquina: " << (*it)->getNome() <<"| " << endl;
                cout << " |X: " << (*it)->getX() <<"| " << endl;
                cout << " |Y: " << (*it)->getY() <<"| " << endl;
                cout << " |Probabilidade: " << (*it)->getProb() <<"| " << endl;
                cout << " |Premio: " << (*it)->getPremio() <<"| " << endl;


                cout << "Selecione o dado a alterar:" <<endl;
                cout << "1 - Nome" <<endl;
                cout << "2 - Probabilidade" <<endl;
                cout << "3 - Premio" <<endl;
                cout << "0 - Sair" <<endl;

                string nome;
                int premio;
                float prob;
                cin >> op;
                switch(op){

                    case 1:
                        cout << "Introduza o novo nome: " << endl;
                        cin >> nome;
                        (*it)->setNome(nome);
                        break;
                    case 2:
                        cout << "Introduza o nova probabilidade: " << endl;
                        cin >> prob;
                        (*it)->setProb(prob);
                        break;
                    case 3:
                        cout << "Introduza o novo premio: " << endl;
                        cin >> premio;
                        (*it)->setPremio(premio);
                        break;
                    case 0:
                        break;

                    default:
                        cout << "Opção inválida. Tente novamente." << endl;
                }
                system("cls");
            }

        while (op != 0);
        }

    }


    cout << "Máquina não encontrada." << endl;
    return false;
}

//mover maquina
bool Casino::moverMaquina(int id_maq){

    int x, y, xAnt;
    bool movido = false;

    cout << "Mover Máquina de lugar" << endl;

    // Procurar a máquina com o ID fornecido
    list<Maquina *>::iterator itFirst = LM.begin();
    while (itFirst != LM.end() && (*itFirst)->getID() != id_maq) {
        ++itFirst;
    }

    if (itFirst != LM.end()) {
        cout << " |Nome Maquina: " << (*itFirst)->getNome() << "| " << endl;
        cout << " |X: " << (*itFirst)->getX() << "| " << endl;
        cout << " |Y: " << (*itFirst)->getY() << "| " << endl;
        cout << " |Probabilidade: " << (*itFirst)->getProb() << "| " << endl;
        cout << " |Premio: " << (*itFirst)->getPremio() << "| " << endl;
        cout << endl;

        do {
            // Obter a nova posição
            cout << "Novo valor de X: ";
            cin >> x;
            cout << "Novo valor de Y: ";
            cin >> y;

            // Verificar se a nova posição já está ocupada
            bool posicaoOcupada = false;
            list<Maquina *>::iterator it = LM.begin();
            while (it != LM.end()) {
                if ((*it)->getX() == x && (*it)->getY() == y) {
                    posicaoOcupada = true;
                    cout << "Posição já ocupada. Escolha outra posição." << endl;
                    break;
                }
                ++it;
            }

            if (!posicaoOcupada) {
                // Atualizar a posição da máquina
                (*itFirst)->setX(x);
                (*itFirst)->setY(y);
                movido = true;
                cout << "Máquina movida para (" << x << ", " << y << ")" << endl;

                //Atualizar vizinhos
                ////Remover vizinho
                (*itFirst)->removerVizinhoTodos();
                removerVizinho((*itFirst)->getID());

                ////Adicionar nvo vizinho
                adicionarVizinho((*itFirst));

            }

        } while (!movido);
    } else {
        cout << "Máquina com ID " << id_maq << " não encontrada." << endl;
    }

    return movido;

}

// Fim Crud Maquinas


//jogador entra no casino
User* Casino::userEntraCasino(const string &nomeArquivo){

    int numeroLinhas = ContarLinhas(nomeArquivo);
    if (numeroLinhas != -1) {
        cout << "O arquivo tem " << numeroLinhas << " linhas." << endl;
    }

    // Gere um número aleatório dentro do intervalo de linhas do arquivo
    int linhaAleatoria = rand() % numeroLinhas + 1;

    // Abra o arquivo
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return nullptr;
    }

    // Leitura das linhas até chegar à linha desejada
    string linha;
    for (int i = 0; i < linhaAleatoria; ++i) {
        getline(arquivo, linha);
    }

    // Use um stringstream para processar a linha
    stringstream ss(linha);

    // Declare as variáveis para armazenar os dados do usuário
    int id;
    string nome, sobrenome, morada;
    int idade;

    // Leitura dos dados do usuário da linha
    // Leitura dos dados do usuário da linha
    ss >> id;
    ss.ignore(); // Ignora o espaço em branco
    getline(ss, nome, '\t');
    getline(ss, morada, '\t');
    ss >> idade;

    cout << "Código: " << id << endl;
    cout << "Nome: " << nome << endl;
    cout << "Morada: " << morada << endl;
    cout << "Idade: " << idade << endl;

    // Feche o arquivo
    arquivo.close();

    // Crie um objeto User com base nos dados lidos
    User* newUser = new User(id, nome, morada, idade, this);

    jogadoresNoCasino +1;

    // Adicione o usuário ao casino
    return newUser;
}

//adicionar jogador/user
bool Casino::Add(User *ut){

    if (ut == nullptr) {
        cout << "Erro: Não foi possivel adicionar este user pois está a nulo." << endl;
        return false;
    }

    LU.push_back(ut);

    cout << "User adicionado com sucesso!" << endl;

    return true;

}

//contar linhas de um ficheiro
int Casino::ContarLinhas(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return -1; // Indicativo de erro
    }

    int numeroLinhas = 0;
    string linha;

    while (getline(arquivo, linha)) {
        numeroLinhas++;
    }

    cout << "numero de linahs    " << numeroLinhas << endl;
    arquivo.close();
    return numeroLinhas;
}

//jogador entra no casino
bool Casino::entrarJogador(){

    bool entrada = false;

    srand (time(NULL));

    int nRandom1 = rand() % 100 + 1;
    cout << "nRandom1: " << nRandom1 << endl;

    if(nRandom1 < 50){
        entrada = true;
    }

    return entrada;

}

//Ranking dos mais trabalhadores - as que são usadas
list<Maquina *> *Casino::Ranking_Das_Mais_Trabalhadores() {

    //Crie uma cópia da lista de máquinas
    list<Maquina *> * copiaMaquinas = new list<Maquina *>(LM.begin(), LM.end());

    //Ordene a lista usando a função de comparação
    copiaMaquinas->sort([](Maquina* a, Maquina* b) {
        return a->getNJogos() > b->getNJogos();
    });

    return copiaMaquinas;

}

void Casino::listarRankingMaisTrabalhadores(){

    list<Maquina*>* maquinasMaisTrabalhadores = Ranking_Das_Mais_Trabalhadores();

    for (list<Maquina *>::iterator it = maquinasMaisTrabalhadores->begin(); it != maquinasMaisTrabalhadores->end(); ++it) {

        cout << "ID: " << (*it)->getID() << " | Nome: " << (*it)->getNome() << " | Numero de jogos: " << (*it)->getNJogos() << endl;

    }

    delete maquinasMaisTrabalhadores;

}

//jogadores mais frequentes - jogadores que mais tempo passaram a jogar
list<User *> * Casino::Jogadores_Mais_Frequentes (){

    // Crie uma cópia da lista de máquinas
    list<User *> *copiaUser = new list<User *>(LU.begin(), LU.end());

    copiaUser->sort([](User* a, User* b) {
        return a->getTempoCasino() > b->getTempoCasino();
    });

    return copiaUser;

}

void Casino::listarJogadoresMaisFrequentes(){

    list<User*>* copiaUser = Jogadores_Mais_Frequentes();

    for (list<User *>::iterator it = copiaUser->begin(); it != copiaUser->end(); ++it) {

        cout << "ID: " << (*it)->getNUser() << " | Nome: " << (*it)->getNome() << " | tempo casino: " << (*it)->getTempoCasino() << endl;

    }

    delete copiaUser;

}

//jogadores que ganharam mais dinheiro
list<User *> * Casino::Jogadores_Mais_Ganhos () {

    // Crie uma cópia da lista de máquinas
    list<User *> *copiaUser = new list<User *>(LU.begin(), LU.end());

    copiaUser->sort([](User* a, User* b) {
        return a->getGanhos() > b->getGanhos();
    });

    return copiaUser;

}

void Casino::listarJogadoresMaisGanhos(){

    list<User*>* jogadoresMaisGanhos = Jogadores_Mais_Ganhos();

    for (list<User *>::iterator it = jogadoresMaisGanhos->begin(); it != jogadoresMaisGanhos->end(); ++it) {

        cout << "ID: " << (*it)->getNUser() << " | Nome: " << (*it)->getNome() << " | ganhos: " << (*it)->getGanhos() << endl;

    }

    delete jogadoresMaisGanhos;

}

//Relatório casino - informação do estado atual de cada máquina
void Casino::Relatorio(string fich_xml) {

    cout<< "Nome ficheiro: " << fich_xml <<endl;

    XMLWriter XX;
    XX.WriteStartDocument(fich_xml);
    XX.WriteStartElement("RELATORIO");

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); it++) {

        XX.WriteStartElement("Maquina"); // Abre o Elemento “Maquina”
            XX.WriteElementString("ID",to_string((*it)->getID()));
            XX.WriteElementString("ESTADO",estadoString((*it)->getEstado()));
        XX.WriteEndElement(); // Fecha o Elemento “Maquina”

    }

    XX.WriteEndElement(); // Fecha o Elemento “RELATORIO”
}


//subir probabilidade das maquinas vizinhas da maquina que ganhou
void Casino::SubirProbabilidadeVizinhas(Maquina *M_ganhou, float R, list<Maquina *> &lmvizinhas) {

    // Lista para armazenar máquinas afetadas
    list<Maquina *> maquinasAfetadas;

    for (list<Maquina *>::iterator it = lmvizinhas.begin(); it != lmvizinhas.end(); ++it) {
        int distanciaY = abs((*it)->getY() - M_ganhou->getY());
        if (distanciaY <= R) {
            float probAt = (*it)->getProb();
            (*it)->setProb(probAt + 1.0);
            maquinasAfetadas.push_back((*it));
        }
    }

    mostrarMaquinasAfetadas(maquinasAfetadas);

}

void Casino::mostrarMaquinasAfetadas(list<Maquina *> &maquinasAfetadas) {
    cout << "Máquinas afetadas:" << endl;
    for (list<Maquina *>::iterator it = maquinasAfetadas.begin(); it != maquinasAfetadas.end(); ++it) {
        cout << "ID: " << (*it)->getID() << " | Probabilidade: " << (*it)->getProb() << endl;
    }
}

//memoria toral do programa
int Casino::Memoria_Total() {

    int mem = 0;

    //calcular memoria variveis Casino
    mem = nome.size()+sizeof(maxJogadores)+sizeof(jogadoresNoCasino)+sizeof(probabilidadeUser)+sizeof(horaAbertura)+sizeof(minutosAbertura)+sizeof(segundosAbertura)+sizeof(horaFecho)+sizeof(minutosFecho)+sizeof(segundosFecho)+sizeof(totalPremios)+sizeof(totalDinheiroDado);
    mem += sizeof(*this);

    // Adicione o tamanho da lista à memória total
    mem += LM.size() * sizeof(Maquina*);

    for (list<Maquina *>::iterator it = LM.begin(); it != LM.end(); ++it) {
        //mem += sizeof(*it);
        mem += (*it)->Memoria();
    }

    // Adicione a memória associada a membros dinâmicos, se houver
    mem += sizeof(User*) * LU.size(); // Tamanho da lista de ponteiros de usuários

    // Itera sobre os usuários e adiciona a memória associada a cada um
    for (list<User*>::iterator it = LU.begin(); it != LU.end(); ++it)
        mem += (*it)->Memoria();

    cout << "Memoria total Casino: " << mem << endl;

    return mem;

}

