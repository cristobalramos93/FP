//JAIME VIEJO MARTÍNEZ
//CRISTÓBAL RAMOS LAINA
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>



using namespace std;
const int MAX_PROD = 25;
const int PROD_NULO = 0;
const int CENTINELA = -1;
typedef int tArray[MAX_PROD];

void leerFilaFich(ifstream &fich, tArray fila, int &tam); //cargar el archivo
void escribirFilaFich(ofstream &fich, const tArray fila, int tam); //sobreescribe el archivo
void mostrarFila(const tArray fila, int tam); //muestra como queda el array al desplazar o arrastrar
bool esPosValida(int tam, int pos); // si la posicion de lo desplazado por la grua esta dentro del tamaño del array
bool estaVacia(const tArray fila, int pos); // comprueba si el hueco donde vas a dejar lo de la grua esta vacio
void ejecutarGrua(int tam, tArray &fila, int &movimientos); // usa la grua
void ejecutarGuardarFichero(const tArray fila, int tam); // guarda los cambios realizados por la grua y excavadora en el fichero
int posHuecoLibreDir(const tArray fila, int tam, int posIni, int direccion); // busca el primer hueco libre 
bool excavadora1Dir(tArray &fila, int tam, int posIni, int direccion, int numDespla); // realiza el movimiente de los bloques
void ejecutarExcavadora(tArray &fila, int tam, int &movimientos); // ejecuta la excavadora
void menu(int tam, tArray &fila, int &movimientos, bool primera); // menu
bool sonPosicionesPosiblesGrua(int tam, int posIni, int posFin, int posSoltar);//devuelve posible = true si encaja en el array y si ini es menos k fin.
bool esPosibleGrua(const tArray fila, int tam, int posIni, int posFin, int posSoltar); //comprueba si se pisan
//bool grua(tArray &fila, int tam, int posIni, int posFin, int posSoltar, int &movimientos); version 3
bool filaOrdenada(const tArray fila, int tam);//Comprueba que el array quede ordenado despues de cada cambio
bool grua_VO(tArray &fila, int tam, int posIni, int posFin, int posSoltar, int &movimientos); // ejecuta la grua con un solo Array (fila), ya no hay auxiliar como habia en grua()

int main() {
	tArray fila;
	int tam = 0, movimientos = 0;
	bool primera = false;
	menu(tam, fila, movimientos, primera);

	system("pause");
}

void leerFilaFich(ifstream &fich, tArray fila, int &tam) {
	tam = 0; // cada vez que abras un nuevo fichero ponemos el tam a 0, es una manera de borrar el array anterior(lo sobreescribe)
	int x;
	fich >> x;
	while (x != CENTINELA) {

		fila[tam] = x;
		tam++; //el tamaño del array
		fich >> x;
	}

}

void escribirFilaFich(ofstream &fich, const tArray fila, int tam) { //OPCION 2
	for (int n = 0; n < tam; n++) {
		fich << fila[n] << " ";
	}
	fich << "-1";

}

void mostrarFila(const tArray fila, int tam) {

	for (int n = 0; n < tam; n++) {
		cout << setw(2) <<  "|" << setw(2) <<  fila[n];
	}

	cout <<setw(2) << "|" << endl;


	for (int n = 0; n < tam; n++) {

		cout << "----";
	}

	cout << endl;


	for (int n = 0; n < tam; n++) {
		cout << setw(2) << "|" << setw(2) << n;
	}
	cout << setw(2) << "|" << endl;
}


bool esPosValida(int tam, int pos) { //comprueba si el desplazamiento esta dentro del tamaño del array

	bool valido = false;
	if ((pos <= tam - 1) && (pos >= 0)) {
		valido = true;
	}
	return valido;
}


bool estaVacia(const tArray fila, int pos) { //comprueba si la posicion esta vacia
	bool vacia = false;

	if (fila[pos] == PROD_NULO) {
		vacia = true;
	}
	return vacia;
}


