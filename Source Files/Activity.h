#include <vector>
#include <string>
#include <set>
#include "Resource.h"

//#include "OtherStructures.h"
using namespace std;




#ifndef ACTIVIDAD_H_
#define ACTIVIDAD_H_


class Activity {

	string name;

	/*Instantes*/
	int t_normal;					// tiempo normal
	int t_tope;						// tiempo maximo al que se puede reducir el tiempo normal
	int t_min;						// instante de inicio minimo
	int t_max;						// instante de inicio maximo
	int dif_t_normal_tope;			// tiempo normal - tiempo tope , se usa en el algoritno ackoff
	int t_secuenciacion;			// parametro auxiliar , instante de secuenciacion en la limitacion de recursos
	
	/*Costes*/
	float cost_normal;				//coste normal
	float cost_oportunity;			//coste si la actividad se realizase en el tiempo tope

	/*Holguras*/
	int h_total;					// holgura total
	int h_free;						// holgura libre y holgura libre de retraso




public:

	/*Recurso*/
	typedef struct resource								// estructura que representa la asignacio de un recurso
	{
		Resource* resource_asig;						// recurso asignado
		int units_asig;									// unidades del recurso asignadas a la acitvidad
		resource(Resource *resource_asig, int units);
	};

	std::vector<resource*> resources;					// lista de los recursos asignados
	


	/*Relaciones*/
	typedef struct relation								//estructura que representa la relacion de la actividad con otras actividades
	{
		Activity * activity;							// actividad con la que se establece la relacion
		relation(Activity * activity);					
	};

	vector<relation*> List_Activities_pred;				// lista de actividades predecesoras
	vector<relation*> List_Activities_suc;				// lista de actividades succesoras


	
	/*Metodos*/
	Activity(string name, int t_normal, int t_tope, float cost_normal, float cost_oportunity);
	Activity(string name, int t_normal, float cost_normal);
	Activity();
	~Activity();

	void clear();																	// resetea la actividad	
	void modify(int t_normal, int t_tope, float cost_normal,float cost_oportunity); //modifica la actividad
	bool isContained(vector<Activity*> list);										//comprueba si la acitvidad esta incluida dentro del vector

	string getName(){ return name;}
	void setTNormal(int time){ t_normal=time;}
	int getTNormal(){ return t_normal;}
	int getTTope(){ return t_tope;}

	
	/*Instantes*/
	void setTMin(int t_min){ this->t_min=t_min;}
	int getTMin(){ return t_min;}
	void setTMax(int t_max){ this->t_max=t_max;}
	int getTMax(){ return t_max;}
	int getEarlyStart(){ return t_min;}									// Inicio temprano
	int getLateStart(){ return t_max;}									// Inicio tardio
	int getEarlyEnd(){ return t_min+t_normal;}							// Fin temprano
	int getLateEnd(){ return t_max+t_normal;}							// Fin tardio
	int getTSecuenciacion(){ return t_secuenciacion;}
	void setTSecuenciacion(int t){ this->t_secuenciacion=t;}
	void setDifTNormalTope(){ this->dif_t_normal_tope=t_normal-t_tope;} //Calcula el tiempo que se puede reducir
	void setDifTNormalTope( int dif){ this->dif_t_normal_tope=dif;}
	int getDifTNormalTope(){ return dif_t_normal_tope;};
	
	
	/*Holguras*/
	void setHTotal(int h_total){ this->h_total=h_total;}
	int getHTotal(){ return h_total;}
	void setHFree(int h_free){ this->h_free=h_free;}
	int getHFree(){ return h_free;}
	void calculHolguraFree(int time );								// calcula la holgura libre
	int getHLR(){ return h_free;}									// holgura libre de retraso
	int getHLA();													// holgura libre de adelanto

	/*Costes*/
	float getCostNormal(){ return cost_normal;}
	float getOportunityCost(){ return this->cost_oportunity;};

	
	/*Predecesora*/
	vector<relation *> *getList_Activities_pred(){return &List_Activities_pred;};
	void addPredecesor(Activity * pred);						// crea una relacion desde una acitvidad a esta		
	int getPositionRelationPredecesor(string name);				//devuelve la posicion en el vector predecesoras donde esta la relacion con la actividad name
	int calculMinTime( );										// calcula el tiempo minimo de la actividad
	bool predecesorIsInitial();									// comprueba si es una actividad de inicio
	

	/*Succesora*/
	vector<relation *> *getList_Activities_suc(){return &List_Activities_suc;};
	void addSuccesor(Activity * succ);							// crea una relacion hace una acitvidad desde esta	
	int getPositionRelationSuccesor(string name);				//devuelve la posicion en el vector succesoras donde esta la relacion con la actividad de nombre
	int calculMaxTime( );										// calcula el tiempo maximo de la actividad
	bool succesorIsEnd();										// comprueba si es una actividad de fin
	string succesorsToString();									// obtiene la lista de nombre de las succesoras

	bool validateDependences();									// comprueba si se complen las relaciones de precedencias a la hora de calcular los instantes


	/*Recursos*/
	void incrementResource(int value, string resource);			// incrementa las unidades asignadas del recurso si este ha sido asignado previamente
	void decrementResource(int value, string resource);			// decrementa las unidades asignadas del recurso si este ha sido asignado previamente
	void allocateResource(Resource * resource, int value);		// asigna a la actividad unidades de un recurso, reseteando las unidades si ya estaba asignado
	void deleteResource(Resource *resource);					// elimina la asignacion que tiene la actividad del recurso resource
	int getPositionResource(string resource);					// obtiene la posicion en el vector resources donde esta el recurso
	Activity::resource* getResource(string name_resource);		// obtiene la asignacion del recurso a la actividad, si no esta asignado devulve null
	vector<Activity::resource*> getResources(){return resources;}	
	bool isEnoughResource(set<disp_instant, CompareDisponibilities>::iterator disponibility); //comprueba si hay suficientes recursos para que pueda empezar la actividad
	bool isEnoughResource(disp_instant disponibility);			
		
 
};



/* ACTIVIDAD PROCESANDOSE EN LIMITACION DE RECURSOS*/

typedef struct						// estructura para representar las actividades que se estan procesando en la limitacion de recursos
{		
	int instant;					// instante que termina de procesarse la actividad
	Activity * act;					// actividad procesandose

}procesing_activity;


class CompareActProcesandose{		// clase auxiliar para ordenar de las actividades que estan procesandose por instante de fin de proceso
	public:
		bool operator()(procesing_activity d1, procesing_activity d2)
		{
		   if (d1.instant < d2.instant) return true;	
		   return false;
		}
};


string num_to_str(float n) ;



#endif;


