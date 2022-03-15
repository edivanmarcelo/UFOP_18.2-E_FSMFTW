#ifndef Construcao_hpp
#define Construcao_hpp

#include "Inicializa.hpp"

void constroiSolucaoParcialmenteGulosaVMP(int qnt);
int  procuraCMP(int linha, int v, int k, int &EMP);
int  procuraEMP(int linha, int k, double tanque);
int adicionaEMP(int i, int k, int EMP, int v);
void atualizaCliente(int coluna);
bool testaCliente(int i, int j);


#endif