void ejecutarGrua(int tam, tArray &fila, int &movimientos) { //opcion 3, EJECUTA GRÚA
	int posIni, posFin, posSoltar;
	cout << "Desde que posicion quieres mover? de 0 a  " << tam - 1 << endl;
	cin >> posIni;
	cout << "Hasta que posicion?" << endl;
	cin >> posFin;
	cout << "Donde lo quieres dejar?" << endl;
	cin >> posSoltar;
	grua_VO(fila, tam, posIni, posFin, posSoltar, movimientos);
	mostrarFila(fila, tam);
}


void ejecutarGuardarFichero(const tArray fila, int tam) { //opcion 2, GUARDA EN UN FICHERO LOS CAMBIOS REALIZADOS POR LA GRUA Y EXCAVADORA
	ofstream archivo;
	string guarda;
	cout << "En que archivo lo quieres guardar? (ponga .txt)" << endl;
	cin >> guarda;
	archivo.open(guarda);
	escribirFilaFich(archivo, fila, tam);
}


int posHuecoLibreDir(const tArray fila, int tam, int posIni, int direccion) { //BUSCA UN HUECO LIBRE EN CUALQUIERA DIRECCION
	int x;
	bool libre = false;
	if (direccion == 1) {
		for (int n = posIni + 1; n < tam; n++) { // busca cual es su primera posicion vacia a su derecha
			if ((estaVacia(fila, n)) && (libre == false)) {
				libre = true;
				x = n; // mete en x la primera posicion donde esta el 0 de la derecha
			}
		}

		if (libre == true) {
			return x; //devuelve la primera posicion libre desde posIni
		}
		else return tam;
	}
	else { // si es izquierda

		for (int n = posIni - 1; n > -1; n--) { // busca cual es la primera posicion vacia a su izquierda

			if ((estaVacia(fila, n)) && (libre == false)) { // comprueba si la posicion es 0
				libre = true;
				x = n; // mete en x la primera posicion donde esta el 0 de la izquierda
			}

		}
		if (libre == true) {
			return x; //ha encontrado hueco y devuelve la primera posicion libre desde posIni
		}
		else return tam; // no ha encontrado ningun hueco

	}
}


bool excavadora1Dir(tArray &fila, int tam, int posIni, int direccion, int numDespla){ //mueve los bloques hasta el libre
	bool mover = true;
	int hueco;

	for (int z = 0; z < numDespla; z++) { // numero de desplazamientos

		hueco = posHuecoLibreDir(fila, tam, posIni, direccion); // tenemos el hueco libre
		if (hueco == tam) { // si hueco = tam es que no hay hueco
			mover = false; // si no ha encontrado hueco
		}

		else { // si ha encnotrado hueco

			if (direccion == 1) { //mueve todo una posicion a la derecha

				for (hueco; hueco > posIni; hueco--) {
					fila[hueco] = fila[hueco - 1];
				}
				fila[posIni] = PROD_NULO;
				posIni++;


			}

			else { //mueve todo una posicion a la izquierda

				for (hueco; hueco < posIni; hueco++) {
					fila[hueco] = fila[hueco + 1];
				}
				fila[posIni] = PROD_NULO;
				posIni--;

			}




		}

	}
	return mover;

}

void ejecutarExcavadora(tArray &fila, int tam, int &movimientos) { //ejecuta la excavadora llamando a excavadora1Dir
	int posIni, direccion, numDespla;
	cout << "Desde que posicion quieres hacer el desplazamiento? " << endl;
	cin >> posIni;
	while (!esPosValida(tam, posIni)) {
		cout << "Tiene que ser entre 0 y " << tam - 1 << endl;
		cin >> posIni;
	}
	cout << "En que direccion? (-1 izquierda 0 1 derecha)" << endl;
	cin >> direccion;
	while ((direccion != 1) && (direccion != -1)) {
		cout << "La direccion es 1 o -1" << endl;
		cin >> direccion;
	}

	cout << "Cuantos desplazamientos quiere hacer?" << endl;
	cin >> numDespla;
	while (numDespla < 0) {
		cout << "Tiene que ser mayor que cero, repita" << endl;
		cin >> numDespla;
	}
	excavadora1Dir(fila, tam, posIni, direccion, numDespla);
	movimientos++;
	mostrarFila(fila, tam);
	if (filaOrdenada(fila, tam)) {
		cout << "Fila ordenada!!!" << endl;
	}
	else cout << "La fila no esta ordenada" << endl;
}



