//Cristobal Ramos Laina
//Jaime Viejo Martinez
#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<Windows.h>
#include<cstring>
#include<conio.h>

using namespace std;

const int MAX = 50;
const int MAXH = 10;
const int MAXE = 100;
typedef enum tCasilla { Libre = ' ', Muro = '#', DestinoL = '.', DestinoC = '*', DestinoJ = '+', Jugador = '@', Caja = '$' };
typedef enum tTecla { arriba = 72, abajo = 80, derecha = 77, izquierda = 75, salir = 27, nada, deshacer = 100 /*68*/ };
typedef tCasilla tTablero[MAX][MAX];

typedef struct {
	tTablero tablero;
	int nfilas;
	int ncolumnas;
	int filaJugador;
	int columnaJugador;
	int numCajas;
	int numCajasEnDestino;
}tSokoban;

typedef struct {
	tSokoban tableros[MAXH];
	int contador;
}tHistoria;

typedef struct {
	tSokoban estado;
	int numMovimientos;
	string nFichero;
	int nivel;
	tHistoria historia;
}tJuego;


typedef struct {

	string nombreArchivo;
	int nivel;
	int movi;
}tPartida;

typedef tPartida tExitos[MAXE];


typedef struct {

	string jugador;
	tExitos exitos;
	int contador;

}tInfo;

void dibujarCasilla(tCasilla casilla);
void dibujar(const tJuego & juego);
void inicializa(tJuego & juego);
bool cargarJuego(tJuego & juego, tPartida &partida, tInfo & info);
bool cargarNivel(ifstream &fichero, tSokoban &sokoban, int nivel, tJuego &juego); // hemos añadido tJuego juego
tCasilla convertir(char level, tJuego &juego, int i, int j);
tTecla leerTecla(int tecla);
bool hacerMovimiento(tJuego &juego, tPartida & partida, tInfo & info);
int pedirMovimiento();
bool bloqueado(const tJuego &juego);
bool guardar(tJuego &juego);
bool deshacerMovimiento(tJuego &juego);
void jugar(tJuego &juego, tPartida & partida, tInfo & info);
void menu(tJuego &juego, tPartida & partida, tInfo & info);
void actualizar(tInfo & info, tPartida & partida);
void buscarInfo(tPartida &partida, tInfo &info);

int main() {
	tJuego juego;
	tTecla tecla;
	tPartida partida;
	tInfo info;
	ifstream archivo;
	ofstream ar;
	string nombre;
	char teclado;
	juego.historia.contador = 0;
	juego.estado.numCajas = 0;
	juego.estado.numCajasEnDestino = 0;
	info.contador = 0;
	cout << "Hola, como te llamas?" << endl;
	cin >> nombre;
	archivo.open(nombre);
	info.jugador = nombre;
	if (archivo.is_open()) { //no es la primera vez que juega
		buscarInfo(partida, info);
	}
	else {//es la primera vez que juega
		cout << "Es la primera vez que juegas, voy a crear un archivo con tu nombre" << endl;
		ar.open(nombre);
	}
	menu(juego, partida, info);
	system("pause");

}



void menu(tJuego &juego, tPartida & partida, tInfo & info) {
	int opcion;
	string vacio;
	ifstream archivo;
	cout << "1- Jugar partida" << endl;
	cout << "2- Informacion de jugadas" << endl;
	cout << "0- Salir" << endl;
	//cin.ignore(256, '\n');  //esto es para boorar el buffer, 256 caracteres hasta llegar a /n que es null
	cin >> opcion;
	while (opcion != 0 & opcion != 1 & opcion != 2) {
		cout << "La opcion no es valida, intruduce 0, 1 o 2: " << endl;
		cin >> opcion;
	}
	switch (opcion) {
	case 1:
		if (cargarJuego(juego, partida, info)) {
			guardar(juego);
			dibujar(juego);
			while (juego.estado.numCajasEnDestino != juego.estado.numCajas) {
				jugar(juego, partida, info);
			}
			if (juego.estado.numCajas != 0) {
				cout << "Enhorabuena, has ganado!" << endl;
				partida.movi = juego.numMovimientos;
				actualizar(info, partida);
			}
			juego.estado.numCajas = 0;
			juego.estado.numCajasEnDestino = 0;
			menu(juego, partida, info);
		}
		break;
	case 2:
		archivo.open(info.jugador);
		if (archivo.is_open()) {
			getline(archivo, vacio);
			if (vacio.length() != 0) { //comprueba que el archivo tenga contenido de esta manera sabe su hay o no informacion
				cout << "Informacion de " << info.jugador << endl;
				cout << "Archivo" << setw(7) << "Nivel" << setw(14) << "Movimientos" << endl;
				for (int n = 0; n < info.contador; n++) {
					cout << info.exitos[n].nombreArchivo << setw(7) << info.exitos[n].nivel << setw(11) << info.exitos[n].movi << endl;
				}
			}
			else cout << "Todavia no tienes informacion de jugadas" << endl;
		}
		else cout << "error al buscar la informacion del jugador" << endl;
		menu(juego, partida, info);
		break;
	case 0: break; //no se necesitaba el system pause
	}
}




