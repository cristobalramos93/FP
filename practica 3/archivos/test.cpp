#include <iostream>
#include "colores.h"

using namespace std;

int main() {
        
    cout << "yo estoy en color normal\n";
    setColor(Rojo);
    cout << "yo aparezco en color rojo\n";
    setColor(Azul);
    cout << "yo voy de azul\n";
    setColor(15);
    cout << "yo voy de intenso\n";
    setColor(Gris);
    cout << "yo estoy en color normal\n";
    
    return 0;    
}