void menu(int tam, tArray &fila, int &movimientos, bool primera) {
	ifstream fich;
	string ar;
	int opcion, posIni, numDespla;
	cout << "1. - Cargar fila de fichero" << endl;
	cout << "2. - Guarda fila en fichero" << endl;
	cout << "3. - Usar grua eficiente" << endl;
	cout << "4. - Usar excavadora" << endl;
	cout << "0. - Salir" << endl;
	cin >> opcion;
	while (opcion > 4 || opcion < 0) {
		cout << "Tiene que ser entre 1 y 4 (0 para abandonar)" << endl;
		cin >> opcion;
	}
	switch (opcion) {

	case 0:
		cout << "Adios";
		break;

	case 1:
		cout << "Que archivo quiere abrir (ponga.txt)?" << endl;
		cin >> ar;
		fich.open(ar);
		if (fich.is_open()) {
			primera = true;
			leerFilaFich(fich, fila, tam);
			movimientos = 0;
			mostrarFila(fila, tam);
			menu(tam, fila, movimientos, primera);
		}
		else {
			cout << "Error al abrir el archivo" << endl;
			menu(tam, fila, movimientos, primera);
		}
		break;

	case 2:
		if (primera == true) {
			ejecutarGuardarFichero(fila, tam);
		}
		else cout << "Primero tiene que cargar un archivo" << endl;
		menu(tam, fila, movimientos, primera);

		break;

	case 3:
		if (primera == true) {
			ejecutarGrua(tam, fila, movimientos);
			cout << "Llevas " << movimientos << " movimientos" << endl;
			if (filaOrdenada(fila, tam)) {
				cout << "Fila ordenada!!!" << endl;
			}
			else cout << "La fila no esta ordenada" << endl;
		}
		else cout << "Primero tiene que cargar un archivo" << endl;

			menu(tam, fila, movimientos, primera);
		
		break;

	case 4:
		if (primera == true) {
			ejecutarExcavadora(fila, tam, movimientos);
			cout << "Llevas " << movimientos << " movimientos" << endl;
		}
		else  cout << "Primero tiene que cargar un archivo" << endl;
		menu(tam, fila, movimientos, primera);
		break;
	}
}


bool sonPosicionesPosiblesGrua(int tam, int posIni, int posFin, int posSoltar) {
	bool posible = true;
	int distancia;
	distancia = posFin - (posIni - 1);

	if (!esPosValida(tam, posIni)) {
		cout << "No existe esa posicion de inicio" << endl;
		posible = false;
	}

	if (!esPosValida(tam, posFin)) {
		cout << "No existe esa posicion final" << endl;
		posible = false;
	}

	if (posIni > posFin) {
		cout << "La posicion final tiene que ser mayor que la inicial" << endl;
		posible = false;
	}

	if (posSoltar < 0 || posSoltar > tam - 1) {
		cout << "Tienes que soltarlo entre el 0 y " << tam - 1 << endl;
		posible = false;
	}


	if (((distancia + posSoltar) <= tam - 1) && (posible == true)) {
		posible = true;
	}
	return posible; //devuelve posible = true si encaja en el array y si ini es menos k fin.
}

