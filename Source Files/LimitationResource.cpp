#include "Project.h"
#include <algorithm>
#include <math.h>
#include "Exceptions.h"
#include "Rules.h"
//#include "OtherStructures.h"

/**************************************************************************/
/*		ALGORITMO LIMITACION RECURSOS									  */
/**************************************************************************/

disp_instant createDisponibility(int t, Activity * act, set<disp_instant, CompareDisponibilities>::iterator disponibility )
{
	disp_resource resource_in_t;
	disp_instant resources_in_t;
	Activity::resource * rec;

	/*Para todos los recursos, añadirle a las unidades del instante anterior, 
	las unidades que devulveria esta actividad*/
	for(int i=0; i<disponibility->resources.size();i++)
	{
		rec=act->getResource(disponibility->resources.at(i).resource->getName());
		resource_in_t.resource=disponibility->resources.at(i).resource;

		if(rec!=NULL)	// la actividad consumia este recurso
			resource_in_t.units_disp=rec->units_asig+ disponibility->resources.at(i).units_disp;
		else			//la actividad no consumia del recurso
			resource_in_t.units_disp=disponibility->resources.at(i).units_disp;
		resources_in_t.resources.push_back(resource_in_t);
		
	}
	resources_in_t.instant=t;
	return resources_in_t;

}





int get_sequencing_instant(std::set<disp_instant, CompareDisponibilities>  *disponibility, Activity * act)
{
	int t_earliest=0;
	set<disp_instant, CompareDisponibilities>::iterator it=disponibility->begin();
	Activity * act_pred;

	/*Encontrar el instant mas pronto que puede empezar*/
	if(!act->predecesorIsInitial()) // si no es una actividad de comienzo
	{
		/*Calcular cual de sus predecesoras termina mas tarde, 
		que seria el isntante mas pronto que podria empezar la actividad*/
		for(int pred=0;pred<act->getList_Activities_pred()->size();pred++)
		{
			act_pred=act->getList_Activities_pred()->at(pred)->activity;
			if(act_pred->getTSecuenciacion()+act_pred->getTNormal()>t_earliest)
				t_earliest=act_pred->getTSecuenciacion()+act_pred->getTNormal();
		}
	}


	/*Situarse en el instante que mas pronto que puede empezar donde haya disponibilidad de recursos*/
	while(disponibility->end()!=it&& it->instant<t_earliest) it++; // asi tinc dubte de si shauria de fer it-- si no es igual al isntat
	

	/*Comprobar si todo el tiempo que dura la actividad desde que se inicia hay suficientes recursos*/
	while(disponibility->end()!=it&&it->instant< t_earliest+act->getTNormal() )
	{

		/*En la lista no hay mas disponibilidades, por lo que no hay impedimiento para que comienze en el t_earliest*/
		if(disponibility->end()==it)
			return t_earliest; // t mas pronto

		/*Si no hay suficiente recurso en ese instante, la actividad comenzara el siguiente instante, 
		siempre que este instante lo permita, cosa que se comprueba duranre la siguiente interacion*/
		if(!act->isEnoughResource(it))
		{
			it++;
			t_earliest=it->instant;
		}else
			it++;
	}
	return t_earliest;
}

void update_disponibility(std::set<disp_instant, CompareDisponibilities> *disponibility, Activity *act, int t)
{
		int i=0;
		Activity::resource *rec;
		set<disp_instant, CompareDisponibilities>::iterator it=disponibility->begin();
		
		/*Dentro de la lista de disponibilidades, situarse en el momento que empezaria a consumir recursos*/
		while(disponibility->end()!=it&& it->instant<t) it++;

		/*Consumir los recursos de los diferentes instantes de disponibilidad, hasta que termine la actividad*/
		while(disponibility->end()!=it&& it->instant<t+act->getTNormal())
		{
			/*Actulizar disponibilidades de todos los recursos*/
			for(int k=0; k<it->resources.size();k++)
			{
				rec=act->getResource(it->resources.at(k).resource->getName());

				if(rec!=NULL)			//la actividad no tiene asignado ese recurso
 					it->resources.at(k).units_disp-=rec->units_asig;
			}
			it++;
		}
		it--;

		/*Si el tiempo que dura la actividad va mas alla de los instantes de disponibilidad guardados, 
		añadir una nueva disponibilidad, que sera el resultado de devolver a la disponibilidad anterior
		los recursos consumidos*/
		if(it->instant!=t+act->getTNormal())
			disponibility->insert(createDisponibility(t+act->getTNormal(), act, it));
		
}

