#include <sstream>
#include "Resource.h"
#include "Exceptions.h"
#include "Activity.h"
#include <set>


using namespace std;

/************************************************************/
/*						CONSTRUCTORES						*/	
/************************************************************/
Activity::Activity(string name,int tn, int tp, float cnormal, float cuoport)
{
	this->name=name;
	t_normal=tn;
	t_tope=tp;
	cost_normal=cnormal;
	cost_oportunity=cuoport;
	h_total=0;
	h_free=0;
	t_min=-1;
	t_max=-1;
}

Activity::Activity(std::string name,int tn, float cnormal)
{
	this->name=name;
	t_normal=tn;
	t_tope=tn;
	cost_normal=cnormal;
	cost_oportunity=cnormal;
	h_total=0;
	h_free=0;
	t_min=-1;
	t_max=-1;
}

Activity::Activity(){}

/***************************************************/
/*					DESTRUCTOR					   */
/***************************************************/
Activity::~Activity()
{
	/*Destruir las relaciones*/
	for(int i=0;i<getList_Activities_pred()->size();i++)
		delete(getList_Activities_pred()->at(i));
	for(int i=0;i<getList_Activities_suc()->size();i++)
		delete(getList_Activities_suc()->at(i));

	/*Destruir las asignaciones de recurso*/
	for(int i=0;i<resources.size();i++)
		delete(resources.at(i));

	
}

/********************************************/
/*				METODOS						*/
/********************************************/

void Activity::clear()
{
	/*Inicializa la actividad*/
	h_total=0;
	h_free=0;
	t_min=-1;
	t_max=-1;
}


void Activity::modify(int t_normal, int t_tope, float cost_normal, float cost_oportunity)
{
	/*Modifica los parametros de la actividad*/
	this->t_normal=t_normal;
	this->t_tope=t_tope;
	this->cost_normal=cost_normal;
	this->cost_oportunity=cost_oportunity;
}


bool Activity::predecesorIsInitial()
{
	/*Comprueba si su predecesora es la actividad ficticia inicio*/
	if(getList_Activities_pred()->size()==1&&getList_Activities_pred()->at(0)->activity->getName()=="Inicio")
		return true;
	return false;

}

bool Activity::succesorIsEnd()
{	
	/*Comprueba si su succesora es la actividad ficticia fin*/
	if(getList_Activities_suc()->size()==1&&getList_Activities_suc()->at(0)->activity->getName()=="Fin")
		return true;
	return false;


}

/************************************************************************/
/*							RELACIONES									*/
/************************************************************************/

Activity::relation::relation(Activity * act)
{
	this->activity=act;
}


void Activity::addSuccesor(Activity * succ)
{
	/*Añadir una succesora*/
	relation *relation_succ=new relation(succ);
	this->List_Activities_suc.push_back(relation_succ);
}

void Activity::addPredecesor(Activity * pred)
{
	/*Añadir una predecesora*/
	relation *relation_pred=new relation(pred);
	this->List_Activities_pred.push_back(relation_pred);
}


int Activity::getPositionRelationPredecesor(string name_pred)
{
	/*Obtener la posicion dentro de las relaciones de precedencia,
	donde se encuentra la relacion con la actividad dada*/
	for(int i=0; i<this->getList_Activities_pred()->size();i++)
	{
		if(getList_Activities_pred()->at(i)->activity->name.compare(name_pred)==0)
			return i;
	}

	return -1;// no es predecesora 

}

int Activity::getPositionRelationSuccesor(std::string name)
{
	/*Obtener la posicion dentro de las relaciones de succesoras,
	donde se encuentra la relacion con la actividad dada*/
	for(int i=0; i<this->getList_Activities_suc()->size();i++)
	{
		if(getList_Activities_suc()->at(i)->activity->name.compare(name)==0)
			return i;
	}

	return -1; // no es succesora

}


bool Activity::isContained(std::vector<Activity* > List)
{
	/*Comprueba si la acttividad esta dentro del vector dado*/
	for(int j=0; j<List.size();j++)
		if(List.at(j)->getName()==getName())
			return true;

	return false;

}

