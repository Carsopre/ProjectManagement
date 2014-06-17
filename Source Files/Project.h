#include "Activity.h"
//#include "Recurso.h"
#include "Path.h"
//#include "header.h"

#include <deque>
//#include "Recurso.h"
#include <string>
#include <sstream>
#include <fstream>








using namespace std;
#ifndef PROJECT_H_
#define PROJECT_H_



#define INF 1000000000000




class Project {

			/*ACTIVIDADES*/
			vector<Activity*> activities;			// actividades del proyecto
			
			/*CAMINOS CRITICOS*/
			vector<Path*> *critical_paths;			// caminos criticos del proyecto

			/*RECURSOS*/
			vector<Resource *> * resources;			// recursos del proyecto

			/*COSTES*/
			float overrun;							// sobrecoste
		
			/*ACTIVIDADES AUXILIARES*/
			Activity * begin;						// actividad auxiliar de incio
			Activity * end;							// actividad auxiliar de fin

			/*DEBUG*/
			string report;							// nombre del fichero donde guardar la informacion de debug
			string sub_report;						// nombre del subfichero segun el algoritmo que se debuguee 
			ofstream output;						// puntero fichero debug
			bool enabled;							// modo debug habilitado o no


		private:

			int  getPositionActivity(string name);		// devuelve la posicion de la actividad dentro del vector de actividades
			int  getPositionResource(string name);		// devuelve la posicion del recurso dentro del vector de recursos
			bool validateDependences();					// valida que los instantes de tiempo entre relaciones sean correcto, dtodas las succesoras deben empezar mas tarde que las predecesoras
			bool validateLimitationMaxUnitsAllocated();	// valida que la asignacion de cada uno de los recursos, por dia, no supera el maximo de uniddades cada dia
			void clear();								// borra los datos calculados del proyecto 

			/*CAMINO CRITICO*/
			void extractCriticalPaths( Path* path, Activity * partida );	// extrae los caminos criticos que forman parte del proyecto

			/*NIVELACION RECURSOS*/
			bool levelResources(vector<Resource*> *resources);				// nivela los recursos de forma que para mejorar debe mejorar la suma de los CV de los recursos
			double calculCostResource(string name_resource);					// calcula el coste de la asignacion de ese recurso a lo largo de los dias que dura el proyecto
			
			/*CALCULO MIN COSTE MIN DURACION*/
			void decrement_days(vector<Path*> *paths,int max_decr_dias,Path* act_to_modif);												// actualiza la duracion de las actividades
			float get_min_cost(vector<Path*> paths_a_modif, int n_act,Path * activities_to_modif_pruebas,Path* activities_to_modif_sol);// selecciona la mejor combinacion de actividades que reducen los caminos criticos que no superen el sobrecoste permitido
			Path* select_activities_min_cost(vector<Path*> *paths, float& cost, int &max_decr_days, int cost_searched);					// selecciona la mejor combinacion de actividades que reducen los caminos criticos
			Path* select_activities_min_cost(vector<Path*> *paths, float& cost, int &max_decr_days);	
			void extractPaths( vector<Path*> *paths,Path * path, Activity* partida );													// obtiene todos los caminos que forman parte del proyecto


			/*LIMITACION RECURSOS*/
			float limitationResourcesSerie(int rule);									// limitar recursos con el esquema serie y regla de prioridad rule
			float limitationResourcesParalel(int rule);									// limitar recursos con el esquema paralelo y regla de prioridad rule
			float HBRPMultiPasada();													// limitar recursos con multipasada
			float limitationResourcesSerie( deque<Activity*> elegir, string step);		// proceso de adelanto/atraso (step) de las actividades despues de la limitacion de recursos con esquema serie
			float limitationResourcesParalel( deque<Activity*> elegir, string step);	// proceso de adelanto/atraso (step) de las actividades despues de la limitacion de recursos con esquema paralelo

			/*DEBUG*/
			void mode_debug(string text);												// metodos para mostrar los pasos de los algoritmos si esta habilitado el modo debug		
			void mode_debug(string before,vector<Activity*> *path, string after);
			void mode_debug(string before,deque<Activity*> path, string after);
			void mode_debug(string before,vector<disp_resource> disp, int t, string after);
			void mode_debug(string before,set<disp_instant, CompareDisponibilities> *disponibility, string after);



