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

void mostrarMenu() {
  puts(BARRA);
  printf("                   SEBA´S ADVENTURE 64\n");
  puts(BARRA);
  printf("\nSeleccione una opción:\n\n1. Crear perfil de jugador\n2. Mostrar "
         "perfil de jugador\n3. Agregar ítem a jugador\n4. Eliminar ítem a "
         "jugador\n5. Agregar puntos de habilidad al jugador\n6. Mostrar "
         "jugadores con un ítem específico\n7. Deshacer última acción del "
         "jugador\n8. Exportar datos de jugadores a archivo de texto\n9. "
         "Cargar datos de jugadores desde un archivo de texto\n0. Salir\n\n");
  puts(BARRA);
}

void crearPerfil(Jugador *player, HashMap *map, List *nombres, char *nombre) {
  strcpy(player->nombre, nombre);
  player->puntos = 0;
  insertMap(map, player->nombre, player);
}

void mostrarItems(Pair *casilla) {
  List *node = ((Jugador *)casilla->value)->inventario;
  unsigned short cont = 1;

  if (firstList(node) == NULL) {
    printf("No tiene\n");
    return;
  }
  for (char *a = firstList(node); a != NULL; a = nextList(node)) {
    printf("Item %hu: %s\n", cont, a);
    cont++;
  }
}

void mostrarPerfil(char *nombre, HashMap *map) {
  printf("\n");
  puts(BARRA);
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  }
  else { 
    printf("Nombre: %s\nPuntos: %hu\nItems: %i\n", ((Jugador *)casilla->value)->nombre,((Jugador *)casilla->value)->puntos, ((Jugador *)casilla->value)->items);
    if (((Jugador *)casilla->value)->items == 0) {
      printf("El inventario esta vacío\n");
    } else {
      mostrarItems(casilla);
    }
  }
}

void agregarItem(HashMap *map){
  char clave[31], aux[31];
  printf("Ingrese el nombre del jugador\n");
  scanf(" %[^\n]s", clave);
  Pair *casilla = searchMap(map,clave);
  if(casilla == NULL){
    printf("El jugador ingresado no existe\n");
    return;
  }
  else{
    printf("Ingrese el nombre del item\n");
    scanf(" %[^\n]s", aux);
    if(firstList(((Jugador*)casilla->value)->inventario) == NULL){
      ((Jugador*)casilla->value)->inventario = createList();
      inventario* i = (inventario *) malloc (sizeof(inventario));
      strcpy(i->item,aux);
      pushBack(((Jugador *)casilla->value)->inventario,i);
      ((Jugador *)casilla->value)->items++;
    }
  }
}

int main(){
  HashMap *map = createMap(10000);
  char nombre[31];
  char item[31];
  unsigned short numIngresado;
  int largoName;

  while (true) {
    mostrarMenu();

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
      Jugador *player = (Jugador *)malloc(sizeof(Jugador));
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]", nombre);
      largoName = strlen(nombre);
      while (largoName > 31 || largoName < 1) {
      printf("Ingrese un nombre válido (hasta 30 caracteres)\n");
      scanf(" %[^\n]", nombre);
      largoName = strlen(nombre);
      }
      crearPerfil(player, map, nombres, nombre);
    }
    if (numIngresado == 2) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      //mostrarPerfil(nombre, map);
    }
    if (numIngresado == 3) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      printf("Ingrese el nombre del item\n");
      scanf(" %[^\n]s", item);
      //agregarItem(map,nombre,item);
    }
    if (numIngresado == 4) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      //eliminarItem(map,nombre);
    }
    if (numIngresado == 5) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      //agregarPuntos(map,nombre);
    }
    if (numIngresado == 6) {
      printf("Ingrese el nombre del item\n");
      scanf(" %[^\n]s", nombre);
      //mostrarJugadoresItemEsp(map, nombres, nombre);
    }
    if (numIngresado == 7) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      //deshacer(map,nombre);
    }
    if (numIngresado == 8) {

    }
    if (numIngresado == 9) {

    } 

  }
}