bool Activity::validateDependences()
{
	bool result=true;

	/*Comprueba si las dependecias se cumplen*/
	for(int i=0; i< this->List_Activities_suc.size();i++)

		/*La succesoras no pueden empezar antes de que terminen las predecesoras*/
		if(t_min+t_normal>List_Activities_suc.at(i)->activity->t_min)
		{
			showErrorInfo(E_DEPENDENCES, name+" TN="+num_to_str(t_min+t_normal)+"   y   "+List_Activities_suc.at(i)->activity->name+" TN="+num_to_str(List_Activities_suc.at(i)->activity->t_min));
			result=false;
		}
	return result;

}

std::string Activity::succesorsToString()
{
	/*Obtener las actividades succesoras en formato string*/
	std::string path_text="";

	/*No indicamos como succesora la actividad ficticia fin*/
	if(this->succesorIsEnd())
		return path_text;

	/*String de nombres de las succesoras separadas por guion*/
	for(int i=0; i<this->List_Activities_suc.size(); i++)
	{
		if(i!=0)
			path_text.append("-");	
		path_text.append(List_Activities_suc.at(i)->activity->getName());

	}
	return path_text;

}

/******************	CAMINO CRITICO	******************/

int Activity::calculMinTime()
{
	int max=-1;
	Activity * act;

	if(List_Activities_pred.empty())
		return 0;


	/*Examinar lista de predecesoras*/
	for(int i=0; i<List_Activities_pred.size();i++)
	{
		act=List_Activities_pred.at(i)->activity;

		/*Si la succesora no tiene tiempo minimo, calcularlo*/
		if(act->t_min==-1)
			act->t_min=act->calculMinTime();
			
		/*Maximo instante de finalizacion temprano entre las predecesoras*/
		if(max<act->t_min+act->t_normal)
			max=act->t_min+act->t_normal;
			
	}

	return max;
}


int Activity::calculMaxTime()
{
	int min=10000000000000; // infinito
	Activity * act;

	if(List_Activities_suc.empty())
		return t_min;   

	/*Examina las succesoras*/
	for(int i=0; i<List_Activities_suc.size();i++)
	{
		act=List_Activities_suc.at(i)->activity;

		/*Si la succesora no tiene calculado el TMAX, calcularlo*/
		if(act->t_max==-1)
		{
			act->t_max=act->calculMaxTime();

			/*Holgura total*/
			act->h_total=act->t_max-act->t_min;
		}
		
		/*Minimo instante de comienzo mas tardio de las succesoras*/
		if(min>act->t_max-t_normal)
			min=act->t_max-t_normal;
			
	}

	return min;

}


void Activity::calculHolguraFree( int time)
{
	h_free=time;	// como maximo usamos la propia duracion del proyecto 

	/*Es la actividad de fin*/
	if(List_Activities_suc.empty())
		h_free=time-(t_normal+t_min);   


	else
	{	/*Explora sus succesoras*/
		for(int i=0; i<this->List_Activities_suc.size();i++)
		{
			/*Minima diferencia entre el TMIN de una succesora y el instante de finalizacion mas temprano de la actividad*/
			if(h_free>List_Activities_suc.at(i)->activity->t_min-(t_normal+t_min))
				h_free=List_Activities_suc.at(i)->activity->t_min-(t_normal+t_min);

			/*Calculamos la holgura libre para la succesora*/
			List_Activities_suc.at(i)->activity->calculHolguraFree(time);
		}

	}

}



int Activity::getHLA() //holgura libre de retraso
{
	Activity * act;
	int max=0;
	if(List_Activities_pred.empty())
		return 0;

	/*Examinar lista de predecesoras*/
	for(int i=0; i<List_Activities_pred.size();i++)
	{
		act=List_Activities_pred.at(i)->activity;
			
		/*Maximo intante de comienzo mas tardio entre las predecesoras*/
		if(max<act->t_min+act->t_normal)
			max=act->t_min+act->t_normal;
			
	}

	/*Tiempo desde que podria empezar la actividad hasta que realmente empieza*/
	return t_min-max;

}






