//Jaime Viejo Martinez
//Cristobal Ramos Laina
#ifndef clavevalor_h
#define clavevalor_h
#include"listacadenas.h"
const int MAX_REGISTROS = 200;

typedef struct {

	string clave;
	tListaCadenas valor;
}tRegistroIndicePalabras;


typedef struct {
	tRegistroIndicePalabras registros[MAX_REGISTROS];
	int contador;
}tIndicePalabras;

int getTamanno(const tIndicePalabras & l);
tRegistroIndicePalabras getRegistro(const tIndicePalabras & l, int pos);
int busqueda(const tIndicePalabras & l, const string & s, int ini, int fin);
	tListaCadenas buscar(const tIndicePalabras & l, const string & s, int & estaaqui); //LA POSICION DE LA CLAVE SE LA DA BUSQUEDA()
void insertarb(tIndicePalabras & idx, string  & palabra, string & nombreArchivo, tVisitados & visitados, tNoVisitados & noVisitados);
void imprimirTabla(const tIndicePalabras & idx);
char convertir(char hola);
#endif