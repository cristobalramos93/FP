//Jaime Viejo Martinez
//Cristobal Ramos Laina
#include"matriz.h"

tMatriz matrizConstante(double x, int n) {
	tMatriz L;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			L.datosMatriz[i][j] = x;
		}
	}
	return L;
}


tMatriz multiplicacion(double x, tMatriz & m) {

	tMatriz o;

	for (int i = 0; i < m.filas; i++) {
		for (int j = 0; j < m.columnas; j++) {
			o.datosMatriz[i][j] = m.datosMatriz[i][j] * x;
		}
	}
	return o;
}


tMatriz sumaMatriz(const tMatriz  m1, tMatriz  m2) {

	tMatriz suma;

	for (int i = 0; i < m1.filas; i++) {
		for (int j = 0; j < m1.columnas; j++) {
			m2.datosMatriz[i][j] = m1.datosMatriz[i][j] + m2.datosMatriz[i][j];
		}
	}
	return m2;

}

tVector multiplicacionVector(const tMatriz & m,   tVector &  v) { //REVISAR

	for (int n = 0; n < m.columnas; n++) {
		double dato = 0;
		for (int j = 0; j < v.tam; j++) {
			dato += m.datosMatriz[n][j] * v.datos[j];
		}
		v.datos[n] = dato;
	}

	return v;
}

tVector inicializaVector( tVector & v) {
	for (int n = 0; n < v.tam; n++) { //inicializamos el vector a unos
		v.datos[n] = 1;
	}
	return v;
}


tVector vectorPropio(const tMatriz & M, tVector &  PR) {
	tVector PR1;
	PR1 = PR;
	int z = 0;
	double resultado;
	bool valido = true;
	bool entra = true;


		while (valido) {
			if (!entra) {
				PR = PR1;
			}
			entra = false;
			PR1 = multiplicacionVector(M, PR1); //multiplicamos el vector por la matriz 
			for (int n = 0; n < PR.tam; n++) {
				resultado = PR1.datos[n] / PR.datos[n];
				if ((resultado > 1.00001) || (resultado < 0.99999)) { 
					valido = false;
				}

			}
			if (valido) {
				PR1 = normaliza(PR1);

				return PR1;
				valido = false;
			}
			else valido = true;

		}
}


tVector normaliza(const tVector & v) {

	tVector aux;
	for (int n = 0; n < v.tam; n++) {

		aux.datos[n] = v.datos[n] / v.datos[0];
	}
	
	return aux;

}




void mostrarMatriz(tMatriz matriz) {


	for (int n = 0; n < matriz.filas; n++) {
		for (int i = 0; i < matriz.columnas; i++) {
			cout << setw(15) << matriz.datosMatriz[n][i];
		}
		cout << endl;
	}


}


tMatriz cPorMprima(tMatriz & mprima) { // c * M'
	tMatriz multiplicada;


	multiplicada = multiplicacion(C, mprima);

	return multiplicada;
}


tMatriz calcularCn(tMatriz & L) { // divide la matriz por el numero de columnas y devolvemos (1-C) * Cn
	tMatriz Cn;




	for (int n = 0; n < L.filas; n++) {
		for (int i = 0; i < L.columnas; i++) {

			Cn.datosMatriz[n][i] = 1;
			Cn.datosMatriz[n][i] = Cn.datosMatriz[n][i] / L.columnas;
		}
	}
	Cn.filas = L.filas;
	Cn.columnas = L.columnas;
	Cn = multiplicacion((1 - C), Cn);

	return Cn;
}


tMatriz desdeEnlacesAMatriz(const tMatriz & L, tVisitados visitados, tMatriz &mprima) { //calcula M' a partir de L

	double z = 0;
	for (int n = 0; n < L.columnas; n++) {
		for (int i = 0; i < L.columnas; i++) {

			z += L.datosMatriz[i][n];

		}


		if (z != 0) {
			for (int h = 0; h < L.columnas; h++) {
				mprima.datosMatriz[h][n] = L.datosMatriz[h][n] / z;
			}
		}
		else {

			for (int h = 0; h < L.columnas; h++) {
				mprima.datosMatriz[h][n] = 1;
				mprima.datosMatriz[h][n] = mprima.datosMatriz[h][n] / L.columnas;

			}


		}

		z = 0;
	}

	mprima.columnas = L.columnas;
	mprima.filas = L.filas;

	return mprima;
}

