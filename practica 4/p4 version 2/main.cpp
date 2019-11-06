//Jaime Viejo Martinez
//Cristobal Ramos Laina
#include"indices.h"


void cargarArchivo(tListaCadenas & l);

int main() {
	
	tListaCadenas l;
	cargarArchivo(l);

}


void cargarArchivo(tListaCadenas & l) {
	tIndicePalabras  tabla;
	string  archivoInicial;
	tVisitados  visitados;
	tNoVisitados noVisitados;
	tTotales totales;
	tVector vector;
	tMatriz L, M;
	int n = 0;
	l.contador = 0;
	string ar, pal;
	ifstream archivo;

	cout << "Por favor, introduzca el nombre del fichero raiz a partir del que se creara el indice: " << endl;
	cin >> ar;
	archivo.open(ar);
	if (archivo.is_open()) {
		cout << "Cargando...[CORRECTO]" << endl << endl;
		cout << "Creando tablas... [CORRECTO]" << endl << endl;

		crearTabla(tabla, ar, totales,L,M, vector);
		cout << "Matriz L " << endl;
		mostrarMatriz(L); //muestro la matriz L
		cout << endl;
		imprimirTabla(tabla); //muestro todas las palabras con sus archivos
		cout << endl;
		cout << "Totales: ";
		imprimir(totales); //muestro todos los archivos
		cout << endl;
		cout << "C = " << C << endl;
		cout << "Matriz M: " << endl;
		mostrarMatriz(M);
		cout << "Introduzca una palabra (fin para terminar): " << endl;
		cin >> pal;

		while (pal != "fin") {



			vector.tam = L.columnas;
			buscarPalabra(tabla, pal, vector,totales);
			cout << "Introduzca una palabra (fin para terminar): " << endl;
			cin >> pal;
		}

	}
	else cout << "Cargando...[INCORRECTO]" << endl;


	cout << "Gracias por utilizar el buscador" << endl;
	system("pause");
}