bool all_predecesors_examined(Activity* act, std::vector<Activity*> examined)
{
	int j;

	/*Si es una actividad de inicio no hay problema*/
	if(act->predecesorIsInitial())
		return true;

	/*Examinar todas las predecesoras y comprobar que estan vistas*/
	for(int i=0; i<act->getList_Activities_pred()->size();i++)
	{
		/*Esta ya examinada?*/
		for(j=0; j<examined.size();j++)
		{
			if(act->getList_Activities_pred()->at(i)->activity->getName()==examined.at(j)->getName())
				break;
		}
		/*Alguna no se ha examinado aun*/
		if(j==examined.size())
			return false;
	}	
	return true;

}



vector<procesing_activity> insert_proces(vector<procesing_activity> procesing, procesing_activity act ){
	
	int i;

	/*Ordena las actividades procesandose crecientemente segun su instante de finalizacion de proceso */
	if(procesing.size()==0){
		procesing.push_back(act);
		return procesing;
	}

	for(i=0; i<procesing.size();i++){
		if(act.instant <procesing.at(i).instant){
			procesing.insert(procesing.begin()+i,act); //inserta el proceso en la posicion i
			return procesing;
		}
	}
	
	procesing.push_back(act);
	return procesing;


}
float Project:: limitationResourcesSerie(int rule)
{
	int n_act_to_process=activities.size();
	std::set<disp_instant, CompareDisponibilities> *disponibility=new set<disp_instant, CompareDisponibilities>;
	std::deque<Activity*> selected;
	std::vector<Activity*> examined;
	int t=0;
	float t_max=0;
	disp_resource resource_in_t;
	disp_instant resources_in_t;
	Activity * act;
	
	if(this->resources->size()==0)
	{
		showErrorInfo(E_ALGORITHM_NOT_EXIST_RESOURCES, " limitacion de recursos en serie");
		return INF;
	}

	/*INICIALIZACION*/
	mode_debug("\n\n**********************SERIE************************\n\n");

	/*Disponibilidad inicial, t=0*/
	for(int i=0; i<this->resources->size();i++)
	{
		resource_in_t.resource=resources->at(i);
		resource_in_t.units_disp=resources->at(i)->getUnitsMax();
		resources_in_t.resources.push_back(resource_in_t);
		
	}
	resources_in_t.instant=0;
	disponibility->insert(resources_in_t);

	/*Actividades elegibles son las actividades de inicio*/
	for(int i=0; i< begin->getList_Activities_suc()->size();i++)
		selected=insert(selected, begin->getList_Activities_suc()->at(i)->activity, rule);
		
	mode_debug("",disponibility,"\n");
		
	/*Se examinan todas las actividades*/
	while(n_act_to_process>0)
	{
		mode_debug("\n\nITERACION ____________________________________\n");
		mode_debug("	elegir={",selected,"}\n");


		/*Actividad a secuenciar*/
		act=selected.front();
		selected.pop_front();

		/*calcular instante en que se secuenciara*/
		t= get_sequencing_instant(disponibility, act);

		/*Sequenciar*/
		act->setTSecuenciacion(t);
		examined.push_back(act);
		mode_debug("	La actividad "+act->getName()+" se secuencia en "+num_to_str(t)+"\n");

		/*Guardamos cual es el instante de fin del proyecto,
		que será cuando finalize la actividad que termina mas tarde*/
		if(t+act->getTNormal()>t_max)
			t_max=t+act->getTNormal();
			

		/*actualizar elegibles con las succesoras de la actividad secuenciada, 
		solo si todas las predecesoras de cada succesora han sido vistas*/
		for(int i=0; i< act->getList_Activities_suc()->size();i++)
			if(all_predecesors_examined(act->getList_Activities_suc()->at(i)->activity, examined)&&!act->succesorIsEnd())
				selected=insert(selected,act->getList_Activities_suc()->at(i)->activity, rule);
		
		/*actualizar disponibilidad*/
		update_disponibility(disponibility, act, t);
		mode_debug("	",disponibility,"\n");
		
	
		n_act_to_process--;
	
	}

	delete(disponibility);
	return t_max;


}

/*********** Paralel***************************/



