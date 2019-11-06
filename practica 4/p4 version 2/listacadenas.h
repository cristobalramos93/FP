//Jaime Viejo Martinez
//Cristobal Ramos Laina
#ifndef listacadenas_h
#define listacadenas_h

#include <iostream>
#include<fstream>
#include<string>
#include<ctype.h>
#include<cctype>
#include<algorithm>
#include<iomanip>
#include <stdio.h> 
#include <assert.h>
#include <locale>

using namespace std;
const int MAX_CADENAS = 100;
typedef string tCadenas[MAX_CADENAS];

typedef struct {
	int contador;
	tCadenas cadenas;
}tListaCadenas;

typedef tListaCadenas tVisitados;
typedef tListaCadenas tNoVisitados;
typedef tListaCadenas tTotales;


int getTamanno(const tListaCadenas & l);
string getCadena(const tListaCadenas & l, int pos);
void insertar(tListaCadenas & l, const string & s);
bool buscar(const tListaCadenas & l, const string & s);
void eliminar(tListaCadenas & l, int pos);
void imprimir(const tListaCadenas & l);


















#endif