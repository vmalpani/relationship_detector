#include <ros/package.h>
#include <string>
#include "gtest/gtest.h"
#include "center_of_mass_detector.h"
#include <iostream>

//catkin_make relationship_detector-test
//rosrun relationship_detector relationship_detector-test or catkin_make test



TEST(ExampleTestCase, ExampleTestName) {
  EXPECT_EQ(1, 1);
}


TEST(CenterOfMassDetector, TestApple) {
  char fileName[1024];

  sprintf(fileName,(ros::package::getPath("object_models") + "/models/rgbd-dataset/apple/apple_1/apple_1_1_100.pcd").c_str());
  //want something like this:
  //sprintf(fileName,"%s%s",ros::package::getPath("object_models"), "/models/rgbd-dataset/apple/apple_1/apple_1_1_100.pcd");


  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

  if (pcl::io::loadPCDFile<pcl::PointXYZ> (fileName, *cloud) == -1) //* load the file
  {
    PCL_ERROR ("Couldn't read file \n");
  }

  CenterOfMassDetector centerOfMassDetector = CenterOfMassDetector();

  centerOfMassDetector.setPointCloud(*cloud);
  centerOfMassDetector.calculatePropertyValue();

  CenterOfMassProperty centerOfMassProperty = centerOfMassDetector.getCenterOfMassProperty();

  double absErrorBound = .0001;
  ASSERT_NEAR(centerOfMassProperty.centerOfMass.x, -0.0127071, absErrorBound);
  ASSERT_NEAR(centerOfMassProperty.centerOfMass.y, 0.699493, absErrorBound);
  ASSERT_NEAR(centerOfMassProperty.centerOfMass.z, -0.0152639, absErrorBound);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
