#include "hybrid_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/** Para la realización de estos modulos y structs, utilicé como gía
    el Taller de Structs de C vistos en la ayudantía,
    como también los archivos asociados */

/** Inicializa una lista vacía */
Hlist* list_init(int tipo_pasajero)
{
  Hlist* hlist = calloc(1, sizeof(Hlist));
  hlist -> head = NULL;
  hlist -> tail = NULL;
  hlist -> len = 0;
  hlist -> sections = 0;
  hlist -> shifts = 0;
  hlist -> backshifts = 0;
  list_extend(hlist, 0);
  return hlist;
}

Gate* gate_init(int term_id, int puerta_id)
{
  Gate* puerta = calloc(1, sizeof(Gate));
  puerta -> boys = list_init(0);
  puerta -> adults = list_init(1);
  puerta -> robots = list_init(2);
  puerta -> terminal_id = term_id;
  puerta -> gate_id = puerta_id;
  puerta -> state = 1;
  puerta -> pods_counter = 0;
  puerta -> len = 0;
  return(puerta);
}


Terminal* terminal_init(int cant_puertas, int id_term)
    {
      Terminal* nuevo_terminal = calloc(1, sizeof(Terminal));
      nuevo_terminal -> nro_puertas = cant_puertas;
      nuevo_terminal -> puertas = calloc(cant_puertas, sizeof(Gate));
      nuevo_terminal -> state = 1;
      nuevo_terminal -> max_len = 0;
      nuevo_terminal -> id_terminal = id_term;
      for (int i = 0; i < cant_puertas; ++i)
          {
            nuevo_terminal -> puertas[i] = gate_init(id_term, i);
          }
      return(nuevo_terminal);
      
    } 


/** Inserta un nuevo array al final de la lista */
void list_extend(Hlist* hlist, int valor)
{
  Section* new_array = calloc(1, sizeof(Section)); // Creamos una nueva seccion
  new_array -> value = valor; // asignamos el indice del array supongo
  new_array -> next = NULL; // asignamos valor null a la siguiente section de esta seccion
  new_array -> in_array = 0; // no hay valores en este array, asique asignamos 0

  hlist -> sections += 1; // sumamos 1 en el contador de secciones

  // Si la lista está vacía entonces queda como el primer elemento
  if(hlist -> head == NULL)
      {
        hlist -> head = new_array;
      }
  // Sino, se pone como el siguiente del ultimo
  else
      {
        hlist -> tail -> next = new_array;
      }
  // Luego se deja a este nodo como el último
  hlist -> tail = new_array;
}



/** Inserta un nuevo elemento al final de la lista */
void list_add(Hlist* hlist, int value){

  int indice = hlist -> tail -> in_array; // cuantos elementos hay en el ultimo array
  int arrays = hlist -> sections; // cuantas secciones hay actualmente
  if (indice >= 2000){ // si hay 100 elementos en el ultimo array:
    list_extend(hlist, arrays); // extendemos la lista, según la cantidad de arrays que hay + 1 (el siguiente)
  }
  indice = hlist -> tail -> in_array; // obtengo el indice del array en donde quiero agregar el elemento
  hlist -> tail -> actual[indice] = value; // agrego el elemento al array de esa seccion
  hlist -> tail -> in_array += 1; // agregamos uno a la cantidad de elementos que tiene el array
}



/** Obtiene el elemento del indice pedido */
int list_access(Hlist* hlist, int indice){

  int i_section = (int) floor((double) indice / (double) 2000);
  int anteriores = i_section * 2000;
  int indice_final = indice - anteriores;

   for(Section* current = hlist -> head; current; current = current -> next)
      {
        if (current -> value == i_section)
            {
              return (current -> actual[indice_final]);
            }
      }
  return(-1);

}

