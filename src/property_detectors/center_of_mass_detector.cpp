#include "center_of_mass_detector.h"

void CenterOfMassDetector::calculatePropertyValue()
{
    double x_total = 0;
    double y_total = 0;
    double z_total = 0;

    for(int i =0; i < segmentedObject->pointCloudPtr->size() ; i++)
    {
        x_total += segmentedObject->pointCloudPtr->points[i].x;
        y_total += segmentedObject->pointCloudPtr->points[i].y;
        z_total += segmentedObject->pointCloudPtr->points[i].z;
    }

    geometry_msgs::Point center_of_mass_point;
    center_of_mass_point.x = x_total/segmentedObject->pointCloudPtr->size();
    center_of_mass_point.y = y_total/segmentedObject->pointCloudPtr->size();
    center_of_mass_point.z = z_total/segmentedObject->pointCloudPtr->size();

    centerOfMassProperty = PropertyFactory::getInstance().buildCenterOfMassProperty(segmentedObject->uniqueId, center_of_mass_point);
}

boost::shared_ptr<CenterOfMassProperty> CenterOfMassDetector::getCenterOfMassProperty()
{
	return centerOfMassProperty;
}
