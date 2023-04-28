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
} datInv;   

typedef struct Jugador{
  char nombre[31];
  unsigned short puntos;
  int items;
  List *inventario;
  struct Jugador *historial;
} Jugador;

typedef struct {
  char nombre[31];
} nombres;

void mostrarMenu() {
  puts(BARRA);
  printf("                   SEBA´S ADVENTURE 64\n");
  puts(BARRA);
  printf("\nSeleccione una opción:\n\n1. Crear perfil de jugador\n2. Mostrar "
         "perfil de jugador\n3. Agregar ítem a jugador\n4. Eliminar ítem a "
         "jugador\n5. Agregar puntos de habilidad al jugador\n6. Mostrar "
         "jugadores con un ítem específico\n7. Deshacer última acción del "
         "jugador\n8. Cargar datos de jugadores desde un archivo de texto \n9. "
         "Exportar datos de jugadores a archivo de texto\n0. Salir\n\n");
  puts(BARRA);
}

void listaNombres(List *nombres, char *nombre) {
  datInv *i = (datInv *)malloc(sizeof(datInv));
  strcpy(i->item, nombre);
  pushBack(nombres, i);
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

void agregarItem(HashMap *map, char *nombre, char *item) {
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else {
      // copio los datos del jugador en su historial
      Jugador *aCopiar = ((Jugador *)casilla->value); 
      Jugador *historial = (Jugador*)malloc(sizeof(Jugador));
      memcpy(historial, aCopiar, sizeof(Jugador));
      ((Jugador *)casilla->value)->historial = historial;
      historial->inventario=createList();
      datInv *item=(datInv*)firstList(aCopiar->inventario);
    
      while(item!=NULL){ 
        datInv *aux2=(datInv*) malloc(sizeof(datInv));
        memcpy(aux2,item,sizeof(datInv));
        pushFront(historial->inventario,aux2);
        item=nextList(aCopiar->inventario);
      ///////////////////////
      }
    }
    
    datInv *i = (datInv *)malloc(sizeof(datInv));
    if (firstList(((Jugador *)casilla->value)->inventario) == NULL) {
      ((Jugador *)casilla->value)->inventario = createList();
    }
  
    strcpy(i->item, item);
    pushBack(((Jugador *)casilla->value)->inventario, i);
    ((Jugador *)casilla->value)->items++;
}

void eliminarItem(HashMap *map, char * nombre) {
  
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else {
    // copio los datos del jugador en su historial
    Jugador *aCopiar = ((Jugador *)casilla->value); 
    Jugador *historial = (Jugador*)malloc(sizeof(Jugador));
    memcpy(historial, aCopiar, sizeof(Jugador));
    ((Jugador *)casilla->value)->historial = historial;
    historial->inventario=createList();
    datInv *item=(datInv*)firstList(aCopiar->inventario);
    while(item!=NULL){
      datInv *aux2=(datInv*) malloc(sizeof(datInv));
      memcpy(aux2,item,sizeof(datInv));
      pushFront(historial->inventario,aux2);
      item=nextList(aCopiar->inventario); 
    ////////////////////////
    }
    char nombre2[31];
    printf("Ingrese el nombre del item a eliminar\n");
    scanf(" %[^\n]s", nombre2);
    for (char *a = firstList(((Jugador *)casilla->value)->inventario); a != NULL; a = nextList(((Jugador *)casilla->value)->inventario)) {
      if (strcmp(nombre2, a) == 0) {
        popCurrent(((Jugador *)casilla->value)->inventario);
        ((Jugador *)casilla->value)->items--;
      }
    }
  }
}


void agregarPuntos(HashMap *map, char * nombre) {
  Pair *casilla = searchMap(map, nombre);

  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else {
      // copio los datos del jugador en su historial
      Jugador *aCopiar = ((Jugador *)casilla->value); 
      Jugador *historial = (Jugador*)malloc(sizeof(Jugador));
      memcpy(historial, aCopiar, sizeof(Jugador)); 
      ((Jugador *)casilla->value)->historial = historial;
      historial->inventario=createList();
      datInv *item=(datInv*)firstList(aCopiar->inventario);
      while(item!=NULL){
        datInv *aux2=(datInv*) malloc(sizeof(datInv));
        memcpy(aux2,item,sizeof(datInv));
        pushFront(historial->inventario,aux2);
        item=nextList(aCopiar->inventario); 
      }
    /////////////////
    unsigned short puntos;
    printf("Ingrese la cantidad de puntos\n");
    scanf("%hu", &puntos);
    ((Jugador *)casilla->value)->puntos += puntos;
  }
}

void mostrarJugadoresItemEsp(HashMap *map, List *nombres, char *nombre) {
  int cont = 0;

  printf("Jugadores con el item %s: \n", nombre);
  for (char *a = firstList(nombres); a != NULL; a = nextList(nombres)) {
    long key = hash(a, map->capacity);
    for (char *b = firstList(((Jugador *)map->buckets[key]->value)->inventario);
         b != NULL;
         b = nextList(((Jugador *)map->buckets[key]->value)->inventario)) {
      if (strcmp(b, nombre) == 0) {
        printf("[ %s ]\n", a);
        cont++;
        break;
      }
    }
  }
  if (cont == 0)
    printf("No se encontraron jugadores con ese item\n");
}

void deshacer(HashMap *map, char * nombre){
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else if(((Jugador *)casilla->value)->historial!=NULL){
    
    Jugador *version_anterior =(Jugador*)malloc(sizeof(Jugador)); 
    Jugador *historial = ((Jugador *)casilla->value)->historial;
    memcpy(version_anterior, historial, sizeof(Jugador));
    casilla->value = version_anterior;
  }
  else{
    printf("no hay nada que deshacer\n");
  }
}


const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }

    return NULL;
}

