# include "breadth_search.h"

/**
 *	@require : 	'nodes' : un tableau de structure 't_node' (ou en héritant)
 *		 	'arcs' : une bitmap representant les arcs
 *			's' : indice de sommet source
 *			't' : indice de sommet destination
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : modifies le tableau 'nodes' en lui appliquant un algorithme
 *		   de parcours en largeur sur ses sommets
 */
int breadth_search(t_array * nodes, INDEX s, INDEX t) {

	/* initilisation: on definie toutes les distantes à +oo (MAX_NODES suffit),
	   sauf pour l'origine à 0 */
	INDEX n = nodes->size;
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_node * node = (t_node *) array_get(nodes, i);
		node->pathlen = MAX_NODES;
		node->prev = -1;
	}
	((t_node *)array_get(nodes, s))->pathlen = 0; /* source à 0 */

	/** on crée une file */
	t_list * visitQueue = list_new();
	if (visitQueue == NULL) {
		return (0);
	}
	t_bitmap * visited = bitmap_new(n);
	if (visited == NULL) {
		list_delete(visitQueue);
		return (0);
	}

	/** on ajoute la source a la file */
	list_add(visitQueue, &s, sizeof(INDEX));

	/** tant que la file n'est pas vide */
	while (visitQueue->size > 0) {
		/** on pop la tête de file */
		INDEX uID = *((INDEX *) list_head(visitQueue));
		list_remove_head(visitQueue);
			
		/** on recupere le sommet correspondant */
		t_node * u = (t_node *) array_get(nodes, uID);

		/** s'il n'a pas de successeurs, on continue */
		if (u->successors == NULL) {
			continue ;
		}

		/** sinon, pour chaque successeur */
		INDEX i;
		for (i = 0 ; i < u->successors->size ; i++) {
			/** 'v' est successeurs de 'u' */
			INDEX vID = *((INDEX *) array_get(u->successors, i));
			
			/* si on a déjà visité 'v', on continue */
			if (bitmap_get(visited, vID)) {
				continue ;
			}

			/* sinon, on visite 'v' */
			t_node * v = (t_node *) array_get(nodes, vID);	
			bitmap_set(visited, vID);

			v->prev = uID;
			v->pathlen = u->pathlen + 1;
			/* si on a atteint 't', on a trouvé le chemin */
			if (vID == t) {
				bitmap_delete(visited);
				list_delete(visitQueue);
				return (1);
			}
			list_add(visitQueue, &vID, sizeof(INDEX));
		}
	}
	/** sinon, 's' et 't' sont dans des parties connexes distinctes */
	bitmap_delete(visited);
	list_delete(visitQueue);
	return (0);
}