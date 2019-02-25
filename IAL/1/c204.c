
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, říjen 2017                                        */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

/*
 * VUT FIT 2BIA
 * *****************************
 * 1. Domaca uloha z IAL
 * Implementoval: Jan Jakub Kubik (xkubik32@stud.fit.butbr.cz)
 * Vypracovane: 13.10.2017
 * ***************************
*/

#include "c204.h"

int solved;


/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
  char c;    // default initialization

    do {
      stackTop(s, &c);
      stackPop(s);
      if ( c != '(' ) {
        postExpr[ *postLen] = c;
        (*postLen)++;
      } // if
    } while ( c != '(' );
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat,
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
  char stack_c;
  int repeat = 1;

  while ( repeat ) {
    if ( !stackEmpty(s) )
      stackTop(s, &stack_c);
    // tu to neni dobre overene -> FIXED
    if ( stackEmpty(s) ||
        stack_c == '(' ||
        ( (c == '*' || c == '/') && (stack_c == '+' || stack_c == '-') ) ) {
      stackPush(s, c);

      repeat = 0;
    }
    else {
      postExpr[*postLen] = stack_c;
      (*postLen)++;
      stackPop(s);
    }
  }
}

/*
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202.
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {

char* postExpr = (char*)malloc(MAX_LEN*sizeof(char));
if ( postExpr == NULL ) return NULL;

tStack stack;
stackInit(&stack);

unsigned i_inf = 0; unsigned i_post = 0;
char c;
int myStrlen = 0;

// find out strlen of input string
c = infExpr[myStrlen];
while ( c != '=' ) {    // nie je vobec elegantne spravene lebo nechce reagovat
  myStrlen++;           // na koncovy znak '/0' a predpoklad je ze vstup je spravny vzdy
  c = infExpr[myStrlen];
}
myStrlen++;


// Algoritmus podla 3. prednasky z IAL popisuje presne kroky podla prednasky
for ( i_inf = 0; i_inf < myStrlen; i_inf++ ) {
  c = infExpr[i_inf];
  if ( (c >= '0' && c <= '9') ||   // 2. ############### 0..9, a..z, A..Z
       (c >= 'a' && c <= 'z') ||
       (c >= 'A' && c <= 'Z') ) {
    postExpr[i_post] = c;
    i_post++;
  } // end of 2
  else if ( c == '(' ) {  // 3. ###############
    stackPush(&stack, c);
  } // end of 3
  else if ( c == '+' || c == '-' || c == '*' || c == '/' ) { // 4. ###########
      doOperation(&stack, c, postExpr, &i_post);
  } // end of 4
  else if ( c == ')' ) {  // 5. ##################
    untilLeftPar ( &stack, postExpr, &i_post );
  } // end of 5
  else if ( c == '=' ) { // 6. ###################
    while ( !stackEmpty(&stack) ) {
      stackTop(&stack, &c);
      stackPop(&stack);
      postExpr[i_post] = c;
      i_post++;
    }
    postExpr[i_post] = '=';   // insert = to output string
    i_post++;
  } // end of 6

}
postExpr[i_post] = '\0';  // ending output string

return postExpr;
}

/* Konec c204.c */