void colorFondo(int color) {

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));

}



void dibujarCasilla(tCasilla casilla) {
	switch (casilla) {
	case Libre:
		colorFondo(1); //azul
		cout << "  ";
		break;
	case DestinoC:
		colorFondo(4);
		cout << "[]";
		break;
	case DestinoJ:
		colorFondo(4);
		cout << "00";
		break;
	case Jugador:
		colorFondo(6);
		cout << "00"; //color jugador
		break;

	case DestinoL:
		colorFondo(12); //rojo
		cout << "..";
		break;

	case Muro:
		colorFondo(2); //verde
		cout << "  ";
		break;
	case Caja:
		colorFondo(7);
		cout << "()";
		break;
	}
}

void dibujar(const tJuego & juego) {
	cout << "Fichero: " << juego.nFichero << " Nivel: " << juego.nivel << endl;
	for (int i = 0; i < juego.estado.nfilas; i++) {
		for (int j = 0; j < juego.estado.ncolumnas; j++) {
			dibujarCasilla(juego.estado.tablero[i][j]); // #dibuja el fondo de cada casilla
			colorFondo(0); // #vuelve a poner el fondo en negro
		}
		cout << endl;
	}
	cout << "Numero de movimientos: " << juego.numMovimientos << endl;
}

void inicializa(tJuego & juego) {
	for (int j = 0; j < MAX; j++) {
		for (int i = 0; i < MAX; i++) {
			juego.estado.tablero[j][i] = Libre;
		}
	}
	juego.numMovimientos = 0;
}

bool cargarJuego(tJuego & juego, tPartida &partida, tInfo  & info) {
	inicializa(juego);
	ifstream fichero;
	bool cargado;
	cout << "Que fichero quieres abrir?" << endl;
	cin >> juego.nFichero;
	partida.nombreArchivo = juego.nFichero;

	cout << "Que nivel quieres jugar?" << endl;
	cin >> juego.nivel;
	if (cargarNivel(fichero, juego.estado, juego.nivel, juego)) {
		cargado = true;
		partida.nivel = juego.nivel;
	}
	else {
		cargado = false;
		menu(juego, partida, info);
	}
	return cargado;
}

bool cargarNivel(ifstream &fichero, tSokoban &sokoban, int nivel, tJuego &juego) {
	char aux;
	string level;
	typedef tCasilla tLinea[MAX];
	tLinea linea;
	int n = 0, cont, i = 0;
	int niv;
	bool encontrado = false;
	fichero.open(juego.nFichero);
	if (fichero.is_open()) {
		while (encontrado == false && (!fichero.eof())) { //while (! ficheroEntrada.eof() ){} n <= juego.nivel
			fichero >> aux; //Comprobarlo por que es un #TRIPLE
			if (aux == 'L') {
				fichero >> level;
				fichero >> niv;
				if (niv == juego.nivel) {
					encontrado = true;
					getline(fichero, level);
					getline(fichero, level);
					juego.estado.ncolumnas = level.length();
					while (level.length() != 0) { //comprueba que la cadena sea 0 para acabar de leer
						for (int j = 0; j < level.length(); j++) {
							linea[j] = convertir(level[j], juego, i, j);
							juego.estado.tablero[i][j] = linea[j];
						}
						getline(fichero, level);
						if (level.length() > juego.estado.ncolumnas) { // Guarda la columna mas grande
							juego.estado.ncolumnas = level.length();
						}
						i++; //filas
					}
					if (n > 0) {
						n++;
					}
				}
			}
		}
		if (!encontrado) {
			cout << "error, no se ha encontrado el nivel" << endl;
		}
	}
	else cout << "error, no se ha encontrado el archivo" << endl;
	juego.estado.nfilas = i;
	return encontrado;
}



