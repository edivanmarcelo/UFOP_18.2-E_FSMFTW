#include"Inicializa.hpp"
#include"Construcao.hpp"
#include"VNS.hpp"
#include<iostream>
using namespace std;

//PARAMETROS
#define qnt 15 //QUANTIDADE DE CLIENTES
#define qntIter 10 //NUMERO DE ITERAÇÕES SEM MELHORA
int main(){
    clock_t tempo;
    double auxTempo;
    char arq[15] = "c103_21.txt"; //INSTANCIA USADA
    
    lerArq(qnt, arq);

    alocarDist();
    inicializarDist();

    tempo = clock();
    constroiSolucaoParcialmenteGulosaVMP(qnt);

    multiStart(qntIter, qnt);
    tempo = clock()- tempo;

    imprimirSol();
    verificaSol(qnt);

    auxTempo = tempo / (double)CLOCKS_PER_SEC;
    cout << endl << "TEMPO: " << auxTempo << " s" << endl;

    apagarDist();
    return 0;
}
