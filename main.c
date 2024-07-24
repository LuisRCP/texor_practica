#include "editor.h"

int main(int argc, char *argv[]) {
  // Inicialización y configuración del editor
  enableRawMode();
  if (argc >= 2) {
    editorOpen(argv[1]);
  }
  
  // Bucle principal del editor
  while (1) {
    editorRefreshScreen();
    int c = editorReadKey();
    // Llamadas a las funciones del editor basadas en las teclas presionadas
    // ...
  }
  
  return 0;
}
