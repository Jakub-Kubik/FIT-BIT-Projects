/**
 * @decription  Documentation to structures, global variables
 *              and functions in proj3.c
 * @file        proj3.h
 * @author      Jan Jakub Kubik (xkubik32)
 * @version     1.0
 * @date        14.12.2016
 * @copyright   Jakub Kubik 2016 */


/**
 * @defgroup structures_and_variables
 * A group for all structures and global variables
 * @{ 
 */


/**
 * @description Structure for an object in one cluster */
struct obj_t {
    int id;    /**< identification number */
    float x;   /**< X coordinate */
    float y;   /**< Y coordinate */
};


/**
 * @decription Structure for one cluster which contains a lot of objects */
struct cluster_t {
    int size;           /**< number of objects */
    int capacity;       /**< capacity of allocated memory for cluster's objects */
    struct obj_t *obj;  /**< pointer to an array of objects */
};

/** @} */ // end of structure group


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup basic_functions
 * @details A group of functions for initializing, printings clusters, for
 *          writing help and alocating and dealocating memory for clusters and objects
 * @{
 */
/**
 * @decription  inicialzation of cluster
 * @param c     poniter to one cluster
 * @param cap   new size of alocated memory for objects
 * @pre c       points to structure of one cluster
 * @pre cap     is bigger than 0 */
void init_cluster(struct cluster_t *c, int cap);


/**
 * @decription  removes all objects from one cluster
 * @param c     pointer to one cluster
 * @pre c       points to structure of one cluster
 * @post        cluster c is void */
void clear_cluster(struct cluster_t *c);
/** @} */  // end of group basic_functions


////////////////////////////////////////////////////////////////////////////////
/**
 *  @addtogroup structures_and_variables
 *  @{
 */
/** @decription Constant variable for realocating number of objects in one cluster */
extern const int CLUSTER_CHUNK;
/** @} */ // end of group structures_and_variables


////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup basic_functions
 */
/**
 * @brief             change capacity of cluster
 * @param  c          pointer to one cluster
 * @param  new_cap    new capacity for realocation
 * @return            pointer to realocated cluster
* @pre c              points to one cluster
 * @pre c             capacity is bigger than 0
 * @pre new_capacity  is bigger than 0 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);
/** @} */ // end of group basic_function


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup functions_for_cluster_analysis
 * Here are core function for my program
 *
 */
/**
 * @brief      append object to the end of cluster
 * @param c    pointer to structure of one cluster
 * @param obj  structure of one object
 * @pre c      points to one cluster
 * @pre obj    is an object */
void append_cluster(struct cluster_t *c, struct obj_t obj);


/**
 * @brief     add all objects of oe cluster to another cluster
 *            If it's needed then cluster 'c1' is resized. Objects in
 *            cluster 'c1' are sorted ascending acording to ID number
 *            Cluster 'c2' in unchanged
 * @param c1  pointer to structure of one cluster
 * @param c2  pointer to structure of one another cluster
 * @pre   c1  points to one cluster
 * @pre   c2  points to another cluster
 * @post      cluster c2 is untouched */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);
/** @} */ // end of functions_for_cluster_analysis


////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup basic_functions
 * @{
 */
/**
 * @brief        removes cluster from array of clusters
 * @param  carr  pointer array of clusters
 * @param  narr  number of items in carr cluster
 * @param  idx   index of cluster for removing
 * @return       number of new clusters
 * @pre carr     points to array of cluster
 * @pre narr     is biggeer than 0
 * @pre idx      is index from array of clusters
 * @post         array of cluster is shorter in one item */
int remove_cluster(struct cluster_t *carr, int narr, int idx);
/** @} */  //end of basic functions


////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup functions_for_cluster_analysis
 * @{
 */
/**
 * @brief     counts Euclidean's distance between two objects
 * @param o1  pointer to first object
 * @param o2  pointer to second object
 * @return    the distance betwen two objects
 * @pre o1    points to first object
 * @pre o2    points to second object */
float obj_distance(struct obj_t *o1, struct obj_t *o2);


/**
 * @brief      counts a distance of two clusters
 * @param  c1  pointer to first cluster
 * @param  c2  pointer to second cluster
 * @return     the max distance between two clusters
 * @pre    c1  points to one cluster
 * @pre    c1  size is bigger than 0
 * @pre    c2  points to anothe cluster
 * @pre    c2  size is bigger than 0 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);


/**
 * @brief       search for two nearest clusters
 * @param carr  pointer to array of clusters
 * @param narr  size of array
 * @param c1    first nearest cluster
 * @param c2    second nearest cluster
 * @pre carr    points to array of clusters
 * @pre narr    id bigger than 0 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);


/**
 * @brief    ascended sorting of objects in a cluster acording ID number
 * @param c  pointer to one cluster */
void sort_cluster(struct cluster_t *c);
/** @} */   //end of functions_for_cluster_analysis


////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup basic_functions
 * @{
 */
/**
 * @brief    print of one cluster to stdout
 * @param c  pointer to one cluster */
void print_cluster(struct cluster_t *c);


/**
 * @brief       for printing array of clusters
 * @param carr  a pointer to first item of cluster
 * @param narr  number of items for printing
 * @pre carr    points to array of clusters */
void print_clusters(struct cluster_t *carr, int narr);


/**
 * @brief            for loading of all objects from file. For every object it
 *                   creates new cluster and saved  to the array of clusters
 * @param  filename  name of file with objects
 * @param  arr       pointer to array of clusters
 * @return           number of loaded clusters
 * @pre arr          is a pointe to array of clusters */
int load_clusters(char *filename, struct cluster_t **arr);


/**
 * @brief print help to the program */
void chyba(void);


/**
 * @brief                     clear whole alocated memory before ending of program 
 *                            if some error has happend
 * @param number_of_clusters  number of clusters
 * @param clusters            pointer to array of clusters */
void clear_memory(int number_of_clusters, struct cluster_t *clusters);
/** @} */   // end of group basic_functions