/** Obtiene el elemento del indice pedido en una puerta */
int* gate_access(Gate* puerta, int indice)
    {
      int largo_boys = puerta -> boys -> len;
      int largo_adults = puerta -> adults -> len;
      if (indice < largo_boys)
          {
            int* tupla = (int*) calloc(2, sizeof(int));
            tupla[0] = 0;
            tupla[1] = list_access(puerta -> boys, indice + puerta -> boys -> shifts);
            return(tupla);
          }

      else if ((indice >= largo_boys) && (indice < (largo_boys + largo_adults)))
          {
            int* tupla = (int*) calloc(2, sizeof(int));
            tupla[0] = 1;
            tupla[1] = list_access(puerta -> adults, indice + (puerta -> adults -> shifts) - largo_boys);
            return(tupla);
          }

      else if ((indice >= (largo_boys + largo_adults)))
          {
            int* tupla = (int*) calloc(2, sizeof(int));
            tupla[0] = 2;
            tupla[1] = list_access(puerta -> robots, indice + (puerta -> robots -> shifts) - largo_boys - largo_adults);
            return(tupla);
          }
      else
          {
            printf("no se encuentra\n");
            int* tupla = (int*) calloc(2, sizeof(int));
            tupla[0] = -1;
            tupla[1] = -1;
            return(tupla);
          }

    }


/** Imprime todos los elementos de la lista */
void list_print(Hlist* hlist)
{
  for(Section* current = hlist -> head; current; current = current -> next)
      {
        for (int i = 0; i < 2000; ++i)
          {
            printf("%i\n", current -> actual[i]);
          }
        
      }
}



// Destruye y libera la memoria de una Section
void section_destroy(Section* seccion)
    {
      free(seccion);
    }



/** Libera todos los recursos asociados a esta lista */
void list_destroy(Hlist* hlist)
{
  // Liberamos los nodos
  if(hlist -> head)
  {
    Section* curr = hlist -> head -> next;
    Section* prev = hlist -> head;

    while(curr)
    {
      section_destroy(prev);
      prev = curr;
      curr = curr -> next;
    }
    section_destroy(prev);
  }
  // Liberamos la lista en si
  free(hlist);
}


void gate_destroy(Gate* puerta)
    {
      list_destroy(puerta -> boys);
      list_destroy(puerta -> adults);
      list_destroy(puerta -> robots);
      free(puerta);
    }

void terminal_destroy(Terminal* term)
    {
      for (int i = 0; i < term -> nro_puertas; ++i)
          {
            gate_destroy(term -> puertas[i]);
          }
          free(term -> puertas);
          // Creo que falta un free aca
          free(term);
    }


// La lista elimina al impactado por el laser, y el resto avanza un lugar
Hlist* laser_shift(Hlist* hlista, int indice, int tipo_pasajero)
    { 
      int shifts_now = hlista -> shifts;
      int a_matar = indice + shifts_now;
      int largo = hlista -> len;
      Hlist* nueva_lista = list_init(tipo_pasajero);
      int largo_nuevo = largo - 1;
      for (int i = shifts_now; i < a_matar; ++i)
          {
            list_add(nueva_lista, list_access(hlista, i));
          }
      for (int i = a_matar + 1; i < largo + shifts_now; ++i)
          {
            list_add(nueva_lista, list_access(hlista, i));
          }
      nueva_lista -> len += largo_nuevo;
      list_destroy(hlista);
      return(nueva_lista);
    }



// Se elimina al impactado por el laser de la puerta
void laser_killer(Gate* puerta, int i_muerto)
    {
      Hlist* lista_nueva;
      int largo_boys = puerta -> boys -> len;
      int largo_adults = puerta -> adults -> len;

      if (i_muerto < (largo_boys))
            {
              lista_nueva = laser_shift(puerta -> boys, i_muerto, 0);
              puerta -> boys = lista_nueva;
              puerta -> len -= 1;
            }

      else if ((i_muerto >= (largo_boys)) && (i_muerto < (largo_boys + largo_adults)))
                {
                  lista_nueva = laser_shift(puerta -> adults, i_muerto - (largo_boys), 1);
                  puerta -> adults = lista_nueva;
                  puerta -> len -= 1;
                }
            else
                {
                  lista_nueva = laser_shift(puerta -> robots, i_muerto - (puerta -> boys -> len) - (puerta -> adults -> len), 2);
                  puerta -> robots = lista_nueva;
                  puerta -> len -= 1;
                }
    }



