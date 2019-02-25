/**
 * @decription  program for simple cluster analysis
 *              method of the most remote neighbor
 * @author      Jan Jakub Kubik (xkubik32)
 * @date        2.12.2016 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <errno.h>
#include <string.h>

/*****************************************************************
 * Macros for debugging. Turn of by decdinition NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)
;
// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole objektov.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity; // je velkost alokovanej oblasti pamate
    struct obj_t *obj;
};

/* navratove kody funkcii */
#define CHYBA -1
#define OK 0


/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // alokacia objektov pre cluster
    c->obj = (struct obj_t *)malloc(cap*sizeof(struct obj_t));

    assert(c->obj != NULL);       // overenie alokacie
    c->size = cap;
    c->capacity = cap;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);     // odstrani vsetky objekty z jedneho zhluku

    // inicializuje ukazatel na obj ako prazdny -> nema ziadne misto alokovane
    c->obj = NULL;
    c->size = 0;
    c->capacity = 0;
}


const int CLUSTER_CHUNK = 10;   // hodnota odporucana pre realokaciu

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
  // NEROBIL SOM
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
  if (c->size == c->capacity) {   // ak nie je dost velky cluster tak ho zvacsim
    //osetrenie pretecenia pri realokacii c->size + CLUSTER_CHUNK would overflow
    if ((CLUSTER_CHUNK > 0) && (c->size > INT_MAX - CLUSTER_CHUNK)) {
      printf("pretecenie pri alokacii\n");
      exit(-1);
    }
    int newKap = c->size + CLUSTER_CHUNK;
    c = resize_cluster(c, newKap);
    c->capacity = newKap;
  }

  // zapis objektu do pola objektov v clustr
  // pole je idexovanne od nula preo zapisujem na c->size
  c->obj[c->size].id = obj.id;
  c->obj[c->size].x = obj.x;
  c->obj[c->size].y = obj.y;

  // pridal som objekt do pola objektov v clustri tak zvacsim premennu velkost
  // pre konkretny cluster
  c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // pridava objekty do prveho clustru dokym nebudu vsetky objekty z c2
    // prenesene do clustru 1
    // cluster 2 nemenim
    for (int i=0; i<c2->size; i++) {
      append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);         // utriedi jednotlive objekty v clustri 1
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(&carr[idx]);    // odstrani vsetky objekty z clustra
    narr--;                      // zmensi pocet klastrov
    return narr;
}

/**
 * Funkcia na uvolnenie celej pamamti
 * vyvola sa pri chybe a na konci porgramu
 */
void clear_memory(int number_of_clusters, struct cluster_t *clusters)
{
  while(number_of_clusters > 0) {
    number_of_clusters = remove_cluster(clusters, number_of_clusters, number_of_clusters-1);
  }
  free(clusters);
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float powx = powf(o1->x - o2->x, 2);
    float powy = powf(o1->y - o2->y, 2);
    float vzdalenost = sqrtf(powx + powy);
    return vzdalenost;      // nebudem to zaokruhlovat
}

