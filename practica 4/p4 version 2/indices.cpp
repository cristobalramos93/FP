//Jaime Viejo Martinez
//Cristobal Ramos Laina
#include"indices.h"

void crearTabla(tIndicePalabras & tabla, string & archivoInicial, tTotales & totales, tMatriz & L, tMatriz & M, tVector & vector) {
	tabla.contador = 0;
	tMatriz aux;
	string pal;
	char caracter[MAX_CADENAS];
	tVisitados  visitados;
	tNoVisitados  noVisitados;
	bool encontrado = false;
	bool en = false;
	tListaCadenas a;
	visitados.contador = 0;
	noVisitados.contador = 0;
	totales.contador = 0;
	noVisitados.cadenas[noVisitados.contador] = archivoInicial;
	totales.cadenas[totales.contador] = archivoInicial;
	totales.contador++;
	noVisitados.contador++;
	L.filas = 2;
	L.columnas = 2;
	L = matrizConstante(0.0, MAX_TAM); //crea una matriz a ceros
	tMatriz mprima;
	mprima = matrizConstante(0.0, MAX_TAM);
	while (noVisitados.contador != 0) {

		visitados.cadenas[visitados.contador] = noVisitados.cadenas[noVisitados.contador - 1]; // lo saco de no visitados y lo meto en visitados
		visitados.contador++;
		noVisitados.contador--;
		ifstream archivo;
		archivo.open(visitados.cadenas[visitados.contador - 1]); // abro el archivo que este sin abrir
		assert(!archivo.fail());
		while (!archivo.eof()) {//comprueba que la cadena sea 0 para acabar de leer
			archivo >> caracter;
			pal = "";
			int i = 0;
			caracter[i] = convertir(caracter[i]); // quita la tilde
			while (caracter[i] != '\0' && (!ispunct(caracter[i]))) { // comprueba si es un espacio en blanco o coma y tal
				pal += caracter[i];
				i++;
				caracter[i] = convertir(caracter[i]); // quita la tilde

			}


			if (caracter[i] == '<') { // detecta que es un caracter y entra aqui si es enlace
				i++;
				while (caracter[i] != '>') {
					pal += caracter[i];
					i++;
				}
				std::transform(pal.begin(), pal.end(), pal.begin(), tolower); //convierte de mayusculas a minusculas



				en = false;
				for (int n = 0; n < totales.contador; n++) { // para que no se meta el mismo archivo varias veces en totales
					if (pal == totales.cadenas[n]) {
						en = true;
					}
				}


				if (!en) { // para que no se meta el mismo archivo varias veces en totales
					noVisitados.cadenas[noVisitados.contador] = pal; // si encuentra un enlace en un archivo, lo mete en no visitados
					noVisitados.contador++;
				}


				encontrado = false;
				for (int n = 0; n < totales.contador; n++) { // para que no se meta el mismo archivo varias veces en totales
					if (pal == totales.cadenas[n]) {
						encontrado = true;
					}
				}


				if (!encontrado) { // para que no se meta el mismo archivo varias veces en totales
					totales.cadenas[totales.contador] = pal; // ese nuevo archivo tambien lo meto en totales
					totales.contador++;
				}


				aux = matrizConstante(0.0, totales.contador); //crea una matriz a ceros

				aux.columnas = totales.contador;
				aux.filas = totales.contador;
				bool modif = false;
				bool mod = false;
				for (int n = 0; n < totales.contador; n++) { // el enlace
					int y, z;
					if (pal == totales.cadenas[n] && modif == false) { // busco la posicion del enlace en totales
						y = n;
						modif = true;
					}
					if ((visitados.cadenas[visitados.contador - 1]) == totales.cadenas[n] && mod == false) { //busco posicion del archivo que está abierto
						z = n;
						mod = true;
					}
					if (modif == true && mod == true) {
						aux.datosMatriz[y][z] = 1;
					}

				}
				L = sumaMatriz(L, aux);
				L.columnas = totales.contador;
				L.filas = totales.contador;

			}

			else { // solo entra aqui para insertar la palabra si no es un enlace
				std::transform(pal.begin(), pal.end(), pal.begin(), tolower); //convierte de mayusculas a minusculas
				insertarb(tabla, pal, visitados.cadenas[visitados.contador - 1], visitados, noVisitados); //inserta la palabra
			}

		}

	}

	tMatriz b, c;
	mprima = desdeEnlacesAMatriz(L, visitados, mprima); // M'
	b = cPorMprima(mprima);
	c = calcularCn(L);
	b.filas = L.filas;
	b.columnas = L.columnas;
	M = sumaMatriz(b, c); // d = M
	M.columnas = L.columnas;
	M.filas = L.filas;

	vector.tam = L.columnas;
	vector = inicializaVector(vector);





	vector = vectorPropio(M, vector);
	
}



void buscarPalabra(tIndicePalabras & tabla, string pal, const tVector  vector, tTotales totales) {

	bool encontrada = false;

	for (int n = 0; n < tabla.contador; n++) {
		if (tabla.registros[n].clave == pal) {
			encontrada = true;

			for (int i = 0; i < tabla.registros[n].valor.contador; i++) {
				cout << "Encontrada en  ";
				cout << tabla.registros[n].valor.cadenas[i];

				for (int j = 0; j < vector.tam; j++) {

					if (tabla.registros[n].valor.cadenas[i] == totales.cadenas[j]) {
						cout << "   relevancia ( " << vector.datos[j] << " )" << endl;
					}
				}

			}
			cout << endl;
		}
	}
	if (!encontrada) {
		cout << "La palabra no aparece en ningun archivo" << endl;
	}

}