// Administra el escape de las primeras 8 personas en una Gate
void escape(Gate* puerta)
    {
      printf("POD %i %i %i LOG\n", puerta -> terminal_id, puerta -> gate_id, puerta -> pods_counter);
      int* retornado;
      int* resp_retornado;
      int count_boys = 0;
      int count_adults = 0;
      int count_robots = 0;
      for (int i = 0; i < 8; ++i)
          {
            retornado = gate_access(puerta, i);
            resp_retornado = retornado;
            printf("%i\n", retornado[1]);
            if (retornado[0] == 0)
                {
                  count_boys += 1;
                }
            else if (retornado[0] == 1)
                {
                  count_adults += 1;
                }
            else
                {
                  count_robots += 1;     
                }
            free(resp_retornado);
          }
      puerta -> boys -> shifts += count_boys;
      puerta -> boys -> len -= count_boys;
      puerta -> adults -> shifts += count_adults;
      puerta -> adults -> len -= count_adults;
      puerta -> robots -> shifts += count_robots;
      puerta -> robots -> len -= count_robots;        
      puerta -> pods_counter += 1;
      puerta -> len -= 8;
      
    }


// Elige la puerta indicada para el pasajero que esta ingresando
void choice_ingreso(Terminal* term, int id_pasajero, int tipo_pasajero)
    {
      int menor_largo = 99999999;
      Gate* puerta_conveniente;
      int temporal;
      if (tipo_pasajero == 0)
          {
            for (int i = 0; i < term -> nro_puertas; ++i)
                {
                  temporal = term -> puertas[i] -> boys -> len;
                  if (temporal < menor_largo)
                        {
                          if (term -> puertas[i] -> state == 1)
                            {
                              menor_largo = temporal;
                              puerta_conveniente = term -> puertas[i];
                            }
                        }
                }
            list_add(puerta_conveniente -> boys, id_pasajero);
            puerta_conveniente -> boys -> len += 1;
          }
      else if (tipo_pasajero == 1)
          {
            for (int i = 0; i < term -> nro_puertas; ++i)
                {
                  temporal = (term -> puertas[i] -> boys -> len) + (term -> puertas[i] -> adults -> len);
                  if (temporal < menor_largo)
                      {
                          if (term -> puertas[i] -> state == 1)
                              {
                                 menor_largo = temporal;
                              puerta_conveniente = term -> puertas[i];
                              }
                         
                      }
                }
            list_add(puerta_conveniente -> adults, id_pasajero);
            puerta_conveniente -> adults -> len += 1;
          }
      else
          {
            for (int i = 0; i < term -> nro_puertas; ++i)
            {
              temporal = term -> puertas[i] -> len;
              if (temporal < menor_largo)
                      {
                          if (term -> puertas[i] -> state == 1)
                              {
                                menor_largo = temporal;
                                puerta_conveniente = term -> puertas[i];
                              }
                          
                      }
            }
            list_add(puerta_conveniente -> robots, id_pasajero);
            puerta_conveniente -> robots -> len += 1;
          }
    puerta_conveniente -> len += 1;
    }


// Maneja el cierre de un Gate
void cierre(Gate* puerta, Terminal* term)
    {
      int* tupla;
      puerta -> state = 0;
      for (int i = 0; i < puerta -> len; ++i)
          {
            tupla = gate_access(puerta, i);
            choice_ingreso(term, tupla[1], tupla[0]);
            free(tupla);
          }
    }