bool esPosibleGrua(const tArray fila, int tam, int posIni, int posFin, int posSoltar) { //comprueba si se pisan
	int distancia;
	int empieza = posIni;
	int suelta = posSoltar;
	bool pisar = true;
	if (sonPosicionesPosiblesGrua(tam, posIni, posFin, posSoltar)) { // si no se sale del rango del array continua
		distancia = posFin - (posIni - 1); // calcula la distancia de lo que quieres mover
		for (int n = 0; n < distancia; n++) {
			if ((!estaVacia(fila, posIni) && !estaVacia(fila, posSoltar)) && (pisar == true)) { //si una de las dos esta vacia o las dos no entra en el if porque siempre se va a poder hacer
				if (((posSoltar <= posFin) && (posFin >= posIni) && (posIni < posSoltar) || (suelta <= empieza) && (posSoltar >= empieza))) { //ROTACION HACIA DERECHA || ROTACION HACIA LA IZQUIERDA
					pisar = true; // es posicion valida si entra en el if de arriba (true)
				}
				else pisar = false; // estra en el false si no es rotacion y detecta que no es posible el movimiento
			}
			posSoltar++;
			posIni++;
		}

	}
	else pisar = false;

	return pisar;
}

/*bool grua(tArray &fila, int tam, int posIni, int posFin, int posSoltar, int &movimientos) {
tArray aux;
int distancia;
int meter = posIni;
distancia = posFin - (posIni - 1);
bool realizar = true;
if (esPosibleGrua(fila, tam, posIni, posFin, posSoltar)) {

movimientos++;

for (int n = 0; n < distancia; n++) {
aux[n] = fila[meter];
fila[meter] = PROD_NULO;
meter++;


}

for (int n = 0; n < distancia; n++) {


if (fila[posSoltar] == 0 || (posSoltar == posIni)) {
fila[posSoltar] = aux[n];
}
posSoltar++;
}

}



else {
cout << "No ha sido posible" << endl;
realizar = false;
}
return realizar;
}

*/

bool filaOrdenada(const tArray fila, int tam) { //Comprueba que el array quede ordenado despues de cada cambio
	tArray aux;
	bool ordenado = true;
	int z = 0;
	for (int n = 0; n < tam; n++) {

		if (!estaVacia(fila, n)) {
			aux[z] = fila[n];
			z++;
		}

		for (int n = 0; n < z - 1; n++) {
			if (aux[n] > aux[n + 1] && ordenado == true) {
				ordenado = false;
			}
		}

	}
	return ordenado;
}





bool grua_VO(tArray &fila, int tam, int posIni, int posFin, int posSoltar, int &movimientos) {
	int empieza = posIni, suelta = posSoltar;
	int distancia;
	distancia = posFin - (posIni - 1);
	bool realizar = true, mismoSitio = false;
	if (esPosibleGrua(fila, tam, posIni, posFin, posSoltar)) { //aqui comprueba si el movimiento es posible hacerlo, es decir, si no se salen de los rangos y no se pisan unos a otros

		movimientos++;


		if (posSoltar >= posIni - distancia - 1 && posSoltar < posIni) { // si rotan hacia la izquierda

			for (int n = 0; n < distancia; n++) { 
				fila[posSoltar] = fila[posIni];
				fila[posIni] = PROD_NULO;
				posIni++;
				posSoltar++;
			}

		}

		else {

			for (int n = 0; n < distancia; n++) { // si rotan hacia la derecha o solo se desplazan por huecos libres 
				if (!estaVacia(fila, posFin)) { // si la posicion que se mueve es un 0, que no la cambie por la del destino
					fila[posFin + (posSoltar - posIni)] = fila[posFin];
				}
				if (empieza == suelta) { // si lo dejas en el mismo sitio solo pone mismoSitio a true y no los pone a 0
					mismoSitio = true;
				}
				else fila[posFin] = PROD_NULO;
				posFin--;
			}
		}

	}

	else {
		cout << "No ha sido posible" << endl;
		realizar = false;
	}

	if (mismoSitio == true) { // si esta en true significa que lo has dejado en el mismo sitio y te lo dice
		cout << "Lo has dejado en el mismo sitio pero te sumo el movimiento" << endl;

	}
	return realizar;
}