void importar(HashMap *map,FILE *archivo){
  char linea[1300];
  char *aux;
  int i, edadInt;
fgets(linea, 1024, archivo);
  while(fgets(linea, 1024, archivo) != NULL){ 
    Jugador* p = (Jugador*) malloc(sizeof(Jugador));
    p->inventario = createList();
    for(i = 0 ; i < 4 ; i++){
        aux = get_csv_field(linea, i);
    }
  }
  fclose(archivo);
}

void exportar(List *nombres,HashMap *map,FILE *archivo){
  fprintf(archivo, "Nombre,Puntos,Cantidad de Items,Items\n");
  for (char *a = firstList(nombres); a != NULL; a = nextList(nombres)){
    int key = hash(a, map->capacity);
    fprintf(archivo, "%s,%u,%u", ((Jugador*)map->buckets[key]->value)->nombre,((Jugador*)map->buckets[key]->value)->puntos, ((Jugador*)map->buckets[key]->value)->items);
    for (char *b = firstList(((Jugador *)map->buckets[key]->value)->inventario) ; b != NULL; b = nextList(((Jugador *)map->buckets[key]->value)->inventario)){
      fprintf(archivo, ",%s", b);
      if(nextList(((Jugador*)map->buckets[key]->value)->inventario) != NULL){
        fprintf(archivo,", ");
      }
      else{
        fprintf(archivo, "\n");
      }
    }
  }
  fclose(archivo);
}

int main(){
  HashMap *map = createMap(10000);
  char nombre[31];
  char item[31];
  List *nombres = createList();
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
      mostrarPerfil(nombre, map);
    }
    if (numIngresado == 3) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      printf("Ingrese el nombre del item\n");
      scanf(" %[^\n]s", item);
      agregarItem(map,nombre,item);
    }
    if (numIngresado == 4) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      eliminarItem(map,nombre);
    }
    if (numIngresado == 5) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      agregarPuntos(map,nombre);
    }
    if (numIngresado == 6) {
      printf("Ingrese el nombre del item\n");
      scanf(" %[^\n]s", nombre);
      mostrarJugadoresItemEsp(map, nombres, nombre);
    }
    if (numIngresado == 7) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      deshacer(map,nombre);
    }
    if (numIngresado == 8) {
      printf("Escriba el nombre del archivo\n");
      scanf(" %[^\n]", nombreArchivo);
      setbuf(stdout, NULL);

      FILE *archivo = fopen(nombreArchivo,"rt");
      if (archivo == NULL) {
        printf("* Error al abrir el archivo.\n");
      } else{
        importar(map, archivo,nombres);
      }
    }
    if (numIngresado == 9) {
      if(map->buckets != NULL){
        printf("Escriba el nombre del archivo\n");
        scanf(" %[^\n]", nombreArchivo);
        FILE* archivo = fopen(nombreArchivo, "a");
        exportar(nombres,map, archivo);
      } 
      else {
        printf("* No hay datos exportables\n");
      }
    } 

  }
}