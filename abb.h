#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* Crea el arbol
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el arbol, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura arbol fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del arbol y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura arbol fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del arbol, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura arbol fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al arbol.
 * Pre: La estructura arbol fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del arbol.
 * Pre: La estructura arbol fue inicializada
 */
size_t abb_cantidad(const abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura arbol fue inicializada
 * Post: La estructura arbol fue destruida
 */
void abb_destruir(abb_t *arbol);

/*Recorre todo el arbol aplicando la funcion visitar. Si en algun momento el resultado
 *de aplicar la funcion visitar devuelve false, se deja de iterar sobre el arbol.
 *Pre: El arbol fue creado
 *Post: Se le aplico la funcion visitar a la cantidad de objetos deseados
 */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


typedef struct abb_iter abb_iter_t;

/*Crea un iterador
 *Pre:El arbol fue creado
 */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/*Avanza el iterador y devuelve true si se pudo hacer la operacion.
 *En caso que apunte al final devuelve false
 *Pre: El iterador fue creado
 *Post: El iterador apunta al siguiente elemento
 */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/*Devuelve la clave actual, esa clave no se puede modificar ni liberar
 *Pre: El arbol fue creado
 */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/*Devuelve true si el iterador esta en el final. En caso contrario devuelve false
 *Pre: El arbol fue creada
 */
bool abb_iter_in_al_final(const abb_iter_t *iter);


/*Destruye el iterador
 *Pre: El arbol fue creada
 *Post: El iterador fue destruido
 */
void abb_iter_in_destruir(abb_iter_t* iter);
