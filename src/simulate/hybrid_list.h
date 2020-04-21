/*****************************************************************************/
/*                              Lista Ligada     Hibrida                     */
/*****************************************************************************/
/*                                                                           */
/* Lista simplemente ligada para guardar datos de tipo int. Cada nodo tiene  */
/* un puntero al nodo siguiente en la lista. La estructura principal tiene   */
/* un puntero al primer y ultimo elemento de la lista.                       */
/*****************************************************************************/

// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once

// Declaramos el struct para un nodo de la lista
struct linked_list_array;
// Definimos un alias para el struct nodo.
// La sintaxis es "typedef tipo alias"
typedef struct linked_list_array Section;

// Definimos el struct nodo
struct linked_list_array
{
  /** El valor que guarda el nodo */
  int value;
  /** Un puntero al siguiente nodo de la lista */
  int actual[2000];
  Section* next;
  int in_array;


};

// Declaramos el struct para la lista
struct hybrid_list;
// Definimos un alias para el struct lista
typedef struct hybrid_list Hlist;

// Definimos el struct lista
struct hybrid_list
{
  /** El primer nodo de la lista */
  Section* head;
  /** El último nodo de la lista */
  Section* tail;

  /** Es casi el ultimo nodo de la lista */
  Section* almost_tail;

  int len; // Largo de la lista

  int sections; // Cantidad de sub-arrays

  int shifts; // Cantidad de espacios que se ha desplazado la lista

  int backshifts; // cantidad de personas que mueren por laser en la lista

  int tipo_pasajero; // Tipo de pasajeros que tiene esta lista
};


// Declaramos el struct para las Gates
struct s_gate;
// Definimos un alias para el struct gate
typedef struct s_gate Gate;
// Definimos el struct Gate
struct s_gate
{
	Hlist* boys;
	Hlist* adults;
	Hlist* robots;
	int terminal_id;
	int gate_id;
	int state;
	int pods_counter;
	int len;
};


// Declaramos el structr para los Terminales
struct terminales;
// Definimos un alias para el struct terminales
typedef struct terminales Terminal;
// Definimos el struct Terminal
struct terminales
{
	int nro_puertas;
	Gate** puertas;
	int state;
	int id_terminal;
	int max_len;
};


// Declaramos las funciones asociadas

/** Inicializa una lista vacía */
Hlist* list_init(int tipo_pasajero);
/** Inicializa una puerta vacía */
Gate* gate_init(int term_id, int puerta_id);
/** Inicializa un Terminal con sus puertas */
Terminal* terminal_init(int cant_puertas, int id_terminal);
/** Inserta un nuevo elemento al final de la lista */
void list_extend(Hlist* hlist, int valor);
/** Inserta un nuevo elemento al final de la lista */
void list_add(Hlist* hlist, int value);
/** Retorna el elemento pedido mediante indice */
int list_access(Hlist* hlist, int indice);
/** Imprime todos los elementos de la lista */
void list_print(Hlist* hlist);
/** Libera todos los recursos asociados a esta lista */
void list_destroy(Hlist* hlist);
/** Libera todos los recursos asociados a esta puerta */
void gate_destroy(Gate* puerta);
/** Libera todos los recursos asociados a este terminal */
void terminal_destroy(Terminal* term);
/** Va desplazando a cada persona un lugar si estaban detras del impactado por el laser **/
Hlist* laser_shift(Hlist* hlista, int indice, int tipo_pasajero);
/** Se ingresa a una persona en la cola mas conveniente segun el tipo de pasajero **/
void choice_ingreso(Terminal* term, int id_pasajero, int tipo_pasajero);
/** Entrega el valor de un pasajero en cola segun el indice en cola total (Gate index) */
int* gate_access(Gate* puerta, int indice);
/** Clausura un Terminal, y los envía a otro */
void clausurar(Terminal* term, Terminal* term_acoge);
/** Imprime el famoso TITANIC LOG */
void titanic_log(Terminal** terminales, int cant_terminales);
/** Destruye y libera la memoria de una Section */
void section_destroy(Section* seccion);
/** Funcion que ayudad a matar al apuntado con el laser */
void laser_killer(Gate* puerta, int i_muerto);
/** Funcion que asiste el escape de las primeras 8 personas de una puerta */
void escape(Gate* puerta);
/** Asiste el cierre de un Gate */
void cierre(Gate* puerta, Terminal* term);