float Project:: limitationResourcesParalel(int rule)
{
	std:: vector<procesing_activity> procesing;
	disp_instant disponibility;
	std::deque<Activity*> selected;
	std::vector<Activity*> examined;
	int t=0;
	int j=0;
	disp_resource resource_in_t;
	Activity * act;
	Activity::resource * rec;
	procesing_activity procesing_act, act_proces_fin;
	
	
	if(this->resources->size()==0)
	{
		showErrorInfo(E_ALGORITHM_NOT_EXIST_RESOURCES, " limitacion de recursos en paralelo");
		return INF;
	}

	/*Inicializacion*/
	mode_debug("\n\n********************PARALELO*******************\n\n");

	/*Disponibilidad inicial, t=0*/
	for(int i=0; i<this->resources->size();i++)
	{
		resource_in_t.resource=resources->at(i);
		resource_in_t.units_disp=resources->at(i)->getUnitsMax();
		disponibility.resources.push_back(resource_in_t);
	}
	disponibility.instant=0;
	
	/*Actividades elegibles son las actividades de inicio*/
	for(int i=0; i< begin->getList_Activities_suc()->size();i++)
		selected=insert(selected, begin->getList_Activities_suc()->at(i)->activity, rule);
	
	/*Secuenciar actividades*/
	while(procesing.size()!=0||t==0)
	{
		/*Hay alguna actividad procesandose?*/
		if(procesing.size()!=0)
		{
			act_proces_fin=procesing.front(); // obtiene la actividad que termina antes de procesarse
			t=act_proces_fin.instant;
		}
		mode_debug("\n\nITERACION ___T="+num_to_str(t)+"_________________________________\n");

		/*actualizar el procesamiento, todas borrar las actividades que terminan en t y añadirlas a vistas*/
		while(procesing.size()>0&&act_proces_fin.instant==t)
		{
			mode_debug("	La actividad "+act_proces_fin.act->getName()+" termina de procesarse\n");
			
			/*Añadirlas a vistas*/
			examined.push_back(act_proces_fin.act);
			procesing.erase(procesing.begin());

			/*Restaurar la disponibilidad*/
			for(int k=0; k<disponibility.resources.size();k++)
			{
				rec=act_proces_fin.act->getResource(disponibility.resources.at(k).resource->getName());
				if(rec!=NULL) //no tiene asignado ese recurso
					disponibility.resources.at(k).units_disp+=rec->units_asig;
			}
			
		
			/*actualizar elegibles con las succesoras de la actividad secuenciada, 
			solo si todas las predecesoras de cada succesora han sido vistas*/
			for(int i=0; i< act_proces_fin.act->getList_Activities_suc()->size();i++)
				if(all_predecesors_examined(act_proces_fin.act->getList_Activities_suc()->at(i)->activity, examined)&&!act_proces_fin.act->succesorIsEnd())
					selected=insert(selected,act_proces_fin.act->getList_Activities_suc()->at(i)->activity, rule);
			
			/*Siguiente procesandose*/
			if(procesing.size()>0)
				act_proces_fin=procesing.front();
		
		}
		mode_debug("	",disponibility.resources,t,"\n");
		mode_debug("	elegir={",selected,"}\n");

		/*Comprobar que actividades elegibles se pueden secuenciar*/
		j=0;
		while(!selected.empty()&&j<selected.size())
		{
			/*Hay suficientes recursos para secuenciar esa actividad?*/
			if(selected.at(j)->isEnoughResource(disponibility))
			{
				/*Pasa a procesarse*/
				act=selected.at(j);
				procesing_act.act=act;
				procesing_act.instant=t+act->getTNormal();
				procesing=insert_proces(procesing,procesing_act);
				act->setTSecuenciacion(t); 
				selected.erase(selected.begin()+j);

				mode_debug("	Se secuencia "+act->getName()+" en "+num_to_str(t)+" y termina en "+num_to_str(procesing_act.instant)+"\n");
	
				/*Consume los recursos disponibles*/
				for(int k=0; k<disponibility.resources.size();k++)
				{
					rec=act->getResource(disponibility.resources.at(k).resource->getName());
					if(rec!=NULL) //no tiene asignacion de ese recurso
						disponibility.resources.at(k).units_disp-=rec->units_asig;
				}
			}else
				j++;
		}
		mode_debug("	",disponibility.resources, t,"\n");

	}

	return t;
}



