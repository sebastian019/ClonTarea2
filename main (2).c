#include "hashmap.h"
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BARRA "-------------------------------------------------------"


//Se definen las estructuras de datos y TDA usadas en el programa.
typedef struct {//Lista de Items para cada jugador
  char item[31];
} datInv;           

typedef struct {//Mapa que almacena los nombres de todos los jugadores que usa como clave los nombres de los items.
  char nombre[31];
} nombres;

typedef struct Jugador{//Mapa que contiene todos los datos del perfil de un jugador y se accede a traves del nombre del jugador.
  char nombre[31];
  unsigned short puntos;
  int items;
  List *inventario;
  struct Jugador *historial;
} Jugador;


void mostrarMenu() {//Función que se encarga de desplegar el menu cada vez que se ingresa al programa.
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

void listaNombres(List *nombres, char *nombre) {//Esta se encarga de crear o actualizar la lista de nombres cada vez que se ingrese un nuevo jugador. Esta lista tiene usos principalmente a la hora de recorrer el mapa eficientemente, sin la necesidad de pasar por los espacios NULL.
  datInv *i = (datInv *)malloc(sizeof(datInv));
  strcpy(i->item, nombre);
  pushBack(nombres, i);
}

void crearPerfil(Jugador *player, HashMap *map, List *nombres, char *nombre) {//Funcion que recibe como argumentos principales(ademas del struct del jugador y el map) el nombre ingresado por el usuario para crearle su buckets correspondiente y la Lista de nombres para actualizarla. 
  strcpy(player->nombre, nombre);
  player->puntos = 0;
  insertMap(map, player->nombre, player);
  listaNombres(nombres, nombre);//Se actualiza la lista de nombres
  printf("\n* Perfil creado con éxito\n");
}

void mostrarItems(Pair *casilla) {//Funcion que recibe el nodo encontrado en mostrarPerfil() e inicializa un nodo para el recorrido mas facil y efectivo de la lista de Items.
  List *node = ((Jugador *)casilla->value)->inventario;
  unsigned short cont = 1;

  if (firstList(node) == NULL) {//Validacion de que existe la lista.
    printf("No tiene\n");
  }
  
  for (char *a = firstList(node); a != NULL; a = nextList(node)) {//Recorrido de la lista de Items que muestra en pantalla los nombres de todos los elementos de la lista.
    printf("Item %hu: %s\n", cont, a);
    cont++;
  }
}

void mostrarPerfil(char *nombre, HashMap *map) {//Se encarga de Mostrar todos los elementos de el jugador ingreado por el usuario.
  printf("\n");
  puts(BARRA);
  Pair *casilla = searchMap(map, nombre);
  
  if (casilla == NULL) {//Validacion de que existe el jugador
    printf("\n* El jugador ingresado no existe\n");
    return;
  }
  else {
    printf("Nombre: %s\nPuntos: %hu\nItems: %i\n", ((Jugador *)casilla->value)->nombre,((Jugador *)casilla->value)->puntos, ((Jugador *)casilla->value)->items);
    
    if (((Jugador *)casilla->value)->items == 0) {//Validacion de que el jugador posee mínimo un item.
      printf("El inventario esta vacío\n");
    } else {
      mostrarItems(casilla);
    }
  }
}

//Funcion que agrega un item la lista de un jugador especifico.
//Ademas de guardar en el historial lo realizado, en caso de que se quiera usar la funcion deshacer
void agregarItem(HashMap *map, char *nombre, char *item) {
  Pair *casilla = searchMap(map, nombre);//Se realiza una busqueda del jugador ingresado.
  
  if (casilla == NULL) {
    printf("\n* El jugador ingresado no existe\n");
    return;
  } 
  else {
    // copia los datos del jugador en su historial
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
  }
    
  datInv *i = (datInv *)malloc(sizeof(datInv));
  if (firstList(((Jugador *)casilla->value)->inventario) == NULL) {
    ((Jugador *)casilla->value)->inventario = createList();
  }
  
  strcpy(i->item, item); //Se ingresa el valor del argumento item dentro del struct local para luego ingresarlo a la lista a traves de un pushBack. Finalmente se incrementa la cantidad del elemento Items(no confundir con la lista).
  pushBack(((Jugador *)casilla->value)->inventario, i);
  ((Jugador *)casilla->value)->items++;
  printf("\n* Ítem agregado con éxito\n");
}
//Elimina un Item del inventario del jugador ingresado.
//Ademas de guardar en el historial lo eliminado, en caso de que se use la funcion deshacer
void eliminarItem(HashMap *map, char * nombre) {
  Pair *casilla = searchMap(map, nombre);
  
  if (casilla == NULL) {
    printf("\n* El jugador ingresado no existe\n");
    return;
  } 
  else {
    Jugador *aCopiar = ((Jugador *)casilla->value); 
    Jugador *historial = (Jugador*)malloc(sizeof(Jugador));
    
    memcpy(historial, aCopiar, sizeof(Jugador));
    
    ((Jugador *)casilla->value)->historial = historial;
    historial->inventario=createList();
    datInv *item=(datInv*)firstList(aCopiar->inventario);
    
    while(item!=NULL) {
      datInv *aux2=(datInv*) malloc(sizeof(datInv));
      memcpy(aux2,item,sizeof(datInv));
      pushFront(historial->inventario,aux2);
      item=nextList(aCopiar->inventario); 
    }
    
    char nombre2[31];
    printf("Ingrese el nombre del item a eliminar\n");
    scanf(" %[^\n]s", nombre2);
    
    for (char *a = firstList(((Jugador *)casilla->value)->inventario); a != NULL; a = nextList(((Jugador *)casilla->value)->inventario)) { //Se recorre la lista de Inventario hasta encontrar el item con el nombre coincidente al que especifico el usuario y se procede a desenlazarlo de la lista con la funcion popCurrent().
      if (strcmp(nombre2, a) == 0) {
        popCurrent(((Jugador *)casilla->value)->inventario);
        ((Jugador *)casilla->value)->items--;
        printf("\n* Ítem eliminado con éxito\n");
      }
    }
  }
}

//Esta funcion agrega puntos al jugador ingresado
//Ademas de guardar en el historial lo realizado, en caso de que se quiera usar la funcion deshacer
void agregarPuntos(HashMap *map, char * nombre) { 
  Pair *casilla = searchMap(map, nombre);

  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } 
  else {
    Jugador *aCopiar = ((Jugador *)casilla->value); 
    Jugador *historial = (Jugador*)malloc(sizeof(Jugador));
    
    memcpy(historial, aCopiar, sizeof(Jugador)); 
    ((Jugador *)casilla->value)->historial = historial;
    historial->inventario=createList();
    datInv *item=(datInv*)firstList(aCopiar->inventario);
    
    while(item!=NULL) {
      datInv *aux2=(datInv*) malloc(sizeof(datInv));
      memcpy(aux2,item,sizeof(datInv));
      pushFront(historial->inventario,aux2);
      item=nextList(aCopiar->inventario); 
    }

  unsigned short puntos;
  printf("Ingrese la cantidad de puntos\n");
  scanf("%hu", &puntos);
  ((Jugador *)casilla->value)->puntos += puntos;
  printf("\n* Puntos agregados con éxito\n");
  }
}
//esta funcion muestra todos los jugadores con un item especifico, se ingresa el mapa, la lista de nombres para poder tener las claves y el item a buscar
void mostrarJugadoresItemEsp(HashMap *map, List *nombres, char *nombre) { 
  int cont = 1;

  printf("Jugadores con el item %s: \n", nombre);
  for (char *a = firstList(nombres); a != NULL; a = nextList(nombres)) {
    Pair *aux = searchMap(map, a);
    
    for (char *b = firstList(((Jugador *)aux->value)->inventario) ; b != NULL ; b = nextList(((Jugador *)aux->value)->inventario)){
      if (strcmp(b, nombre) == 0){
        printf("%d.[ %s ]\n",cont, a);
        cont++;
        break;
      }
    }
  }
  if (cont == 1) {
    printf("\n* No se encontraron jugadores con ese item\n");
  }
}