/************************************************************/
/*						RECURSOS							*/
/*************************************************************/


Activity::resource::resource(Resource *resource_asig, int units)
{
	this->resource_asig=resource_asig;
	this->units_asig=units;
}


int Activity::getPositionResource(string name_rec){

	/*Posicion dentro de las asignaciones, donde esta la asignacion
	del recurso dado*/
	for(int i=0; i<this->resources.size();i++)
		if(resources.at(i)->resource_asig->getName()==name_rec)
			return i;

	return -1; // no ha sido asignado
}

Activity::resource* Activity::getResource(string name_rec){
	
	/*Obtener la asignacion del recurso dado*/
	for(int i=0; i<this->resources.size();i++)
		if(resources.at(i)->resource_asig->getName()==name_rec)
			return resources.at(i);

	return NULL;	// no ha sido asignado
}

void Activity:: incrementResource(int value, string name)
{

	int pos=getPositionResource(name);
	
	if(pos==-1)
		showErrorInfo( E_RESOURCE_NOT_ALLOCATED, name+" no esta asignado a la actividad "+this->name);
	
	/*El incremento dela asignacion superara al maximo de unidades del recurso?*/
	else if(resources.at(pos)->units_asig+value>resources.at(pos)->resource_asig->getUnitsMax())
		showErrorInfo(E_RESOURCE_MAX_EXCEDED, name);

	else
		this->resources.at(pos)->units_asig+=value;

}




void Activity:: decrementResource(int value, string name_resource){


	int pos=getPositionResource(name_resource);
	
	if(pos==-1)
		showErrorInfo( E_RESOURCE_NOT_ALLOCATED, name_resource+" no esta asignado a la acitividad "+this->name);

	/*No se puede quitar lo que no se tiene asginado*/
	else if(resources.at(pos)->units_asig<value)
		showErrorInfo(E_VALUE_RESOURCE, "Se esta intentando asinar un value negativo al resource "+name_resource);

	else
	{
		resources.at(pos)->units_asig-=value;

		/*Si no hay unidades asignadas eliminamos el la asignacion*/
		if(resources.at(pos)->units_asig==0)
		{
			showAvisoInfo(A_UNITS_RESOURCE_0, name+" tiene 0 unidades asignadas del recurso "+name_resource );
			resources.erase(resources.begin()+pos);
		}
	}
}

void Activity::allocateResource(Resource *resource_asign, int value){

	int pos=getPositionResource(resource_asign->getName());
	Activity::resource * resource;

	/*El recurso ya esta añadido, cambiamos su valor*/
	if(pos!=-1){
		showAvisoInfo(A_RESOURCE_ALREADY_ADDED, name+" ya tiene asignado "+ resource_asign->getName() );
		this->getResource(resource_asign->getName())->units_asig=value;
	
	}
	else /*Lo asignamos creandolo*/
	{
		resource=new Activity::resource(resource_asign,value);
		this->resources.push_back(resource);
	}
}

void Activity::deleteResource(Resource *resource){

	int pos=getPositionResource(resource->getName());

	if(pos==-1)
		showErrorInfo( E_RESOURCE_NOT_ALLOCATED, resource->getName()+" no esta asignado a la actividad "+name );
	
	else //borra la asignacion del recurso
		resources.erase(resources.begin()+pos);
}




bool Activity::isEnoughResource(std::set<disp_instant, CompareDisponibilities>::iterator disponibility)
{

	/*Comprobar que para cada disponibilidad de un recurso, no se consume mas alla de esa disponiblidad*/
	for(int i=0; i<disponibility->resources.size();i++)
	{
		resource * res=getResource(disponibility->resources.at(i).resource->getName());
		if(res!=NULL&&res->units_asig>disponibility->resources.at(i).units_disp)
			return false;
	}
	return true;
}

bool Activity::isEnoughResource(disp_instant disponibility)
{
	
	for(int i=0; i<disponibility.resources.size();i++)
	{
		resource * res=getResource(disponibility.resources.at(i).resource->getName());
		if(res!=NULL&&res->units_asig>disponibility.resources.at(i).units_disp)
			return false;
	}
	return true;
}



