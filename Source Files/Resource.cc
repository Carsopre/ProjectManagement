#include "Resource.h"
#include "Exceptions.h"
#include <string.h>
/************************************/
/*			Constructores			*/
/************************************/
Resource::Resource(std::string name)
{	
	this->name=name;
	this->units_max=1;
	}

Resource::Resource(std::string name, float units_max)
{
	this->name=name;
	this-> units_max=units_max;
}

/*************************/
/*		Destructor		 */
/*************************/
Resource::~Resource(){}