float Project::HBRPMultiPasada()
{
	float best=INF;
	int best_rule=MIN_LFT_FIFO;
	int best_eschema=_SERIE;
	float temporal;

	if(this->resources==NULL)
	{
		showErrorInfo(E_ALGORITHM_NOT_EXIST_RESOURCES, "HBRP multi pasada");
		return INF;
	}

	mode_debug("#################### MULTI PASADA ##############################\n\n");

	/*Para cada regla*/
	for(int rule=MIN_LFT_FIFO; rule<=MAX_NSUC_FIFO;rule++)
	{
		/*Resultado con el esquema paralelo*/
		temporal=this->limitationResourcesParalel(rule);
		if(best>temporal) //es best, update
		{
			best=temporal;
			best_rule=rule;
			best_eschema=_PARALEL;
		}

		/*Resultado con el esquema secuencial*/
		temporal=this->limitationResourcesSerie(rule);
		if(best>temporal)// es mejor, actualizar
		{
			best=temporal;
			best_rule=rule;
			best_eschema=_SERIE;
		}
	
	}
	mode_debug("Mejor esquema y mejor regla con coste "+num_to_str(best)+" es la siguente\n");

	if(best==INF)
		return INF;	//no se ha podido aplicar el algoritmo

	/*Una vez se sepa cual es el mejor resultado, aplicarlo*/
	if(best_eschema==_SERIE)
		return limitationResourcesSerie(best_rule);
	else
		return limitationResourcesParalel(best_rule);

}


bool Project:: limitarResources(int rule, int eschema)
{
	float t;

	/*Resolver segun el esquema eledigo y la regla de prioridad*/
	if(eschema==_SERIE)
		t=limitationResourcesSerie(rule);
	else if(eschema==_PARALEL)
		t=limitationResourcesParalel(rule);
	else 
		t=HBRPMultiPasada();

	if(t==INF)
		return false;	//no se ha podido aplicar el algoritmo
	
	/*actualizar los resultados*/
	for(int i=0; i<activities.size();i++)
		activities.at(i)->setTMin(activities.at(i)->getTSecuenciacion());
	end->setTMin(t);

	/*Actualizar t.max y las holguras*/
	this->updateTMax();
	
	/*comprobar que el resultado es valido*/
	if(!validateDependences())					//cumple las dependencias entre succesor y predecesor
		exit(-1);
	if(!validateLimitationMaxUnitsAllocated())	//cumple con la limitacion diaria de recursos
		exit(-1);


	return true;
}

/************************************************************************/
/*					RETRASO / ADELANTO									*/
/************************************************************************/



int get_sequencing_instant_suc(std::set<disp_instant, CompareDisponibilities>  *disponibility, Activity * act)
{
	int t_earliest=0;
	set<disp_instant, CompareDisponibilities>::iterator it=disponibility->begin();

	/*Encontrar el instante mas pronto que puede empezar, dependiendo de cuando terminen sus sucesoras*/
	if(!act->succesorIsEnd())
	{
		for(int suc=0;suc<act->getList_Activities_suc()->size();suc++)
			if(act->getList_Activities_suc()->at(suc)->activity->getTSecuenciacion()+act->getList_Activities_suc()->at(suc)->activity->getTNormal()>t_earliest)
				t_earliest=act->getList_Activities_suc()->at(suc)->activity->getTSecuenciacion()+act->getList_Activities_suc()->at(suc)->activity->getTNormal();
	}


	/*Situarse en ese instante*/
	while(disponibility->end()!=it&& it->instant<t_earliest) it++;
	

	while(disponibility->end()!=it&&it->instant< t_earliest+act->getTNormal() )
	{
		if(disponibility->end()==it)
			return t_earliest;

		if(!act->isEnoughResource(it))
		{
			it++;
			t_earliest=it->instant;
		}else
			it++;
	}
	return t_earliest;
}

bool dependenciesFinished(string step, Activity * act, std::vector<Activity *> examined)
{
	/*Comprobar si ya estan examinadas las dependencias*/
	if(step.compare("retraso")==0)
	{
		/*En retraso tenemos que ver el proyecto del reves por lo que intenresa comprobar las succesoras*/
		for(int i=0; i< act->getList_Activities_suc()->size();i++)
			if(!act->getList_Activities_suc()->at(i)->activity->isContained(examined)&&!act->succesorIsEnd())
				return false;
	}
	else
	{	/*En adelanto atenderemos a lasp redecesoras*/
		for(int i=0; i< act->getList_Activities_pred()->size();i++)	
			if(!act->getList_Activities_pred()->at(i)->activity->isContained(examined)&&!act->predecesorIsInitial())
				return false;
	}
	return true;
}

