#include<deque>
#include "Activity.h"

/*		ESQUEMAS	 */
#define _PARALEL    3000
#define _SERIE		 3001
#define _MULTIPASADA 3002

/*	REGLAS PRIORIDAD	*/
#define MIN_LFT_FIFO 2000		//instante de finalizacion mas tardio
#define MIN_EFT_FIFO 2001		//instante de finalizacion mas temprano
#define MIN_LST_FIFO 2002		//instante de comienzo mas tardio
#define MIN_EST_FIFO 2003		//instante de comienzo mas temprano
#define MIN_HT_FIFO 2004		// holgura total
#define MIN_HL_FIFO 2005		// hlgura libre
#define MAX_DEM_FIFO 2006		// demanda de recursos
#define MIN_DUR_FIFO 2007		// duracion de la actividad
#define MAX_NSUC_FIFO 2008		// numero de succesoras



#ifndef RULES_H_
#define RULES_H_
//////////////////////////////////////////////////////////
//////		REGLAS DE PRIORIDAD	PRIORity			//////
//////////////////////////////////////////////////////////

bool MinLFT_FIFO(Activity * act1,Activity * act2);

bool MinEFT_FIFO(Activity * act1,Activity * act2);

bool MinHT_FIFO(Activity * act1,Activity * act2);
bool MinHL_FIFO(Activity * act1,Activity * act2);

bool MinEST_FIFO(Activity * act1,Activity * act2);
bool MinLST_FIFO(Activity * act1,Activity * act2);
bool MinDUR_FIFO(Activity * act1,Activity * act2);

bool MaxNSUC_FIFO(Activity * act1,Activity * act2);

bool MaxDEM_FIFO(Activity * act1,Activity * act2);


////////////////////////////////////////////////////////
////////   	  ELEGIR REGLA SEGUN OPCION		    ////////
////////////////////////////////////////////////////////
bool choiceRule(Activity * act1,Activity * act2, int rule);	// aplica la regla indicada por rule

deque<Activity*> binsearch(deque<Activity*> activities, Activity * act, int left, int right, int rule); //algoritmo de divide y venceras
deque<Activity*> insert(deque<Activity*> activities, Activity * act, int rule_prio);	//insertar actividad de forma ordenada

#endif;
