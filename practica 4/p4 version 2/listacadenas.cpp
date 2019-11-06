//Jaime Viejo Martinez
//Cristobal Ramos Laina
#include "listacadenas.h"


int getTamanno(const tListaCadenas & l) {

	return l.contador;
}


string getCadena(const tListaCadenas & l, int pos) {


	return l.cadenas[pos];
}


void insertar(tListaCadenas & l, const string & s) {

	l.cadenas[l.contador] = s;
	l.contador++;
}

bool buscar(const tListaCadenas & l, const string & s) {
	bool encontrado = false;
	for (int n = 0; n < l.contador; n++) {

		if (s == l.cadenas[n]) {
			encontrado = true;
		}
	}
	return encontrado;
}

void eliminar(tListaCadenas & l, int pos) {
	for (int n = pos; n < l.contador - 1; n++) {
		l.cadenas[n] = l.cadenas[n + 1];
	}
	l.contador--;
}


void imprimir(const tListaCadenas & l) {
	for (int n = 0; n < l.contador; n++) {
		cout << l.cadenas[n] << " ";
	}
}