		public:

			
			Project();
			~Project(void);
			vector<Path*>*  getCriticalPaths(){ return critical_paths;};					// devuelve los caminos criticos
			int sizeCriticalPaths(){ return critical_paths->size();}						// numero de caminos criticos
			vector<Activity*> getActivities(){ return activities;};							// devuelve las actividades
			int sizeActivities(){ return activities.size();}								// numero de acitvidades
			vector<Resource *>* getResources(){ return resources;};							// devuelve los recursos
			int sizeResources(){ return resources->size();}									// numero de recursos
			Activity * getEnd(){ return end; };												// actividad de fin
			Activity * getBegin(){ return begin;}											// actividad de inicio
			float getTotalCost();															// devuelve el coste total de proyecto, incluyendo el sobrecoste
			float getOverrun(){ return overrun;}											// devuelve el sobrecoste del proyecto

			/*ACTIVIDADES*/
			void addActivity(string name, int tn, int tp, float cnormal, float cuoport);	// añade una actividad incluyendo el tiempo tope y coste de oportunidad
			void addActivity(string name, int tn,  float cnormal);							// añade una acitvidad
			void modifyActivity(string name, int tn, int tp, float cnormal, float cuoport);	// modifica la actividad nomb
			Activity * getActivity(string name);											// devuelve la actividad name
			void deleteActivity(string name);												// borra la actividad name
			void updateTMax();																// actualiza el tiempo maximo de las actividades segun el tiempo minimo que tengan

			/*RELACIONES*/
			void addRelation(string orig, string dest);										// añade una relacion de orig a dest
			void deleteRelation(string orig, string dest);									// borra la relacio de orig a dest
	

			/*RECURSOS*/
			Resource * getResource(string name);											// devuelve el recurso name
			void addResource(string name, int units_max);									// añade un recurso
			void modifyResource(string name, int units_max);								// modifica el recurso
			void deleteResource(string name);												// borra el recurso name
			void allocateResourceActivity(string resource, string activity, int units);		// asigna una cantidad de recurso a una actividad
			void deleteResourceActivity(string resource, string Activity);					// quita la asignacion del recurso a la actividad
			void incrementUnitsResourceActivity(string resource, string activity, int units);//incrementa el numero de unidades del recurso asignadas a la actividad 
			void decrementUnitsResourceActivity(string resource, string activity, int units);//decrementa el numero de unidades del recurso asignadas a la actividad 
			vector<double>  getAllocationResourcePerDay(string name);							// devuelve en el vector las unidades asignadas del recurso cada dia
			

			/*ALGORITMO CAMINO CRITICO*/
			void calculCriticalPath();													// algoritmo de calculo de camino critico

			/*ALGORITMO DE NIVELACION DE RECURSOS*/
			bool levelAllResources();													// nivela todos los recursos permitiendo, de forma que para mejorar debe mejorar la suma de los CV de los recursos
			bool levelResources(vector<string> names_rec);								// nivela los recursos names_Rec independientemente unos de otros
			
		
			/*ALGORITMO ACKOFF-SASIENI: MINIMO COSTE MINIMA DURACION*/
			void calculPathMinTimeMinCost();											// algoritmo minimo coste minima duracion
			void calculPathMinTimeMinCost(int cost_searched);							// algoritmo min coste min duracion sin que el sobrecoste diario sea mayor a cost_searched
			

			/*ALGORITMO limitacion DE RECURSOS*/
			bool limitarResources(int rule, int eschema);								// limitacion de recursos con una regla de prioridad y esquema 
			bool retrasoAdelantoActivities(int eschema_retraso, int eschema_adelanto);	// proceso de adelanto/atraso despues de la limitacion

			
			/*ANALISIS*/
			float getProbabDurationLessThan(int x, double var);			// devuelve la probabilidad de que el proyecto finalize antes de x dias
			float getProbabDurationBetween(int x,int y, double var);	// probabilidad de que en fin de proyecto este entre x e y
			int getDurationByProbability(double x, double var);			// instante fin del proyecto con una probabilidad maxima x
			string analizeDelayTMin(int days, string name_activity);	// devuelve el informe de realizar el analisis de flexibilidad si se retrasa una actividad
			string analizeAnticipateTMin(int days,string name_activity);// devuelve el informe de realizar el analisis de flexibilidad si se adelanta una actividad
			
			/*DEBUG*/
			void setReportDebug(string path);							// asigna un directorio donde guardar los resultados
			bool startDebug(std::string sub_report_chosed);				// empieza a depurar
			void finishDebug();											// termina de depurar
			void enableDebug();											// habilita el modo debug
			void disableDebug();										// deshabilita el modo debug


};

string num_to_str(float n) ; // funcion auxiliar para tranformar floats en cadenas

#endif 
