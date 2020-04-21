#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hybrid_list.h"

/** Retorna true si ambos strings son iguales */
bool string_equals(char* string1, char* string2)
{
  return !strcmp(string1, string2);
}

void simulate(char* filename)
{
  // Abrimos el archivo
  FILE* file = fopen(filename, "r");

  // Leemos la cantidad de terminales
  int terminal_count;
  fscanf(file, "%i", &terminal_count);

  // TODO: Inicializar estructura principal
  //printf("#terminales: %i\n", terminal_count);
  Terminal** titanic = malloc(terminal_count * sizeof(Terminal*));

  


  for (int term = 0; term < terminal_count; ++term)
      {
        int gate_count;
        fscanf(file, "%i", &gate_count);
        // TODO: Inicializar terminal
        titanic[term] = terminal_init(gate_count, term);
        //printf("terminal #%i: %i puertas\n", term, gate_count);
      }

  char command[32];

  while(true)
  {
    fscanf(file, "%s", command);

    if(string_equals(command, "END"))
    {
      break;
    }
    else if(string_equals(command, "INGRESO"))
    {
      int terminal, passenger_id, priority;

      fscanf(file, "%i %i %i", &terminal, &passenger_id, &priority);

      // TODO: Procesar ingreso
      choice_ingreso(titanic[terminal], passenger_id, priority);
      //printf("Ingreso de pasajero %i a terminal %i con prioridad %i\n", passenger_id, terminal, priority);
    }
    else if(string_equals(command, "ABORDAJE"))
    {
      int terminal, gate;
      fscanf(file, "%i %i", &terminal, &gate);

      // TODO: Procesar abordaje
      escape(titanic[terminal] -> puertas[gate]);
      //printf("Inicio del abordaje en el terminal %i puerta %i\n", terminal, gate);
    }
    else if(string_equals(command, "CIERRE"))
    {
      int terminal, gate;
      fscanf(file, "%i %i", &terminal, &gate);

      // TODO: Procesar cierre de puerta
      cierre(titanic[terminal] -> puertas[gate], titanic[terminal]);
      //printf("Cierre de puerta %i en terminal %i\n", gate, terminal);
    }
    else if(string_equals(command, "CLAUSURA"))
    {
      int term_out, term_in;
      fscanf(file, "%i %i", &term_out, &term_in);

      // TODO: Procesar clausura de terminal
      clausurar(titanic[term_out], titanic[term_in]);
      //printf("Clausura de terminal %i, todos los pasajeros proceder al terminal %i\n", term_out, term_in);
    }
    else if(string_equals(command, "LASER"))
    {
      int terminal, gate, index;
      fscanf(file, "%i %i %i", &terminal, &gate, &index);

      // TODO: Procesar laser
      laser_killer(titanic[terminal] -> puertas[gate], index);
      //printf("Voy a matar al nro %i de la fila, de la puerta %i del terminal %i\n", index, gate, terminal);
      //printf("Laser perdido impacta a la persona en indice %i de la fila para la puerta %i del terminal %i\n", index, gate, index);
    }
  }

  fclose(file);
  //printf("TITANIC LOG\n");
  // TODO: Liberar recursos e imprimir los pasajeros que siguen a bordo
  titanic_log(titanic, terminal_count);
  printf("END LOG\n");
  for (int i = 0; i < terminal_count; ++i)
      {
        terminal_destroy(titanic[i]);
      }
  free(titanic);
}

int main(int argc, char *argv[])
{
  // Este programa recibe dos parámetros:
  //  argv[0] = el programa en sí
  //  argv[1] = la ruta al archivo de input
  if (argc != 2)
  {
    printf("Cantidad de parámetros incorrecta\n");
    printf("Uso correcto: %s PATH_TO_INPUT\n", argv[0]);
    return 1;
  }

  simulate(argv[1]);

  return 0;
}
