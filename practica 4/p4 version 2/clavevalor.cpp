//Jaime Viejo Martinez
//Cristobal Ramos Laina
#include"clavevalor.h"

int getTamanno(const tIndicePalabras & l) {
	return l.contador;
}

tRegistroIndicePalabras getRegistro(const tIndicePalabras & l, int pos) {

	return l.registros[pos];

}

int busqueda(const tIndicePalabras & l, const string & s, int ini, int fin) {

	int  pos = -1;

	if (ini <= fin) {
		int mitad = (ini + fin) / 2;
		if (s == l.registros[mitad].clave) {
			pos = mitad;
		}
		else if (s < l.registros[mitad].clave) {
			pos = busqueda(l, s,ini, mitad - 1);
		}
		else {
			pos = busqueda(l, s,mitad + 1, fin);
		}

	}

	return pos;

}


tListaCadenas buscar(const tIndicePalabras & l, const string & s, int & estaaqui) {

	return l.registros[estaaqui].valor;


}

void insertarb(tIndicePalabras & idx, string  & palabra, string & nombreArchivo, tVisitados & visitados, tNoVisitados & noVisitados) {
	int pos;
	bool repetido = false;
		pos = busqueda(idx, palabra, 0, idx.contador-1); // si la palabra no es nueva, mete en esa palabra el nombre del txt
		if(pos != -1){ //que no sea -1 significa que no es nueva
		for (int n = 0; n < idx.registros[pos].valor.contador; n++) { // por si hay la misma palabra en el mismo txt que no te ponga varias veces el mismo txt
			if (nombreArchivo == idx.registros[pos].valor.cadenas[n]) {
				repetido = true;
			}
		}
		if (!repetido) {// si la palabra repetida no es del mismo archivo , es decir, si en un mismo archivo hay dos palabras iguales
			insertar(idx.registros[pos].valor, nombreArchivo); //si la palabra ya existe, mete en la clave el nuevo archivo en que sale

		}

		//noVisitados.cadenas[noVisitados.contador] = nombreArchivo; //REVISAR
	}

	else { // si la palabra es nueva (-1) , crea una clave son esa palabra y le mete el txt que esta leyendo
		bool sitio = false;

		if (idx.contador == 0) { //La primera la mete xk no hay nada que ordenar
			sitio = true;
			idx.registros[0].clave = palabra;
			idx.registros[0].valor.contador = 0;
			idx.registros[0].valor.cadenas[0] = nombreArchivo;
			idx.registros[0].valor.contador++;
			idx.contador++;
		}



		for (int n = 0; n < idx.contador; n++) { // busca cual es su sitio
			if (palabra < idx.registros[n].clave && sitio == false) { // aqui esta su sitio
				sitio = true;
				for (int i = idx.contador; i > n; i--) {

					idx.registros[i] = idx.registros[i - 1]; //a partir de su posicion, mueve todos uno a la derecha
				}
				idx.registros[n].clave = palabra; //una vez movidos todos, lo mete en su sitio
				idx.registros[n].valor.contador = 0;
				idx.registros[n].valor.cadenas[0] = nombreArchivo;
				idx.registros[n].valor.contador++;
				idx.contador++;
			}

		}


		if (!sitio) { // si su sitio es al final del todo, lo mete
			idx.registros[idx.contador].clave = palabra;
			idx.registros[idx.contador].valor.contador = 0;
			idx.registros[idx.contador].valor.cadenas[0] = nombreArchivo;
			idx.registros[idx.contador].valor.contador++;
			idx.contador++;
		}



	}



}

void imprimirTabla( const tIndicePalabras & idx) {


	for (int n = 0; n < idx.contador; n++) {
		cout << idx.registros[n].clave << "  "; // muetsra la palabra
		for (int i = 0; i < idx.registros[n].valor.contador; i++) {
			cout << idx.registros[n].valor.cadenas[i] << " "; // muestra en que txt esta
		}
		cout << endl;
	}
}






char convertir(char hola) {

	if (hola == 'á' || hola == 'Á') {
		hola = 'a';
	}
	if (hola == 'é' || hola == 'É') {
		hola = 'e';
	}
	if (hola == 'í' || hola == 'Í') {
		hola = 'i';
	}
	if (hola == 'ó' || hola == 'Ó') {
		hola = 'o';
	}
	if (hola == 'ú' || hola == 'Ú') {
		hola = 'u';
	}

	return hola;
}