/*
 Pocita vzdalenost dvou shluku.
 vzdalenost 2 clustrov pocitam ako maximalnu vzdialenost medzi ich objektmi
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // vzdalenost 2 clustrov pocitam ako maximalnu vzdialenost medzi ich objektmi
    float vzdal;
    float max=0;  // max vzdialenost mozem inicalizovat 0 pretoze max vzdialenost
                  // musi byt vzdy vacsia ako 0

    // cyklus pre prvy cluster aby sa jeho objekty porovnali so vsetkymi objektami
    // druheho clustra
    for (int i=0; i<c1->size; i++)
    {
      for (int j=0; j<c2->size; j++)
      {
        // pocitam vzdialenost medzi dvoma objektami pomocou funkcie
        vzdal = obj_distance(&c1->obj[i], &c2->obj[j]);

        if (vzdal > max) {      // zistujem najvacisu vzdialenost medzi 2 clustrami
          max = vzdal;
        }
      }
    }
    return max;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    int a;
    // 0 <= X,Y <= 1000 PODLA ZADANIA
    // MAX VZDIALENOST MEDZI 2 OBJEKTAMI JE SQRT((X1-X2)^2+(Y1-Y2)) CO JE
    // max sqrt(2*10^6) co je cca 1415
    // MIN DEFALUTNE INICIALIZUJEM NA 1415 LEBO VZDIALENOST MEDZI DVOMA OBJEKTMI
    // NEMOZE BYT NIKDY VIAC AKO 1415
    int min = 1415;

    // PRECHADZAM ZHLUK PO ZHLUKU A POROVNAVAM JEHO VZDIALENOST S OSTATNYMI ZHLUKMI
    // POMOCOU FUNKCIE CLUSTER DISTANCE
    for (int i=0; i<narr; i++)
    {
      for (int j=0; j<narr; j++)
      {
        if (i == j) continue;   // dva rovnake clustre name zmysel porovnavat
        if (carr[j].size == 0) continue;  // ak je cluster neinicialozovany tak
        if (carr[i].size == 0) continue;  // to nema zmysel ho porovnanvat
                                          // s ostatnymi zhlukmi
        // zistujem vzdialenost pomocou najvzdialenejsich objektov
        a = cluster_distance(&carr[i], &carr[j]);
        // index najblizsich clustrov ulozim do premennej *c1 a *c2
        if (a < min) {
          min = a;
          *c1 = i;
          *c2 = j;
        }
      }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCIU SOM NEROBIL
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCIU SOM NEROBIL
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}
/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCIU SOM NEROBIL
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}
/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}
/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    FILE *fr;   // otvorim pozadovany sobor a overim ci sa to podarilo
    if ((fr = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Nepodarilo sa otvorit subor %s\n", filename);
      return CHYBA;
    }
    // overim format count=
    int number;                          // premenna na pocet clustrov
    char count[7];
    fscanf(fr, "%6s", count);
    if (strcmp(count, "count=")) {
      fprintf(stderr, "count= neodpoveda pozadovanemu formatu\n");
      fclose(fr);
      return CHYBA;
    }
    fscanf(fr, "%d", &number);    // nacitam pocet clustrov
    if (number <= 0) {
      fprintf(stderr, "0 alebo Zaporny pocet clustrov: nedefinovane\n");
      fclose(fr);
      return CHYBA;
    }

    // ALOKUJEM PAMAT PRE POLE CLUSTROV
    *arr = (struct cluster_t *)malloc(number*sizeof(struct cluster_t ));
    if (*arr == NULL) {                   // overim ci sa alokacia podarila
      printf("Nepodarilo sa alokovat pamat pre clustre\n");
      return CHYBA;
    }
    int pocet_medzier = 0;
    int pocet_riadkov = 0;
    char test;
    while ((test = getc(fr)) != EOF) {
    //  printf("Pocet medzier je %d\n", pocet_medzier);
      if (test == ' ') {
        pocet_medzier++;
      }
      else if(test == '\n') {
        pocet_riadkov++;
        pocet_medzier=0;
      }
      if (pocet_medzier > 2) {
        fprintf(stderr, "Nespravny format riadku\n");
        clear_memory(number, *arr);
        fclose(fr);
        return CHYBA;
      }
    }
    //pocet_riadkov--;
    //printf("Pocet riadkov=%d poc r=%d\n", number, pocet_riadkov);
    if (pocet_riadkov < number ){
      fprintf(stderr, "Nespravny pocet riadkov\n");
      clear_memory(number, *arr);
      fclose(fr);
      return CHYBA;
    }
    // posun o count=cislo
    fseek(fr, 0, SEEK_SET);
    fscanf(fr, "count=%d", &number);
    //printf("Number=%d\n", number);
    for (int i=0; i<number; i++) {        // NACITANIE OBJEKTOV ZHLUKU
      init_cluster(*arr+i, 1);    // alokacia a defaultna inicializacia pre 1 objekt v zhluku
      if (fscanf(fr,"%d", &(*arr+i)->obj->id) == 0)  {
        fprintf(stderr, "V subore chyba ID objektu\n");
        clear_memory(number, *arr);
        fclose(fr);
        return CHYBA;
      }

      if (fscanf(fr,"%f", &(*arr+i)->obj->x) == 0) {
        fprintf(stderr, "V subore chyba X-ova suradnica\n");
        clear_memory(number, *arr);
        fclose(fr);
        return CHYBA;
      }
      if (fscanf(fr,"%f", &(*arr+i)->obj->y) == 0) {
        fprintf(stderr, "V subore chyba y-ova suradnica\n");
        clear_memory(number, *arr);
        fclose(fr);
        return CHYBA;
      }
      int xx = (*arr+i)->obj->x;
      int yy = (*arr+i)->obj->y;
      if (xx != (*arr+i)->obj->x || yy != (*arr+i)->obj->y) {
        fprintf(stderr, "X alebo Y nie su int\n");
        clear_memory(number, *arr);
        fclose(fr);
        return CHYBA;
      }
      if (errno) {                      // overenie pretecenia pri nacitavani ID
        fprintf(stderr, "Pretecenie ID\n");
        clear_memory(number, *arr);
        fclose(fr);
        return CHYBA;
      }
      // x && y su vo rozsahu <0, 1000> inak vraciam chybu podla zadania
      if ((*arr+i)->obj->x < 0 || (*arr+i)->obj->x > 1000) {
        fprintf(stderr, "Suradnica X>1000\n");
        clear_memory(number, *arr);
        fclose(fr);
        return CHYBA;
      }
      if ((*arr+i)->obj->y < 0 || (*arr+i)->obj->y > 1000) {
        fprintf(stderr, "Suradnica Y>1000\n");

        return CHYBA;
      }
    }
    fclose(fr);                 // uzatvorenie pouzivaneho suboru

    return number;
}

void chyba(void)
{
  fprintf(stderr, "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
  fprintf(stderr, "\n PROGRAM SA SPUSTA V NASLEDUJUCEJ PODOBE:\n");
  fprintf(stderr, "./proj3 SOUBOR [N]\n");
  fprintf(stderr, "\tSOUBOR je meno suboru so vstupnymi datami\n");
  fprintf(stderr, "\tN je volitelny argument definujuci cielovy\n");
  fprintf(stderr, "\tpocet zhlukov. N>0. Defaultne je N 1\n");
  fprintf(stderr, "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
}
/**
 * Main funkcia
 * Najskor overim pocet argumetov predanych programu
 * Potom overim ci neni pocet pozadovanych clustrov viac ako je pocet clustrov
 * Ak vsetko sedi tak v cykle zistujem najblizsie clustre a zlucujem ich dovtedy
 * dokyal nie je pocecet clustrov rovnaky ako pocet pozadovanych clustrov
 * defaultne je pocet pozadovanych clustrov 1
 */