tCasilla convertir(char level, tJuego &juego, int i, int j) {
	tCasilla linea;
	if (level == '#') {
		return Muro;
	}
	if (level == '@') {
		juego.estado.columnaJugador = j;
		juego.estado.filaJugador = i;
		return Jugador;
	}
	if (level == ' ') {
		return Libre;
	}
	if (level == '.') {
		return DestinoL;
	}
	if (level == '*') {
		return DestinoC;
	}
	if (level == '+') {
		return DestinoJ;
	}
	if (level == '$') {
		juego.estado.numCajas++;
		return Caja;
	}

}



tTecla leerTecla(int tecla) { // HAY QUE COMPROBAR QUE FUNCIONA
	if (tecla == 72) {
		return arriba;
	}
	if (tecla == 80) {
		return abajo;
	}
	if (tecla == 77) {
		return derecha;
	}
	if (tecla == 75) {
		return izquierda;
	}
	if (tecla == 27) {
		return salir;
	}
	if (tecla == 100 || tecla == 68) {
		return deshacer;
	}
	if (tecla != 27 || tecla != 72 || tecla != 80 || tecla != 77 || tecla != 75) {
		return nada;
	}


}

int pedirMovimiento() {
	int movimiento;
	cin.sync();
	movimiento = _getch();
	if (movimiento == 0xe0) {
		movimiento = _getch();
	}
	return movimiento;
}



bool hacerMovimiento(tJuego &juego, tPartida & partida, tInfo & info) {
	tTecla tecla;
	bool mov = false;
	int teclados;
	int fdestino, cdestino, cajadestfila, cajadescolumna;
	teclados = pedirMovimiento();
	tecla = leerTecla(teclados);
	switch (tecla) {
	case arriba: fdestino = juego.estado.filaJugador - 1, cdestino = juego.estado.columnaJugador, cajadestfila = juego.estado.filaJugador - 2, cajadescolumna = juego.estado.columnaJugador;
		break;
	case derecha: fdestino = juego.estado.filaJugador, cdestino = juego.estado.columnaJugador + 1, cajadestfila = juego.estado.filaJugador, cajadescolumna = juego.estado.columnaJugador + 2;
		break;
	case izquierda: fdestino = juego.estado.filaJugador, cdestino = juego.estado.columnaJugador - 1; cajadestfila = juego.estado.filaJugador, cajadescolumna = juego.estado.columnaJugador - 2;
		break;
	case abajo:	fdestino = juego.estado.filaJugador + 1, cdestino = juego.estado.columnaJugador, cajadestfila = juego.estado.filaJugador + 2, cajadescolumna = juego.estado.columnaJugador;
		break;
	case salir: cout << "Adios" << endl; //aqui lo igualamos a 0 para que salga del bucle
		fdestino = juego.estado.filaJugador;
		cdestino = juego.estado.columnaJugador;
		juego.estado.numCajas = 0;
		juego.estado.numCajasEnDestino = 0;
		break;
	case nada: cout << "No has puesto nada" << endl;
		fdestino = juego.estado.filaJugador;
		cdestino = juego.estado.columnaJugador;
		mov = false;
		break;
	case deshacer:
		mov = false;
		break;
	}
	if (tecla != deshacer) {
		switch (juego.estado.tablero[fdestino][cdestino]) {
		case Muro: break;
		case Libre:
			if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == Jugador) {
				mov = true;
				juego.estado.tablero[fdestino][cdestino] = Jugador;
				juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = Libre;
			}
			if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == DestinoJ) {
				mov = true;
				juego.estado.tablero[fdestino][cdestino] = Jugador;
				juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = DestinoL;
			}
			if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == DestinoJ) {
				mov = true;
				juego.estado.tablero[fdestino][cdestino] = Jugador;
				juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = DestinoL;
			}
			break;
			//case Destino: juego.sokoban.tablero[fdestino][cdestino] = destinoj;
		case Caja:
			//cambiar por switch
			if (juego.estado.tablero[cajadestfila][cajadescolumna] == Libre) {
				mov = true;
				juego.estado.tablero[cajadestfila][cajadescolumna] = Caja;
				if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == Jugador) {
					juego.estado.tablero[fdestino][cdestino] = Jugador;
					juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = Libre;
				}
				if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == DestinoJ) {
					juego.estado.tablero[fdestino][cdestino] = Jugador;
					juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = DestinoL;
				}
			}
			if (juego.estado.tablero[cajadestfila][cajadescolumna] == DestinoL) {
				mov = true;
				juego.estado.tablero[cajadestfila][cajadescolumna] = DestinoC;
				juego.estado.tablero[fdestino][cdestino] = Jugador;
				juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = Libre;
				juego.estado.numCajasEnDestino++;
			}

			//else mov = false;
			break;
		case Jugador: break; //salir o nada
		case DestinoL:
			if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == DestinoJ) {
				mov = true;
				juego.estado.tablero[fdestino][cdestino] = DestinoJ;
				juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = DestinoL;
			}
			if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == Jugador) {
				mov = true;
				juego.estado.tablero[fdestino][cdestino] = DestinoJ;
				juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = Libre;
			}
			break;
		case DestinoJ:
			juego.estado.tablero[cajadestfila][cajadescolumna] = Jugador;
			juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = DestinoL;
			break;
		case DestinoC:
			//cambiar por switch
			if (juego.estado.tablero[cajadestfila][cajadescolumna] == Libre) {
				mov = true;
				juego.estado.tablero[cajadestfila][cajadescolumna] = Caja;
				juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = Libre;
				juego.estado.tablero[fdestino][cdestino] = DestinoJ;
				juego.estado.numCajasEnDestino--;
			}
			else if (juego.estado.tablero[cajadestfila][cajadescolumna] == DestinoL) {
				mov = true;
				juego.estado.tablero[cajadestfila][cajadescolumna] = DestinoC;
				if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == DestinoJ) {
					juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = DestinoL;
					juego.estado.tablero[fdestino][cdestino] = DestinoJ;
				}
				if (juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] == Jugador) {
					juego.estado.tablero[juego.estado.filaJugador][juego.estado.columnaJugador] = Libre;
					juego.estado.tablero[fdestino][cdestino] = DestinoJ;
				}
			}
			break;
		}
	}
	else {
		if (deshacerMovimiento(juego) && tecla == deshacer) {
			system("cls");
			dibujar(juego);
		}
	}
	if (mov == true) {
		system("cls");
		juego.estado.filaJugador = fdestino;
		juego.estado.columnaJugador = cdestino;
		guardar(juego);
	}
	return mov;
}



