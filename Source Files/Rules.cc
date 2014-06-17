#include "Rules.h"
#include "Activity.h"



///////////////////////////////////////////////////////
/////////		  REGLAS DE PRIORIDAD			///////
///////////////////////////////////////////////////////

bool MinLFT_FIFO(Activity * act1,Activity * act2)
{
	/*Minimo instante de finalizacion tardio*/
	return(act1->getTMax()+act1->getTNormal()<act2->getTMax()+act2->getTNormal());
}

bool MinEFT_FIFO(Activity * act1,Activity * act2)
{
	/*Minimo instante de finalizacion mas temprano*/
	return(act1->getTMin()+act1->getTNormal()<act2->getTMin()+act2->getTNormal());	
}

bool MinHT_FIFO(Activity * act1,Activity * act2)
{
	/*Minima holgura total*/
	return(act1->getHTotal()<act2->getHTotal());		
}

bool MinHL_FIFO(Activity * act1,Activity * act2)
{
	/*Minima holgura libre*/
	return(act1->getHFree()<act2->getHFree());		
}

bool MinEST_FIFO(Activity * act1,Activity * act2)
{
	/*Minimo instante de comienzo mas temprano*/
	return(act1->getTMin()<act2->getTMin());		
}

bool MinLST_FIFO(Activity * act1,Activity * act2)
{
	/*Minimo instante de comienzo mas tardio*/
	return(act1->getTMax()<act2->getTMax());		
}

bool MinDUR_FIFO(Activity * act1,Activity * act2)
{
	/*Minima duracion de la actividad*/	
	return(act1->getTNormal()<act2->getTNormal());		
}

bool MaxNSUC_FIFO(Activity * act1,Activity * act2)
{
	/*Maximo numero de succesoras*/
	return(act1->getList_Activities_suc()->size()>act2->getList_Activities_suc()->size());		
}



bool MaxDEM_FIFO(Activity * act1,Activity * act2)
{
		int prior1=0;
		int prior2=0;

		/*Maxima demanda de recursos*/
		for(int i=0; i<act1->getResources().size();i++)
			prior1+=act1->getResources().at(i)->units_asig;

		for(int i=0; i<act2->getResources().size();i++)
			prior2+=act2->getResources().at(i)->units_asig;

		prior1*=act1->getTNormal();
		prior2*=act2->getTNormal();

		return(prior1>prior2);		
}

////////////////////////////////////////////////////////
///////		  ELEGIR REGLA SEGUN OPCION			////////
////////////////////////////////////////////////////////
bool choiceRule(Activity * act1,Activity * act2, int rule)
{

	switch(rule){
	
		case MIN_LFT_FIFO:
			return MinLFT_FIFO(act1,act2);
		break;

		case MIN_EFT_FIFO:
			return MinEFT_FIFO(act1,act2);
		break;

		case MIN_LST_FIFO:
			return MinLST_FIFO(act1,act2);
		break;

		case MIN_EST_FIFO:
			return MinEST_FIFO(act1,act2);
		break;

		case MIN_HT_FIFO:
			return MinHT_FIFO(act1,act2);
		break;

		case MIN_HL_FIFO:
			return MinHL_FIFO(act1,act2);
		break;

		case MAX_DEM_FIFO:
			return MaxDEM_FIFO(act1,act2);
		break;

		case MIN_DUR_FIFO:
			return MinDUR_FIFO(act1,act2);
		break;

		case MAX_NSUC_FIFO:
			return MaxNSUC_FIFO(act1,act2);
		break;
		
		default: return false; break;
	
	
	}



}

////////////////////////////////////////////////////////////////////
////////////		   INSERCION ORDENARA				///////////
///////////////////////////////////////////////////////////////////


deque<Activity*> binsearch(deque<Activity*> activities, Activity * act, int left, int right, int rule) 
{
	 
	/*Esta funcion hace un ordenamiento de actividades  segun una regla de prioridad,
	siguendo una estrategia de divide y venceras, con coste  temporal de O(log n)*/


	if (left > right) return activities;

	/*Solo hay un elemento*/
	if(left==right)
	{
		/*Ordenar segun la regla de prioridad*/
		if(choiceRule(act,activities.at(left), rule))
			activities.insert(activities.begin()+left, act);
		else
			activities.insert(activities.begin()+left+1, act);
		return activities;
	}
		
	int middle = (left+right)/2;

	/*Como la lista esta ordenada, si la partimos en 2, 
	en que parte estara la posicion donde insertarla*/
	if (choiceRule(act,activities.at(middle), rule))
		 return binsearch(activities,act,left,middle,rule);
	else
		return binsearch(activities,act,middle+1,right,rule);
	 
		
}

deque<Activity*> insert(deque<Activity*> activities, Activity * act, int rule_prio){

	/*Inserta la actividad act de forma ordenada*/
	if(activities.size()==0)
	{
		activities.push_back(act);
		return activities;
	}else
		/*busca la posicion dond einsertar la actividad*/
		return binsearch(activities, act, 0, activities.size()-1, rule_prio);


}


