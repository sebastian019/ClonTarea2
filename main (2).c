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







  return 0;
}