// Vamos a hacer el metodo para clausurar un Terminal
void clausurar(Terminal* term, Terminal* term_acoge)
    {
      term -> state = 0; // clausurando el terminal
      int mayor_len = 0; // voy a calcular el mayor largo
      int temporal;
      for (int i = 0; i < term -> nro_puertas; ++i)
          {
                temporal = term -> puertas[i] -> len;
                if (temporal > mayor_len)
                    {
                      if (term -> puertas[i] -> state == 1)
                          {
                            mayor_len = temporal;
                          }
                      
                    }
          }
          // ahora tengo el largo mayor de las puertas del terminal clausurado

      Terminal* nuevo_terminal = terminal_init(mayor_len, -1); // creo un nuevo terminal auxiliar
      Terminal* nuevo_resp = nuevo_terminal;
      // este terminal tiene tantas puertas como elementos tenía la puerta más larga
      int* actual_value;
      for (int j = 0; j < mayor_len; ++j)
          {
            for (int i = 0; i < term -> nro_puertas; ++i)
                  {
                    if (term -> puertas[i] -> state == 1) // Necesito que la puerta original sea valida
                        {
                          if (term -> puertas[i] -> len > j) // necesito que esa puerta tenga ese largo
                              {
                                actual_value = gate_access(term -> puertas[i], j);
                                if (actual_value[0] == 0)
                                    {
                                      list_add(nuevo_terminal -> puertas[j] -> boys, actual_value[1]);
                                      nuevo_terminal -> puertas[j] -> boys -> len += 1;
                                      nuevo_terminal -> puertas[j] -> len += 1;
                                    }
                                else if (actual_value[0] == 1)
                                    {
                                       list_add(nuevo_terminal -> puertas[j] -> adults, actual_value[1]);
                                       nuevo_terminal -> puertas[j] -> adults -> len += 1;
                                       nuevo_terminal -> puertas[j] -> len += 1;
                                    }
                                else
                                    {
                                      list_add(nuevo_terminal -> puertas[j] -> robots, actual_value[1]);
                                      nuevo_terminal -> puertas[j] -> robots -> len += 1;
                                      nuevo_terminal -> puertas[j] -> len += 1;
                                    }
                              free(actual_value);
                                
                              }
                        }
                    
                  }
          }
      Terminal* terminal_final = terminal_init(1, -1); // Creo un terminal auxiliar, con una puerta (la final)
      Terminal* final_resp = terminal_final;
      int valor_temp;
      for (int i = 0; i < mayor_len; ++i) // recorriendo todas las puertas nuevas
          {
            for (int k = 0; k < nuevo_terminal -> puertas[i] -> boys -> len; ++k) // recorriendo los boys de las puertas nuevas
                {
                  valor_temp = list_access(nuevo_terminal -> puertas[i] -> boys, k); // obtengo el id del boy
                  list_add(terminal_final -> puertas[0] -> boys, valor_temp);
                  terminal_final -> puertas[0] -> boys -> len += 1;
                  terminal_final -> puertas[0] -> len += 1;
                }

            for (int k = 0; k < nuevo_terminal -> puertas[i] -> adults -> len; ++k)
                {
                  valor_temp = list_access(nuevo_terminal -> puertas[i] -> adults, k);
                  list_add(terminal_final -> puertas[0] -> adults, valor_temp);
                  terminal_final -> puertas[0] -> adults -> len += 1;
                  terminal_final -> puertas[0] -> len += 1;
                }

            for (int k = 0; k < nuevo_terminal -> puertas[i] -> robots -> len; ++k)
                {
                  valor_temp = list_access(nuevo_terminal -> puertas[i] -> robots, k);
                  list_add(terminal_final -> puertas[0] -> robots, valor_temp);
                  terminal_final -> puertas[0] -> robots -> len += 1;
                  terminal_final -> puertas[0] -> len += 1;
                }
          }
      int id_final_temp;
      
      // No necesito shifts porque los terminales son auxiliares y nuevos
      for (int i = 0; i < terminal_final -> puertas[0] -> boys -> len; ++i)
          {
            id_final_temp = list_access(terminal_final -> puertas[0] -> boys, i);
            choice_ingreso(term_acoge, id_final_temp, 0);
          }

      for (int i = 0; i < terminal_final -> puertas[0] -> adults -> len; ++i)
          {
            id_final_temp = list_access(terminal_final -> puertas[0] -> adults, i);
            choice_ingreso(term_acoge, id_final_temp, 1);
          }

      for (int i = 0; i < terminal_final -> puertas[0] -> robots -> len; ++i)
          {
            id_final_temp = list_access(terminal_final -> puertas[0] -> robots, i);
            choice_ingreso(term_acoge, id_final_temp, 2);
          }
          terminal_destroy(nuevo_resp);
          terminal_destroy(final_resp);
      
    }

// Imprime el TITANIC LOG
void titanic_log(Terminal** terminales, int cant_terminales)
    {
      int largo;
      int* valor;// = (int*) calloc(2, sizeof(int));
      //int* resp_valor = valor;
      printf("TITANIC LOG\n");
      for (int i = 0; i < cant_terminales; ++i)
          {
            if (terminales[i] -> state == 1)
                {
                  printf("TERMINAL %i\n", i);

                  for (int k = 0; k < terminales[i] -> nro_puertas; ++k)
                      {
                        if (terminales[i] -> puertas[k] -> state == 1)
                            {
                              largo = terminales[i] -> puertas[k] -> len;
                              printf("GATE %i: %i\n",terminales[i] -> puertas[k] -> gate_id, largo);
                              for (int p = 0; p < largo ; ++p)
                                  {
                                    valor = gate_access(terminales[i] -> puertas[k], p);
                                    printf("%i\n", valor[1]);
                                    free(valor);
                                  }
                            }
                      }
                }
          }
    }