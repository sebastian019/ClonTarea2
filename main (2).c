#include "hashmap.h"
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BARRA "-------------------------------------------------------"

typedef struct {
  char item[31];
} inventario;

typedef struct Jugador{
  char nombre[31];
  unsigned short puntos;
  int items;
  List *inventario;
} Jugador;

int main(){
  HashMap *map = createMap(10000);
  char nombre[31];
  char item[31];
  unsigned short numIngresado;
  int largoName;

  while (true) {

    scanf("%hu", &numIngresado);
    while (numIngresado > 9 || numIngresado < 0) {
      printf("Ingrese un número válido \n");
      scanf("%hu", &numIngresado);
    }
    if (numIngresado == 0) {
      printf("\n");
      puts(BARRA);
      printf("Fin De La Aventura!!!!!\n");
      puts(BARRA);
      return 0;
    }
    if (numIngresado == 1) {
    }

    if (numIngresado == 2) {
    }
    if (numIngresado == 3) {
      
    }
    if (numIngresado == 4) {

    }
    if (numIngresado == 5) {

    }
    if (numIngresado == 6) {

    }
    if (numIngresado == 7) {

    }
    if (numIngresado == 8) {

    }
    if (numIngresado == 9) {

    } 

  }
}