float Project:: limitationResourcesParalel( deque<Activity*> selected, string step)
{
	std:: vector<procesing_activity> procesing;
	disp_instant disponibility;
	std::vector<Activity*> examined;
	int t=0, j=0;
	float t_max=0;
	disp_resource resource_in_t;
	Activity * act;
	Activity::resource * rec;
	procesing_activity procesing_act, act_proces_fin;
	
	
	if(this->resources->size()==0)
	{
		showErrorInfo(E_ALGORITHM_NOT_EXIST_RESOURCES, " proceso de "+step+" de actividades en paralelo");
		return INF;
	}

	/*Inicializacion*/
	mode_debug("\n\n********************PARALELO*******************\n\n");

	/*Disponibilidad inicial, t=0*/
	for(int i=0; i<this->resources->size();i++)
	{
		resource_in_t.resource=resources->at(i);
		resource_in_t.units_disp=resources->at(i)->getUnitsMax();
		disponibility.resources.push_back(resource_in_t);
	}
	disponibility.instant=0;

	/*Secuenciar actividades*/
	while(procesing.size()!=0||t==0)
	{

		mode_debug("\n\nITERACION ___T="+num_to_str(t)+"_________________________________\n");

		/*Hay alguna actividad procesandose?*/
		if(procesing.size()!=0)
		{
			act_proces_fin=procesing.front();
			t=act_proces_fin.instant;
		}

		
		/*actualizar el procesamiento, todas borrar las actividades que terminan en t y añadirlas a vistas*/
		while(procesing.size()>0&&act_proces_fin.instant==t)
		{
			mode_debug("	La actividad "+act_proces_fin.act->getName()+" termina de procesarse\n");
			
			/*Añadirlas a vistas*/
			examined.push_back(act_proces_fin.act);
			procesing.erase(procesing.begin());

			/*Restaurar la disponibilidad*/
			for(int k=0; k<disponibility.resources.size();k++)
			{
				rec=act_proces_fin.act->getResource(disponibility.resources.at(k).resource->getName());
				if(rec!=NULL) // consume de ese recurso?
					disponibility.resources.at(k).units_disp+=rec->units_asig;
			}
			
			if(procesing.size()>0)
				act_proces_fin=procesing.front();
		}

		mode_debug("	",disponibility.resources,t,"\n");
		mode_debug("	elegir={",selected,"}\n");


		/*Comprobar que actividades elegibles se pueden secuenciar*/
		j=0;
		while(!selected.empty()&&j<selected.size())
		{
			/*Hay suficientes recursos para secuenciar esa actividad?*/
			if(selected.at(j)->isEnoughResource(disponibility)&&dependenciesFinished(step,selected.at(j),examined))
			{
				/*Pasa a procesarse*/
				act=selected.at(j);
				procesing_act.act=act;
				procesing_act.instant=t+act->getTNormal();
				procesing=insert_proces(procesing,procesing_act);
				act->setTSecuenciacion(t); 
				selected.erase(selected.begin()+j);

				mode_debug("	Se secuencia "+act->getName()+" en "+num_to_str(t)+" y termina en "+num_to_str(procesing_act.instant)+"\n");
				
				/*Consume los recursos disponibles*/
				for(int k=0; k<disponibility.resources.size();k++)
				{
					rec=act->getResource(disponibility.resources.at(k).resource->getName());
					if(rec!=NULL)// no esta asignado ese recurso
						disponibility.resources.at(k).units_disp-=rec->units_asig;
				}
			}else
				j++;
		}
		mode_debug("	",disponibility.resources, t,"\n");

	}

	return t;
}