bool bloqueado(const tJuego &juego) {
	bool bloked = false;
	for (int i = 0; i < juego.estado.nfilas; i++) {
		for (int j = 0; j < juego.estado.ncolumnas; j++) {
			if (juego.estado.tablero[i][j] == Caja) {
				if ((juego.estado.tablero[i + 1][j] == Muro) && (juego.estado.tablero[i][j - 1] == Muro)) {
					bloked = true;
				}
				if ((juego.estado.tablero[i + 1][j] == Muro) && (juego.estado.tablero[i][j + 1] == Muro) && bloked == false) {
					bloked = true;
				}
				if ((juego.estado.tablero[i - 1][j] == Muro) && (juego.estado.tablero[i][j + 1] == Muro) && bloked == false) {
					bloked = true;
				}
				if ((juego.estado.tablero[i - 1][j] == Muro) && (juego.estado.tablero[i][j - 1] == Muro) && bloked == false) {
					bloked = true;
				}
			}
		}
	}
	return bloked;
}

bool guardar(tJuego &juego) {
	tSokoban aux;
	bool guardado = false;
	if (juego.historia.contador == 0) { //guarda la PRIMERA jugada en la posicion 0
		guardado = true;
		juego.historia.tableros[0] = juego.estado;
		juego.historia.contador++;
	}
	else {
		if (juego.historia.contador != MAXH) { //para que pare de sumar al llegar al tope de jugadas
			juego.historia.contador++;
		}
		for (int n = juego.historia.contador; n > 0; n--) { // adelanta los tableros una posicion, la posicion 0 y 1 tiene el mismo tablero
			juego.historia.tableros[n - 1] = juego.historia.tableros[n - 2];
		}
		juego.historia.tableros[0] = juego.estado; // mete la nueva jugada en la posicion 0
		guardado = true;

	}



	return guardado;

}


bool deshacerMovimiento(tJuego &juego) {
	bool deshacer;
	if (juego.historia.contador == 0) {
		cout << "No puede retroceder" << endl; //por si lo pide sin empezar a jugar
		deshacer = false;
	}
	else {

		for (int n = 0; n < juego.historia.contador - 1; n++) {

			juego.historia.tableros[n] = juego.historia.tableros[n + 1]; //metes a todos en una posicion mas, se acaba borrando la ultima

		}
		juego.estado = juego.historia.tableros[0]; // el que esta en la posicion cero del paso anterior, lo igualas al tablero actual
		if (juego.numMovimientos != 0) {
			juego.numMovimientos--;
		}
		juego.historia.contador--;
		deshacer = true;
	}
	return deshacer;
}

