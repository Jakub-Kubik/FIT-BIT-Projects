
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

/*
 * VUT FIT 2BIA
 * *****************************
 * 1. Domaca uloha z IAL
 * Implementoval: Jan Jakub Kubik (xkubik32@stud.fit.butbr.cz)
 * Vypracovane: 13.10.2017
 * ***************************
*/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/
	tDLElemPtr itmFree;

	while ( L->First != NULL ) {
		itmFree = L->First;
		L->First = L->First->rptr;
		free(itmFree);
	}
	DLInitList(L); // default initialization
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr newItm;
	newItm = malloc(sizeof(struct tDLElem));
	if ( newItm == NULL ) DLError();

  newItm->data = val;
  newItm->lptr = NULL;
  newItm->rptr = L->First;

  if (L->First != NULL) {
    L->First->lptr = newItm;
  }
  else {
    L->Last = newItm;
  }

  L->First = newItm;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr newItm;
  newItm = malloc(sizeof(struct tDLElem));
  if ( newItm == NULL ) DLError();

  newItm->data = val;
  newItm->rptr = NULL;

  if ( L->Last != NULL ) {
    newItm->lptr = L->Last;
    L->Last->rptr = newItm;
  }
  else {
    newItm->lptr = NULL;
    L->First = newItm;
  }

  L->Last = newItm;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
  L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

  L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if ( L->First != NULL ) {
    *val = L->First->data;
  }
  else {
    DLError();
  }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

  if ( L->Last != NULL ) {
    *val = L->Last->data;
  }
  else {
    DLError();
  }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
  tDLElemPtr del;

  if ( L->First == L->Act ) L->Act = NULL;

  if ( L->First != NULL ) {
    del = L->First;
    if ( L->First->rptr != NULL ) {
      L->First = L->First->rptr;
      L->First->lptr = NULL;
    }
    else {
      if ( L->First == L->Last ) L->Last = NULL;
      L->First = NULL;
    }
    free(del);
  }
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
  tDLElemPtr del;

  if ( L->Last == L->Act ) L->Act = NULL;

  if ( L->Last != NULL ) {
    del = L->Last;
    if ( L->Last->lptr != NULL ) {
      L->Last = L->Last->lptr;
      L->Last->rptr = NULL;
    }
    else {
      if ( L->Last == L->First ) L->First = NULL;
      L->Last = NULL;
    }

    free(del);
  }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
  if ( DLActive(L) && L->Act->rptr != NULL ) {

    tDLElemPtr del;
    del = L->Act->rptr;

    if ( del->rptr != NULL ) {
      L->Act->rptr = del->rptr;
      del->rptr->lptr = L->Act;
    }
    else {
      L->Act->rptr = NULL;
      L->Last = L->Act;
    }
    free(del);
  }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
  if ( DLActive(L) && L->Act != L->First ) {
    tDLElemPtr del;
    del = L->Act->lptr;

    if ( del->lptr == NULL ) {
      L->Act->lptr = NULL;
      L->First = L->Act;
    }
    else {
      L->Act->lptr = del->lptr;
      del->lptr->rptr = L->Act;
    }
    free(del);
  }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if ( DLActive(L) ) {

    tDLElemPtr new;

    new = malloc(sizeof(struct tDLElem));
    if ( new == NULL ) DLError();

    new->data = val;

    if ( L->Act->rptr == NULL ) {
        new->rptr = NULL;
        L->Act->rptr = new;
        new->lptr = L->Act;

        L->Last = new;
    }
    else {
      L->Act->rptr->lptr = new;
      new->rptr = L->Act->rptr;
      L->Act->rptr = new;
      new->lptr = L->Act;
    }
  }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if ( DLActive(L) ) {
    tDLElemPtr new;
    new = malloc(sizeof(struct tDLElem));
    if ( new == NULL ) DLError();

    new->data = val;

    if ( L->Act == L->First ) {
      new->lptr = NULL;
      new->rptr = L->Act;
      L->Act->lptr = new;

      L->First = new;
    }
    else {
      L->Act->lptr->rptr = new;
      new->lptr = L->Act->lptr;

      new->rptr = L->Act;
      L->Act->lptr = new;
    }
  }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
  if ( DLActive(L) ) {
      *val = L->Act->data;
  }
  else {
    DLError();
  }
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
  if ( DLActive(L) ) {
    L->Act->data = val;
  }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
  if ( DLActive(L) ) {
    L->Act = L->Act->rptr;
  }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
  if ( DLActive(L) ) {
    L->Act = L->Act->lptr;
  }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
  return ( L->Act != NULL );
}

/* Konec c206.c*/
