#include "Project.h"
#include <algorithm>
#include <math.h>
#include "Exceptions.h"
#include "Rules.h"

/*********************************************************************/
/*----------------Algoritmo de nivelacion de recursos----------------*/
/*********************************************************************/


double Project :: calculCostResource( string name_resource)
{
	int time=end->getTMax();
	double charge=0;
	double cost_day=0;
	Activity::resource * rec_asig;
	double average=0;
	double s=0;
	double t_min;
	int units_max=this->getResource(name_resource)->getUnitsMax();

	/*CALCULAR LA MEDIA PARA ESE RECURSO*/

	/*Recorremos cada dia del proyecto*/
	for(int day=0; day<time;day++)
	{
		charge=0;

		/* Selecciona las actividades que se estan realizando ese dia*/
		for(int i=0; i<activities.size();i++)
		{
			t_min=activities.at(i)->getTMin();
			if(t_min<=day&&t_min+activities.at(i)->getTNormal()-1>=day)
			{
				rec_asig=activities.at(i)->getResource(name_resource);
				if(rec_asig!=NULL)
				{
					average+=rec_asig->units_asig;
					charge+=rec_asig->units_asig;
				}
			}
		}
		/* Prueba factibilidad, se cumpliria la limitacion de recursos?*/
		if(charge>units_max) 
		{
			mode_debug("		Se supera la limitacion de recursos\n");
			return INF;				// como no es factible, coste inifinito
		}
	}

		
	/* Media */
	average=average/time;



	/*CALCULO COEFICIENTE VARIACION */

	/*Recorremos cada dia del proyecto*/
	for(int day=0; day<time;day++)	
	{
		/* S */
		cost_day=0;

		/* Selecciona las actividades que se estan realizando ese dia*/
		for(int i=0; i<activities.size();i++)
		{
			t_min=activities.at(i)->getTMin();
			if(t_min<=day&&t_min+activities.at(i)->getTNormal()-1>=day)
			{
				rec_asig=activities.at(i)->getResource(name_resource);
				if(rec_asig!=NULL)
					cost_day+=rec_asig->units_asig;
			}
		}
		
		s+=(cost_day-average)*(cost_day-average);
	}
	
	s=s/time;
	s=sqrt(s);

	return (s/average);

}

bool Project:: levelResources(std::vector<Resource*> *resources_level)
{
	Activity*act;
	double charges_origin=0;
	double charges_calculated=0;
	std::deque<Activity*> ordered_activities;
	int max_instant;
	int t_best_begin;
	int instant;
	

	if(resources_level->size()==0)
	{
		showErrorInfo(E_ALGORITHM_NOT_EXIST_RESOURCES, " nivelacion de recursos");
		return false;
	}

	/*Ordenar  las actividades en orden decreciente segun el instante de finalizacion mas temprano*/
	for(int i=0; i<activities.size();i++)
		ordered_activities=insert(ordered_activities, activities.at(i),MIN_EFT_FIFO);
	reverse(ordered_activities.begin(),ordered_activities.end());

	mode_debug("********************* NIVELACION RECURSOS *****************\n\n\n");
	mode_debug("Orden de las actividades a mover: ",ordered_activities,"\n");
	
	
	/*Carga disposicion original*/
	for(int i=0; i<resources_level->size();i++)
		charges_origin+=calculCostResource(resources_level->at(i)->getName());
	mode_debug("\nCVoriginal = "+num_to_str(charges_origin)+"\n\n\n");
	
	/*Calcula el instante de inicio de las actividades con menor carga de recursos*/
	for(int i=0; i<ordered_activities.size();i++)
	{
		act=ordered_activities.at(i);
		t_best_begin=act->getTMin();	//parte de intante de inicio mas temprano

		/*Modificar el dia de comienzo de las activities no criticas, consumiendo la holgura libre*/
		instant=act->getTMin()+1;
		max_instant=act->getTMin()+act->getHFree();
		mode_debug(act->getName()+"   TMin Original="+num_to_str(instant)+"  H.Libre="+num_to_str(act->getHFree())+":\n\n");

		/*Mientras haya holgura libre*/
		while(instant<=max_instant)
		{
			/*Inicializa*/
			act->setTMin(instant);
			charges_calculated=0;
			
			/*Calculamos la carga para cada recurso con el CV*/
			for(int r=0; r<resources_level->size();r++)
				charges_calculated+=calculCostResource(resources_level->at(r)->getName());
			mode_debug("	Movemos la actividad "+act->getName()+" al instante "+num_to_str(instant)+" con un CV="+num_to_str(charges_calculated)+"\n");
	
	
			/*La suma total de las cargas de los recursos  mejora respecto a la original?*/
			if(charges_calculated<=charges_origin)
			{
				charges_origin=charges_calculated;
				t_best_begin=instant;
				mode_debug("	La nueva disposicion mejora la anterior\n");
			}
			instant++;
		}

		/*Asignamos la mejora*/
		act->setTMin(t_best_begin);
		mode_debug("	"+act->getName()+" se realiza en "+num_to_str(t_best_begin)+"\n----------------------------------------\n");
		
	}

	/*Actualizar t.max y holguras*/
	updateTMax();
	
	/*Validar si despues de aplicar el algoritmo, se siguen cumpliendo las dependecias y
	la limitacion de recursos*/
	if(!validateDependences())
		exit(-1);
	if(!validateLimitationMaxUnitsAllocated())
		exit(-1);
	return true;

}
bool Project::levelAllResources()
{
	/*Para nivelar todos los recursos le pasamos a la funcion de nivelacion
	todos los recursos del proyecto, junto con el empeoramiento permitido*/
	return levelResources( resources);
}



bool Project::levelResources(std::vector<std::string> names_rec)
{
	std::vector<Resource *> *resources= new std::vector<Resource*>;
	Resource * resource;

	/*Transformamos los nombres de los recursos en recursos y los guardamos*/
	for(int i=0; i<names_rec.size();i++)
	{
		resource=this->getResource(names_rec.at(i));

		if(resource==NULL)
		{
			showErrorInfo(E_RESOURCE_NOT_EXIST, names_rec.at(i));
			return false;
		}
		else
			resources->push_back(resource);
	}

	/*Pasamos a la funcion los recursos que queremos nivelar*/
	return levelResources(resources);
	delete(resources);

	

}