int main(int argc, char *argv[])
{
  struct cluster_t *clusters;       // POINTER NA POLE CLUSTROV

  if (argc == 1 || argc > 3) {     // NESPRAVNY POCET ARGUMENTOV ZADANYCH PROGRAMU
      fprintf(stderr, "Nespravny pocet argumentov zadanych programu\n");
      chyba();
      return CHYBA;
  }
  else {                            // SPRAVNY POCET ARGUMENTOV ZADANYCH PROGRAMU
      long int pozad_poc_clustrov = 1;  // premenna na ziskanie pozadovaneho poctu clastrou defautlne je 1
      // nacitam vsetky clustre zo vstupneho suboru ak nastala chyba vracia -1
      int pocet_clustrov = load_clusters(argv[1], &clusters);
      if (pocet_clustrov == CHYBA) {    // overim ci pri nacitavani clustru nenastala chyba
        return CHYBA;
      }
      for (int i=0; i<pocet_clustrov; i++) {
        for (int j=0; j<pocet_clustrov; j++) {
          if (i == j) continue;
          if (clusters[i].obj->id == clusters[j].obj->id) {
            fprintf(stderr, "Dupicitne ID\n");
            clear_memory(pocet_clustrov, clusters);
            return CHYBA;
          }
        }
      }
      int dealokuj = pocet_clustrov; // premenna na dealokaciu na konci programu

      if (argc == 3 ) {           // OVERIM CI BOL ZADANY POCET CLUSTROV
          char *ptr;              // zistim pozadovany pocet clustrov
          pozad_poc_clustrov = strtol(argv[2], &ptr, 10);
          // overim spravnost formatu pozadovaneho poctu clustrov
          if (*ptr != '\0' || pozad_poc_clustrov > pocet_clustrov || pozad_poc_clustrov <= 0) {
            fprintf(stderr, "Nespranve zadany pocet pozadovanych clustrov\n");
            chyba();
            clear_memory(dealokuj, clusters);
            return CHYBA;
          }
      }
    // najdolezitejsia cast kodu ********************************************

    // KLASTRE BUDEM ZLUCOVAT DOKEDY SA ICH POCET NEBUDE ROVNAT POZADOVANEMU POCTU
    while (pocet_clustrov != pozad_poc_clustrov)
    {
      int x, y; // pozicia 2 najblizsich clustrov
      find_neighbours(clusters, pocet_clustrov, &x, &y);  // najde 2 najblizsie clustre
      merge_clusters(clusters+x, clusters+y);     // zlucim 2 najblizsie clustre (y do x)

      // vzdy budem len o jeden cluster menej vypisovat preto musim
      // preniest vsetky objekty z konca na na miesto z ktoreho som zlucoval
      if (clusters[y].capacity != clusters[pocet_clustrov-1].capacity) {
        remove_cluster(clusters, pocet_clustrov, y);  //ODSTRANI LEN VSETKY OBJEKTY Z DANEHO ZHLUKU
        init_cluster(&clusters[y], clusters[pocet_clustrov-1].capacity);
        clusters[y].capacity = clusters[pocet_clustrov-1].capacity;
      }
      // jednotlive objekty z konca clustra prekopirujem na y-tu poziciu v poli clustrov
      //printf("y je %d Cluster y %d\n",y-1, clusters[y].size );

      for (int i=0; i<clusters[pocet_clustrov-1].size; i++)
      {
        clusters[y].obj[i].id = clusters[pocet_clustrov-1].obj[i].id;
        clusters[y].obj[i].x = clusters[pocet_clustrov-1].obj[i].x;
        clusters[y].obj[i].y = clusters[pocet_clustrov-1].obj[i].y;
        clusters[y].size = i+1;
      }
      pocet_clustrov--;
    }
    print_clusters(clusters, pocet_clustrov);     // vypis  clustrov
    clear_memory(dealokuj, clusters);      // NA ZAVER UVOLNIM PAMAT
    return OK;
  }
}
