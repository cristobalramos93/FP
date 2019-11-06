//Jaime Viejo Martinez
//Cristobal Ramos Laina
#ifndef matriz_h
#define matriz_h
#include"clavevalor.h"

const int MAX_TAM = 10;
const double C = 0.85;

typedef struct {
	int filas, columnas;
	double datosMatriz[MAX_TAM][MAX_TAM];
}tMatriz;




typedef struct {

	int tam;
	double datos[MAX_TAM];

}tVector;

tMatriz matrizConstante(double x, int n);
tMatriz multiplicacion(double x,tMatriz & m);
tMatriz sumaMatriz(const tMatriz  m1, tMatriz  m2);
tVector multiplicacionVector(const tMatriz & m,  tVector & v);
tVector inicializaVector(tVector & v);
tVector vectorPropio(const tMatriz & M,tVector & PR);
tVector normaliza(const tVector & v);
void mostrarMatriz(tMatriz matriz);
tMatriz desdeEnlacesAMatriz(const tMatriz & L, tVisitados visitados, tMatriz &mprima);
tMatriz calcularCn(tMatriz & L);
tMatriz cPorMprima(tMatriz & mprima);

#endif