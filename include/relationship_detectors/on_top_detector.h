#ifndef ON_TOP_DETECTOR_H
#define ON_TOP_DETECTOR_H

#include "relationship_detector.h"
#include "on_top_relationship.h"

#include <boost/shared_ptr.hpp>

class OnTopDetector: public RelationshipDetector
{
	public:
		
		virtual void computeRelationship();
}; 

#endif