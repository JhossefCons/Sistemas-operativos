/**
 * @file
 * @brief Busqueda de archivos.
 * @author Jhossef Nicolas Constain y Julian Felipe piamba
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/**
 * @brief Muestra la ayuda del programa
*/

void usage(char * programName);

/**
 * @brief Muestra si la ruta que se pasó por argumento es un directorio
*/

int es_directorio(char * ruta);

/**
 * @brief Buscara el directorio y contará la cantidad d archivos que hay en el
*/
int buscar(char * dir, char * patron);

int main(int argc, char *argv[]){
    char *dir;
    char *pattern;
   // Verificar si se recibieron los argumentos correctos.
    if(argc != 3){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    // Obtener los argumentos.
    dir = argv[1];
    pattern = argv[2];
    // Buscar archivos y directorios que contengan el patrón y contarlos.
    int total = buscar(dir, pattern);
    printf("Archivos encontrados: %d\n", total);
    exit(EXIT_SUCCESS);
}

int es_directorio(char* ruta){
  
  //Se declara el struct de tipo stat
  struct stat s;

  // Verificar si la ruta es un directorio existente.

  int resultado = stat(ruta, &s);

  // Si ocurre un error, se muestra el mensaje de error y se retorna -1.
  if(resultado < 0){
    perror("stat");
    return 0;
  }
  // Si la ruta es un directorio, retorna 1. En caso contrario, retorna 0.
  if(S_ISDIR(s.st_mode)){
    return 1;
  }
  else{
    return 0;
  }
}

int buscar(char * dir, char * patron){
  // Inicializar una variable "total" para conocer cuantos archivos y directorios tienen el mismo nombre.
  int totalArchivos = 0;

  //Se abre el directorio dir
  DIR *d = opendir(dir);

  //Se declara el struct ent de tipo dirent
  struct dirent *ent;

  //Se declara la cadena que contendra la ruta
  char *ruta;
  
  //Verificará si al abrir no hay nada y dara error si es el caso
  if(d == NULL){
    perror("opendir");
    //retornara el total
    return totalArchivos;
  }

  //Se lee con readdir
  while((ent = readdir(d))!= NULL){

    //Se da la memoria adecuada para almacenar la ruta
    ruta = (char*)malloc(strlen(dir) + strlen(ent->d_name)+2);
    ruta[0] = '\0';

    //Si es nula se dara error y sale del programa
    if(ruta==NULL){
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    // si no es nulo se le concantenan los elementos que corresponden
    strcat(ruta, dir);
    strcat(ruta,"/");
    strcat(ruta,ent->d_name);

    // imprime la ruta si contiene el patron
    if(strstr(ent->d_name, patron)!=NULL){
      printf("%s\n",ruta);
      totalArchivos++;
    }

    // se verifica que la entrada no se aun directorio actual o uno anterior
    if(es_directorio(ruta) && strcmp(ent->d_name,".") != 0 && strcmp(ent->d_name,"..") != 0){
      totalArchivos += buscar(ruta,patron); 
    }
  }
  free(ruta);
  closedir(d);
  return totalArchivos;
}

void usage(char * programName){
  printf("Busqueda de archivos\n");
  printf("Uso: %s DIR PATTERN", programName);  
  printf("\nDIR: Directorio base de busqueda" );
  printf("\nPATTERN: texto contenido en el nombre del archivo/directorio" );
  printf("Busca los archivos y/o directorios dentro de DIR cuyo nombre\ncontiene PATTERN\n" );
}