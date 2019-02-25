
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

/*
 * VUT FIT 2BIA
 * *****************************
 * 2. Domaca uloha z IAL
 * Implementoval: Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz)
 * Vypracovane: 21.10.2017
 * ***************************
*/

/*
 * OVEROVANIE NA NULL POINTRE DO FUNKCII MOZNO BUDE TREBA ESTE ROBIT
*/
#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	for ( int i=0; i < HTSIZE; i++ ) {
		(*ptrht)[i] = NULL;
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	tHTItem *search;
	search = (*ptrht)[ hashCode(key) ];

	if ( search != NULL ) {

		do {
			if ( strcmp(search->key, key) == 0 ) {
				return search;							// item is found
			}

			search = search->ptrnext;
			
		} while ( search != NULL );

		return NULL; 									// item isnt found
	} 

	return NULL;										// linear list in hashed table on line hashCode(key) is empty	

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	tHTItem *change = htSearch(ptrht, key);
	
	if ( change != NULL ) {							// itm found -> change data
		change->data = data;
	}												
	else {											// itm not found -> insert new item

		tHTItem *new_itm = malloc(sizeof(tHTItem)); // create new item
		if ( new_itm == NULL ) {
			exit(99);
		}

		new_itm->key = malloc(strlen(key)+1);		// +1 for '/0'
		if ( new_itm->key == NULL ) {
			exit(99);
		}

		strcpy(new_itm->key, key);					// strcpy include '/0' to new string
		new_itm->data = data;


		tHTItem *help;
		help = (*ptrht)[ hashCode(key) ];

		if ( help == NULL ) { 						 // first item on line hashCode(key) is empty
			
			new_itm->ptrnext = NULL;
		}
		else {  									// first item on line hashCode(key) isnt empty			
			new_itm->ptrnext = help;
		}
		(*ptrht)[ hashCode(key) ] = new_itm;

	}
 // solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	
	tHTItem *read = htSearch(ptrht, key);		// search fot itm 

	if ( read != NULL ) {  						// item is found

		return &read->data;
	}

	return NULL;								// item isnt found

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	tHTItem *itm_rm, *itm_prev;
	itm_rm = (*ptrht)[ hashCode(key) ]; 					// find index in hashed table

	int find = 0;

	if ( itm_rm != NULL ) {					

		if ( strcmp(itm_rm->key, key) == 0 ) { 				// check first item in linear list

			(*ptrht)[ hashCode(key) ] = itm_rm->ptrnext;

			free(itm_rm->key);
			free(itm_rm);

		} 
		else {												// walk from seconf item to the eind of linear list 

			do { 											// search for itm
				if ( strcmp(itm_rm->key, key) == 0 ) {
					find = 1;
					break;
				}
				itm_prev = itm_rm;
				itm_rm = itm_rm->ptrnext;

			}	while ( itm_rm != NULL );


			if ( find == 1 ) {								// item is found				

				if ( itm_rm->ptrnext == NULL ) {

					itm_prev->ptrnext = NULL;
				} 
				else {
					itm_prev->ptrnext = itm_rm->ptrnext;
				}

				free(itm_rm->key);
				free(itm_rm);
			} 

		}

	}

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	tHTItem *itm_act;
	tHTItem *itm_next;

	for( int i = 0; i < HTSIZE; i++ ) {

		itm_act = (*ptrht)[i];

		while( itm_act != NULL ) {

			itm_next = itm_act->ptrnext;

			free(itm_act->key);
			free(itm_act);

			itm_act = itm_next;

		} 
		(*ptrht)[i] = NULL; 				// set up pointers in hashed table to default value

	} 

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}
