//Jaime Viejo Martinez
//Cristobal Ramos Laina
#ifndef indices_h
#define indices_h
#include "matriz.h"
void crearTabla(tIndicePalabras & tabla, string & archivoInicial, tTotales & totales, tMatriz & L, tMatriz &M, tVector & vector);
void buscarPalabra(tIndicePalabras & tabla, string pal,const  tVector  vector, tTotales totales);


#endif