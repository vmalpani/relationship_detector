#ifndef CENTER_OF_MASS_DETECTOR_H
#define CENTER_OF_MASS_DETECTOR_H

#include "property_detector.h"
#include "center_of_mass_property.h"
#include <boost/shared_ptr.hpp>

class CenterOfMassDetector: public PropertyDetector
{
public:
	virtual void computeProperty();

};

#endif