float Project:: limitationResourcesSerie( deque<Activity*> selected, string step)
{
	int n_act_to_process=activities.size();
	std::set<disp_instant, CompareDisponibilities> *disponibility=new set<disp_instant, CompareDisponibilities>;
	std::vector<Activity*> examined;
	int t;
	int t_max=0;
	disp_resource resource_in_t;
	disp_instant resources_in_t;
	Activity * act;
	
	if(this->resources->size()==0)
	{
		showErrorInfo(E_ALGORITHM_NOT_EXIST_RESOURCES, " proceso de "+step+" de actividades en serie");
		return INF;
	}

	/*INICIALIZACION*/
	mode_debug("\n\n**********************SERIE************************\n\n");
	
	/*Disponibilidad inicial, t=0*/
	for(int i=0; i<this->resources->size();i++)
	{
		resource_in_t.resource=resources->at(i);
		resource_in_t.units_disp=resources->at(i)->getUnitsMax();
		resources_in_t.resources.push_back(resource_in_t);
		
	}
	resources_in_t.instant=0;
	disponibility->insert(resources_in_t);
	
	/*Se examinan todas las actividades*/
	while(n_act_to_process>0)
	{
		mode_debug("\n\nITERACION ____________________________________\n");
		mode_debug("	elegir={",selected,"}\n");

		/*Actividad a secuenciar*/
		act=selected.front();
		selected.pop_front();

		/*calcular instante en que se secuenciara*/
		if(step.compare("retraso")==0)
			t= get_sequencing_instant_suc(disponibility, act);	// comprueba las succesoras
		else
			t= get_sequencing_instant(disponibility, act);		//comprueba las predecesoras

		/*Sequenciar*/
		act->setTSecuenciacion(t);
		examined.push_back(act);
		mode_debug("	La actividad "+act->getName()+" se secuencia en "+num_to_str(t)+"\n");
		

		/*Guardamos cual es el instante de fin del proyecto,
		que será cuando finalize la actividad que termina mas tarde*/
		if(t+act->getTNormal()>t_max)
			t_max=t+act->getTNormal();

		/*actualizar disponibilidad*/
		update_disponibility(disponibility, act, t);
		mode_debug("	",disponibility,"\n");
	
		n_act_to_process--;
	
	}

	delete(disponibility);
	return t_max;


}
bool Project::retrasoAdelantoActivities(int eschema_retraso, int eschema_adelanto){

	float max1, max2;
	deque<Activity*> activities_ordered;

	do{

		mode_debug("\n\n#################### ADELANTO/ATRASO#####################\n\n");
		
		/*Ordenar actividades en orden decreciente de finalizacion*/
		for(int i=0; i<this->activities.size();i++)
			activities_ordered=insert(activities_ordered,activities.at(i), MIN_EFT_FIFO);
		reverse(activities_ordered.begin(), activities_ordered.end()); //orden decreciente

		/*RETRASO: aplicar esquema con regla*/
		mode_debug("||||||||||||||||||||||||| RETRASO|||||||||||||||||||||||\n");
		mode_debug("elegir={",activities_ordered,"}\n");

		if(eschema_retraso==_SERIE)
			max1=limitationResourcesSerie( activities_ordered, "retraso");
		else
			max1=limitationResourcesParalel( activities_ordered,"retraso");

		if(max1==INF)
			return false;	//no se ha podido aplicar el algoritmo

		/*Traducir tiempos de secuenciacion*/
		for(int i=0; i<this->activities.size();i++)
			activities.at(i)->setTMin(max1-activities.at(i)->getTSecuenciacion()-activities.at(i)->getTNormal());
		end->setTMin(max1);

		/*Comprobar que el algoritmo lo resuelva bien*/
		if(!validateDependences())
			exit(-1);
		if(!validateLimitationMaxUnitsAllocated())
			exit(-1);

		/*Ordenar actividades en orden creciente de inicio*/
		activities_ordered.clear();
		for(int i=0; i<this->activities.size();i++)
			activities_ordered=insert(activities_ordered,activities.at(i), MIN_EST_FIFO);


		/*ADELANTO: aplicar esquema con regla*/
		mode_debug("||||||||||||||||||||||||| ADELANTO|||||||||||||||||||||||\n");
		mode_debug("elegir={",activities_ordered,"}\n");

		if(eschema_adelanto==_SERIE)
			max2=limitationResourcesSerie( activities_ordered, "adelanto");
		else
			max2=limitationResourcesParalel( activities_ordered,"adelanto");

		if(max2==INF)
			return false;	//no se ha podido aplicar el algoritmo

		/*actualizar t.min*/
		for(int i=0; i<activities.size();i++)
			activities.at(i)->setTMin(activities.at(i)->getTSecuenciacion());
		end->setTMin(max2);

		/*Actualizar t.max y holguras*/
		updateTMax();

		/*Comprobar si el algoritmo lo resuelve bien*/
		if(!validateDependences())
			exit(-1);
		if(!validateLimitationMaxUnitsAllocated())
			exit(-1);

		

	}
	/*Estos pasos se aplicaran mientras no se converja a una solucion*/
	while(max1-max2!=0);

	return true;

}
