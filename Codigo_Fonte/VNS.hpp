#ifndef VNS_hpp
#define VNS_hpp

#include"Inicializa.hpp"
#define NV 4
#define NV_VNS 4
#define alfa 1

void vnd(int iter);
void melhorVizinhoVND(int area);
//MOVIMENTOS//
void realocaIntra_VND(void);
void trocaIntra_VND(void);
void realocaInter_VND(void);
void trocaInter_VND(void);
int alteraSolTroca_VND(int iSol);
int alteraSolRealoc_VND(int iSol);

void VNS(int iter);
void vizinhoQualquerVNS(int area);
//MOVIMENTOS//
void realocaIntra_VNS(void);
void realocaInter_VNS(void);
void trocaIntra_VNS(void);
void trocaInter_VNS(void);
void novaRota_VNS(void);

void multiStart(int iter, int qnt);

#endif
