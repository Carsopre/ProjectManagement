#include <string>
#include <vector>

using namespace std;


#ifndef Resource_H_
#define Resource_H_

class Resource {

	std::string name;
	float units_max;				//unidades disponibles del recurso

public:

	Resource(string name);
	Resource(string name, float units_max);
	~Resource();

	void setUnitsMax(int units){  units_max=units;};
	int getUnitsMax(){ return units_max;};
	string getName(){ return name;};


};







/*DISPONIBILIDAD DE RECURSO EN UN INSTANTE EN LIMITACION DE RECURSOS*/

typedef struct {						// estructura que representa las unidades disponibles de un recurso
	Resource * resource;				// recurso
	int units_disp;						// unidades disponibles para asignar del recurso
	
}disp_resource;

typedef struct							// estructura que representa las unidades disponibles de los recursos en un instante
{
	int instant;						// instante de tiempo
	vector<disp_resource> resources;	// unidades de cada recurso disponible en ese instante

}disp_instant;


class CompareDisponibilities			// clase auxiliar para permitir ordenar las disponibilidades por el instante
{
	public:
    bool operator()(disp_instant d1, disp_instant d2)
    {
       if (d1.instant < d2.instant) return true;
       return false;
    }
};



#endif;