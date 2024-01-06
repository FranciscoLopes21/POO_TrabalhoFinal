#include "XMLWriter.h"

XMLWriter::XMLWriter()
{
    //ctor
    cout << "Passei em <" << __FUNCTION__ << ">" << endl;
}

XMLWriter::~XMLWriter()
{
    //dtor
    cout << "Passei em <" << __FUNCTION__ << ">" << endl;
    WriteEndDocument();
}

string XMLWriter::Tabulacoes(int N)
{
    string s = "";
    for (int i = 0; i < N; i++)
        s += "\t";
    return s;
}
void XMLWriter::WriteStartDocument(string ficheiro)
{
    //cout << "Passei em <" << __FUNCTION__ << ">" << endl;
    NOME_FICHEIRO = ficheiro;
    ofstream F(NOME_FICHEIRO);
    F.close();
}
void XMLWriter::WriteEndDocument()
{
    cout << "Passei em <" << __FUNCTION__ << ">" << endl;
    if (TAGS.size() != 0)
        cout << "O Pa esquecestes ve-la ......" << endl;
    else
        cout << "Tudo OK!" << endl;
    //F.close();
}
void XMLWriter::WriteElementString(string el, string valor)
{
    cout << "Passei em <" << __FUNCTION__ << ">" << endl;
    ofstream F(NOME_FICHEIRO, ios::app);
    if (F.is_open())
        F << Tabulacoes(TAGS.size()) << "<" << el <<">" << valor << "</" << el << ">" << endl;
    F.close();
}
void XMLWriter::WriteStartElement(string el)
{
    cout << "Passei em <" << __FUNCTION__ << ">" << endl;
    ofstream F(NOME_FICHEIRO, ios::app);
    F << Tabulacoes(TAGS.size()) << "<" << el <<">" << endl;
    F.close();
    TAGS.push_front(el);
}
void XMLWriter::WriteEndElement()
{
    cout << "Passei em <" << __FUNCTION__ << ">" << endl;
    //cout << "TAM = " << TAGS.size() << endl;
    string el = TAGS.front();
    TAGS.pop_front();
    //cout << "TAM = " << TAGS.size() << endl;
    ofstream F(NOME_FICHEIRO, ios::app);
    F << Tabulacoes(TAGS.size()) << "</" << el <<">" << endl;
    F.close();
}
