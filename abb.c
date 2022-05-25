#include "abb.h"
#include "pila.h"

typedef struct nodo{
    struct nodo* izq;
    struct nodo* der;
    char* clave;
    void* dato;
}nodo_t;

struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_destruir_dato_t funcion_destruccion;
    abb_comparar_clave_t comparar_calve;
};

typedef struct padre_e_hijo{
    nodo_t* padre;
    nodo_t* hijo;
}padre_e_hijo_t;

nodo_t* crear_nodo(const char *clave, void *dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL){
        return NULL;
    }
    nodo->clave = clave;
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    
    return nodo;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(abb == NULL){
        return NULL;
    }
    abb->cantidad = 0;
    abb->funcion_destruccion = destruir_dato;
    abb->comparar_calve = cmp;
    abb->raiz = NULL;
    
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t* nodo = crear_nodo(clave, dato);
    if(nodo == NULL){
        return false;
    }
    if(abb_cantidad(arbol) == 0){
        arbol->raiz = nodo;
    }else{
        // falta guardar una copia de la clave
        nodo_t* padre = obtener_padre(arbol, arbol->raiz, clave);
        int comparacion = arbol->comparar_calve(arbol->raiz->clave, clave);
        if(comparacion == 0){
            padre->dato = dato;
        }else if(comparacion < 0){
            padre->izq = nodo;
        }else if(comparacion > 0){
            padre->der = nodo;
        }
    }
    arbol->cantidad++;
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){

}

void *abb_obtener(const abb_t *arbol, const char *clave){ 
    nodo_t* nodo = obtener_hijo(arbol->raiz, clave);
    if(nodo == NULL){
        return NULL;
    }
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_t* nodo = obtener_hijo(arbol->raiz, clave);
    if(nodo == NULL){
        return false;
    }
    return true;;
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol, arbol->raiz);
    free(arbol);
}

void _abb_destruir(abb_t *arbol, nodo_t *raiz){
    if(raiz == NULL){
        return; 
    }
    _abb_destruir(arbol, raiz->izq);
    _abb_destruir(arbol, raiz->der);
    free(raiz->clave);
    if(arbol->funcion_destruccion != NULL){
        arbol->funcion_destruccion(raiz->dato);
    }
    free(raiz);
}



nodo_t* obtener_hijo(nodo_t* raiz, const char *clave){
    if(raiz == NULL){
        return NULL;
    }
    obtener_nodo(raiz->izq, clave);
    if(raiz->clave == clave){
        return raiz;
    }
    obtener_nodo(raiz->der, clave);
}

nodo_t* obtener_padre(abb_t *arbol, nodo_t* raiz, const char *clave){
    if(raiz == NULL){
        return NULL;
    }
    obtener_padre(arbol, raiz->izq, clave);
    int comparacion = arbol->comparar_calve(raiz->clave, clave);
    if(comparacion == 0){
        return raiz;
    }else if(comparacion < 0 && raiz->der == NULL){
        return raiz;
    }else if(comparacion > 0 && raiz->izq == NULL){
        return raiz;
    }
    obtener_padre(arbol, raiz->der, clave);
}










padre_e_hijo_t* crear_padre_hijo(){
    padre_e_hijo_t* padre_hijo = malloc(sizeof(padre_e_hijo_t));
    if(padre_hijo == NULL){
        return NULL;
    }
    padre_hijo->padre = NULL;
    padre_hijo->hijo = NULL;
    return padre_hijo;
}

/*
Esta funcion me tiene que devolver el struct con el padre y el hijo
me tiene que servir para las funciones: borrar, guardar, pertenece y obtener
el borrar y guardar va a usar el padre
pertenece y obtenerva a usar el hijo

TENGO QUE TERMINAR LA PARTE DE QUE DEVUELVA AL HIJO, MASOMENOS EL PADRE ESTA HECHO
*/
padre_e_hijo_t* nodo_padre_hijo(padre_e_hijo_t* padre_hijo, nodo_t* raiz, const char *clave){
    if(raiz == NULL){
        return NULL;
    }
    nodo_padre_hijo(padre_hijo, raiz->izq, clave);
    if(clave < raiz->clave && raiz->izq != clave){
        padre_hijo->padre = raiz;
        padre_hijo->hijo = raiz->izq;
        return padre_hijo;

    }else if(clave > raiz->clave && raiz->der != clave){
        padre_hijo->padre = raiz;
        padre_hijo->hijo = raiz->der;
        return padre_hijo;

    }else if(raiz->clave == clave && raiz->der == NULL && raiz->izq == NULL){
        padre_hijo->padre = raiz;
        padre_hijo->hijo = NULL;
        return padre_hijo;
    }

    nodo_padre_hijo(padre_hijo, raiz->der, clave);
}






void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){

}

struct abb_iter{
    abb_t* arbol;
    pila_t* pila;
};

// Funciones extras

void apilo_izq(abb_iter_t* iter, nodo_t* raiz){
    if(raiz == NULL){
        return;
    }
    pila_apilar(iter->pila, raiz);
    apilo_izq(iter, raiz);
}

// Primitivas del iterador

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = mallo(sizeof(abb_iter_t));
    if(iter == NULL){
        return NULL;
    }
    iter->arbol = arbol;
    iter->pila = pila_crear();
    if(iter->pila == NULL){
        free(iter);
        return NULL;
    }
    apilo_izq(iter, iter->arbol->raiz);
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)){
        return false;
    }else{
        char* clave = abb_iter_in_ver_actual(iter);
        nodo_t* padre = obtener_padre(iter->arbol, iter->arbol->raiz, clave);
        pila_desapilar(iter->pila);
        if(padre != NULL){
            pila_apilar(iter->pila, padre->der);
            apilo_izq(iter, padre->der);
        }
        return true;
    }
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    return (char*)pila_ver_tope(iter->pila);
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->arbol->funcion_destruccion);
    free(iter);
}