void jugar(tJuego &juego, tPartida & partida, tInfo & info) {
	if (bloqueado(juego)) {
		cout << "Te has quedado bloqueado" << endl;
		juego.estado.numCajas = 0;
		juego.estado.numCajasEnDestino = 0;
	}
	if (hacerMovimiento(juego, partida, info)) {
		juego.numMovimientos++;
		dibujar(juego);
	}
}

/*void actualizar(tInfo & info, tPartida & partida) {
bool encontrado = false;
ofstream archivo;
ifstream archivoLectura;
archivoLectura.open(info.jugador);
string nombreDearchivo;
int nivelDeArchivo;
archivoLectura >> nombreDearchivo;
while (nombreDearchivo.length() != 0 && encontrado == false) { //comprueba que la cadena sea 0 para acabar de leer
if (nombreDearchivo == partida.nombreArchivo) {
archivoLectura >> nivelDeArchivo;
if (nivelDeArchivo == partida.nivel) {
archivoLectura >> nivelDeArchivo;
encontrado = true;
if (nivelDeArchivo > partida.movi) {
archivo.open(info.jugador);
archivo << partida.nombreArchivo << " " << partida.nivel << " " << partida.movi << endl;

}

}
}
getline(archivoLectura, nombreDearchivo);
getline(archivoLectura, nombreDearchivo);
archivoLectura >> nombreDearchivo;

}
if (encontrado == false) {
archivo.open(info.jugador,std::ios::ate);
archivo << partida.nombreArchivo << " " << partida.nivel << " " << partida.movi << endl;
}
info.exitos[info.contador] = partida;
info.contador++;

tInfo aux;
for (int n = 0; n < info.contador -1; n++) {
if (info.exitos[n].nivel > info.exitos[n + 1].nivel) {
aux.exitos[0] = info.exitos[n + 1];
info.exitos[n + 1] = info.exitos[n];
info.exitos[n] = aux.exitos[0];
}

}

}*/

void actualizar(tInfo & info, tPartida & partida) {
	tInfo aux;
	bool coincide = false;
	for (int n = 0; n < info.contador; n++) {
		if (partida.nombreArchivo == info.exitos[n].nombreArchivo && coincide == false) {
			if (partida.nivel == info.exitos[n].nivel) {
				coincide = true;
				if (partida.movi < info.exitos[n].movi) {
					info.exitos[n] = partida;
				}
			}
		}

	}
	if (coincide == false) {
		info.exitos[info.contador] = partida;
		info.contador++;
	}
	for (int i = 0; i < info.contador; i++) {
		for (int j = 0; j < info.contador - 1; j++) {
			if (info.exitos[j].nombreArchivo > info.exitos[j + 1].nombreArchivo) {
				aux.exitos[0] = info.exitos[j];
				info.exitos[j] = info.exitos[j + 1];
				info.exitos[j + 1] = aux.exitos[0];
			}
		}
	}


	for (int i = 0; i < info.contador; i++) {
		for (int j = 0; j < info.contador - 1; j++) {
			if (info.exitos[j].nombreArchivo == info.exitos[j + 1].nombreArchivo && info.exitos[j].nivel > info.exitos[j + 1].nivel) {
				aux.exitos[0] = info.exitos[j];
				info.exitos[j] = info.exitos[j + 1];
				info.exitos[j + 1] = aux.exitos[0];
			}
		}
	}

	ofstream archivo;
	archivo.open(info.jugador);
	for (int n = 0; n < info.contador; n++) {
		archivo << info.exitos[n].nombreArchivo << " " << info.exitos[n].nivel << " " << info.exitos[n].movi << endl;
	}

}


void buscarInfo(tPartida &partida, tInfo &info) {  //guardo todo lo que hay en el txt en el array de exitos
	info.contador = 0;
	ifstream archivo;
	archivo.open(info.jugador);
	string hola;
	int z;
	archivo >> hola;
	while (hola.length() != 0) {
		info.exitos[info.contador].nombreArchivo = hola;
		archivo >> info.exitos[info.contador].nivel;
		archivo >> info.exitos[info.contador].movi;
		getline(archivo, hola);
		info.contador++;
		archivo >> hola;
	}
}