void deshacer(HashMap *map, char * nombre){
  Pair *casilla = searchMap(map, nombre);
  
  if (casilla == NULL) {
    printf("\n* El jugador ingresado no existe\n");
    return;
  } 
  if (((Jugador *)casilla->value)->historial != NULL) {
    Jugador *version_anterior =(Jugador*)malloc(sizeof(Jugador)); 
    Jugador *historial = ((Jugador *)casilla->value)->historial; 
    memcpy(version_anterior, historial, sizeof(Jugador)); 
    casilla->value = version_anterior;
    printf("\n* Última acción deshecha con éxito\n");
  }
  else {
    printf("\n* No hay acciones para deshacer\n");
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

void eliminarSOH(char* cadena) { //Elimina el caracter de control 15, ya que este genera problemas con la Exportacion de el ultimo caracter de cada lista.
  char* ptr = cadena;
  
  while (*ptr) {
    if (*ptr == '\015') {
        // Desplaza los caracteres a la izquierda
        memmove(ptr, ptr + 1, strlen(ptr + 1) + 1);
    } 
    else ptr++;
  }
}

void importar(HashMap *map,FILE *archivo,List *nombres){
  char linea[1300];
  char *aux;
  int i, edadInt;
  int aux2;
fgets(linea, 1024, archivo); //Se lee la primera linea por separado ya que no se pretende guardar su valor.
  
  while(fgets(linea, 1024, archivo) != NULL){ //Se leen todas las lineas en orden
    Jugador* p = (Jugador*) malloc(sizeof(Jugador));
    p->inventario = createList();
    
    for(i = 0 ; i < 4 ; i++){
      aux = (char *)get_csv_field(linea, i); //aux se convierte en la linea de caracteres i-esima para rellenar el valor correspondiente.
      if(i == 0){
        strcpy(p->nombre, aux);
        listaNombres(nombres, aux);
      }
      if(i == 1){
        p->puntos = atol(aux);
      }
      if(i == 2){
        aux2 = atol(aux);
        insertMap(map, p->nombre, p);
      }
      if(i == 3){
        for (int k = 0 ; k < aux2; k++){
          eliminarSOH(aux); //Desplaza los caracteres a la izquierda
          agregarItem(map, p->nombre, aux);
          i++;
          aux = (char *)get_csv_field(linea, i);
        }
      }
    }
  }
  fclose(archivo);
}

void exportar(List *nombres,HashMap *map,FILE *archivo){ // esta funcion exporta todos los prefiles creados a un archivo de texto
  fprintf(archivo, "Nombre,Puntos,Cantidad de Items,Items\n");
  
  for (char *a = firstList(nombres); a != NULL; a = nextList(nombres)){
    Pair *aux = searchMap(map,a);
    fprintf(archivo, "%s,%u,%u", ((Jugador*)aux->value)->nombre,((Jugador*)aux->value)->puntos, ((Jugador*)aux->value)->items);
    
    if ((((Jugador *)aux->value)->inventario) == NULL) { // se verifica si el inventario esta vacio, si esta vacio hace un salto de linea
      fprintf(archivo, "\n");
    }
   
    for (char *b = firstList(((Jugador *)aux->value)->inventario) ; b != NULL ; b = nextList(((Jugador *)aux->value)->inventario)){
      fprintf(archivo,",%s", b);

      char *c = nextList(((Jugador*)aux->value)->inventario);
      prevList(((Jugador*)aux->value)->inventario);
      
      if (c == NULL) {
        fprintf(archivo, "\n");
        break;
      } 
    }
  }
  fclose(archivo);
}

int main(){
  HashMap *map = createMap(10002); 
  char nombre[31];
  char item[31];
  char nombreArchivo[31];
  List *nombres = createList(); 
  unsigned short numIngresado;
  int largoName;

  while (true) {
    mostrarMenu();
    
    if ((map->size/map->capacity)*100 > 70){
      enlarge(map);
    }
    scanf("%hu", &numIngresado);
    
    while (numIngresado > 9 || numIngresado < 0) {
      printf("Ingrese un número válido \n");
      scanf("%hu", &numIngresado);
    }
    
    if (numIngresado == 0) {
      printf("\n");
      puts(BARRA);
      printf("                  FIN DE LA AVENTURA\n");
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
      printf("Ingrese el nombre del ítem\n");
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
        printf("\n* Error al abrir el archivo.\n");
      } 
      else{
        importar(map, archivo,nombres);
        printf("\n* Archivo importado con éxito\n");
      }
    }
    if (numIngresado == 9) {
      if(map->buckets != NULL){
        printf("Escriba el nombre del archivo\n");
        scanf(" %[^\n]", nombreArchivo);
        FILE* archivo = fopen(nombreArchivo, "w");
        exportar(nombres,map, archivo);
        printf("\n* Datos exportados con éxito\n");
      } 
      else {
        printf("\n* No hay datos exportables\n");
      }
    }
  }
}