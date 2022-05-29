#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct nodo{
    struct nodo* izq;
    struct nodo* der;
    char* clave;
    void* dato;
}nodo_t;

struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_destruir_dato_t funcion_destruir;
    abb_comparar_clave_t funcion_comparar;
};

nodo_t* crear_nodo(abb_destruir_dato_t f_destruir, const char *clave, void *dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL){
        return NULL;
    }
    nodo->clave = malloc((strlen(clave)+1)*sizeof (char));
    if (nodo->clave == NULL) {
	    free(nodo);
	    return NULL;
	}
    strcpy(nodo->clave,clave);
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

void destruir_nodo(nodo_t *a_borrar) {
    free(a_borrar->clave);
    free(a_borrar);
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(abb == NULL){
        return NULL;
    }
    abb->cantidad = 0;
    abb->funcion_destruir = destruir_dato;
    abb->funcion_comparar= cmp;
    abb->raiz = NULL;
    return abb;
}

// Devuelve una referencia al puntero del nodo(nodo_t **) en caso de encontrar coincidencia con la clave, NULL caso contrario 
nodo_t **_buscar_referencia_nodo(nodo_t **ref,const char *clave, const abb_t * arbol){
    if (*ref == NULL) {
        return ref;
    }
    int aux = arbol->funcion_comparar(clave,(*ref)->clave);
    if (aux == 0) {
            return ref;
    }
    if (aux < 1) {
            return _buscar_referencia_nodo(&((*ref)->izq),clave,arbol);
    }
    return _buscar_referencia_nodo(&((*ref)->der),clave,arbol);
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t **ref = _buscar_referencia_nodo(&(arbol->raiz),clave,arbol);
    if (*ref == NULL) {
        nodo_t * nuevo = crear_nodo(arbol->funcion_destruir,clave,dato);
        if (nuevo == NULL) {
            return false;
        }
        (*ref) = nuevo;
        arbol->cantidad++;
    }
    else {
        (*ref)->dato = dato;
    }
    return true;
}

/* Si tiene 0 o 1 hijo borra el nodo y devuelve true, encaso contrario devuelve false */
void *_borrar_con_0_1_hijo(nodo_t **ref) {
        nodo_t *a_borrar = *ref;
        void *dato= a_borrar->dato;
        if ((*ref)->izq == NULL && (*ref)->der == NULL) {
            *ref = NULL;
        }
        else if ((*ref)->izq == NULL && (*ref)->der != NULL) {
            (*ref) = (*ref)->der;
        }
        else if ((*ref)->izq != NULL && (*ref)->der == NULL) {
            (*ref) = (*ref)->izq;
        }
        destruir_nodo(a_borrar);
        return dato;
}
void _intercambiar_nodos(nodo_t *nodoA, nodo_t *nodoB) {
        char *clave = nodoA->clave;
        void *dato = nodoA->dato;
        nodoA->clave = nodoB->clave;
        nodoA->dato = nodoB->dato;
        nodoB->clave = clave;
        nodoB->dato = dato;
}

void *_borrar_con_2_hijos(nodo_t **ref) {
	nodo_t *actual = (*ref);
	(*ref) = (*ref)->der;
	while ( (*ref)->izq != NULL) {
			(*ref) = (*ref)->izq;
    }
    _intercambiar_nodos(actual,*ref);
    return _borrar_con_0_1_hijo(ref);
}

void *abb_borrar(abb_t *arbol, const char *clave){
    void *dato = NULL;
    nodo_t **ref=_buscar_referencia_nodo(&(arbol->raiz),clave, arbol);
    if ((*ref) == NULL) {
        return NULL;
    }
    if ((*ref)->izq == NULL || (*ref)->der == NULL) {
        dato = _borrar_con_0_1_hijo(ref);
    }
	else {
		dato = _borrar_con_2_hijos(ref);
    }
    return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){ 
    nodo_t **ref = _buscar_referencia_nodo((nodo_t **)&(arbol->raiz),clave,arbol);
    return ((*ref) == NULL) ? NULL : (*ref)->dato;

}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_t **ref = _buscar_referencia_nodo((nodo_t **)&(arbol->raiz),clave,arbol);
    return ((*ref) == NULL) ? false : true;
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void _abb_destruir(nodo_t * nodo, abb_destruir_dato_t f_destruir) {
	if (nodo == NULL) {
		return;
	}
	if (f_destruir != NULL) {
		f_destruir(nodo->dato);
	}
	nodo_t *nodo_izq = nodo->izq;
	nodo_t *nodo_der = nodo->der;
	destruir_nodo(nodo);
	_abb_destruir(nodo_izq, f_destruir);
	_abb_destruir(nodo_der, f_destruir);

}


void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->funcion_destruir);
    free(arbol);
}

//
//nodo_t* obtener_padre(abb_t *arbol, nodo_t* raiz, const char *clave){
//    if(raiz == NULL){
//        return NULL;
//    }
//    obtener_padre(arbol, raiz->izq, clave);
//    int comparacion = arbol->funcion_comparar(raiz->clave, clave);
//    if(comparacion == 0){
//        return raiz;
//    }else if(comparacion < 0 && raiz->der == NULL){
//        return raiz;
//    }else if(comparacion > 0 && raiz->izq == NULL){
//        return raiz;
//    }
//    obtener_padre(arbol, raiz->der, clave);
//}
//
//
//void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
//
//}
//
//struct abb_iter{
//    abb_t* arbol;
//    pila_t* pila;
//};
//
// Funciones extras
//
//void apilo_izq(abb_iter_t* iter, nodo_t* raiz){
//    if(raiz == NULL){
//        return;
//    }
//    pila_apilar(iter->pila, raiz);
//    apilo_izq(iter, raiz);
//}
//
// Primitivas del iterador
//
//abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
//    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
//    if(iter == NULL){
//        return NULL;
//    }
//    iter->arbol = (abb_t *)arbol;
//    iter->pila = pila_crear();
//    if(iter->pila == NULL){
//        free(iter);
//        return NULL;
//    }
//    apilo_izq(iter, iter->arbol->raiz);
//}
//
//bool abb_iter_in_avanzar(abb_iter_t *iter){
//    if(abb_iter_in_al_final(iter)){
//        return false;
//    }else{
//        char* clave = abb_iter_in_ver_actual(iter);
//        nodo_t* padre = obtener_padre(iter->arbol, iter->arbol->raiz, clave);
//        pila_desapilar(iter->pila);
//        if(padre != NULL){
//            pila_apilar(iter->pila, padre->der);
//            apilo_izq(iter, padre->der);
//        }
//        return true;
//    }
//}
//
//const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
//    return (char*)pila_ver_tope(iter->pila);
//}
//
//bool abb_iter_in_al_final(const abb_iter_t *iter){
//    return pila_esta_vacia(iter->pila);
//}
//
//void abb_iter_in_destruir(abb_iter_t* iter){
//    pila_destruir(iter->arbol->funcion_destruir);
//    free(iter);
//}
