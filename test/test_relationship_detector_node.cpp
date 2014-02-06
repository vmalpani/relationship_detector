#include <ros/ros.h>
#include "relationship_detector/ObjectCenterProperty.h"
#include "relationship_detector/SegmentedObjectList.h"
#include "relationship_detector/SegmentedObject.h"
#include "gtest/gtest.h"
#include "std_msgs/String.h"
#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>
#include <ros/package.h>

namespace relationship_detector_node_test
{
    class TestRelationshipDetectorNode
    {
        private:
            ros::NodeHandle node_handle;


        public:
            TestRelationshipDetectorNode();

            //will publish segmented objects for the relationshipDetectorNode to look at
            ros::Publisher segmentedObjectsPublisher;

            ros::Subscriber detectedPropertiesSubscriber;
            void relatedObjectsMessageCallback(const relationship_detector::ObjectCenterProperty::ConstPtr &msg);

            bool hasReceivedMessage;
            relationship_detector::ObjectCenterProperty receivedMsg;
    };


    TestRelationshipDetectorNode::TestRelationshipDetectorNode(): node_handle("")
    {
        detectedPropertiesSubscriber = node_handle.subscribe("object_properties", 1000, &TestRelationshipDetectorNode::relatedObjectsMessageCallback, this);

        segmentedObjectsPublisher = node_handle.advertise<relationship_detector::SegmentedObjectList>("segmented_objects",10);

        hasReceivedMessage = false;

        ROS_INFO("test_relationship_detection_node ready");
    }


    void TestRelationshipDetectorNode::relatedObjectsMessageCallback(const relationship_detector::ObjectCenterProperty::ConstPtr &msg)
    {
        ROS_INFO("Received related object list message");
        hasReceivedMessage = true;
        receivedMsg = *msg;



    }

}


//create a node to send messages to the relationship_detector_node so we can validate its responses.
relationship_detector_node_test::TestRelationshipDetectorNode buildTestNode()
{

    relationship_detector_node_test::TestRelationshipDetectorNode node;

    //give test node time to initialize VERY IMPORTANT
    ros::Duration(1).sleep();

    return node;
}


relationship_detector::SegmentedObjectList buildAppleSegmentedObjectsList()
{
    //get point cloud
    std::string fileName = ros::package::getPath("object_models") + "/models/rgbd-dataset/apple/apple_1/apple_1_1_100.pcd";

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

    if (pcl::io::loadPCDFile<pcl::PointXYZ> (fileName, *cloud) == -1) //* load the file
    {
      PCL_ERROR ("Couldn't read file \n");
    }
    sensor_msgs::PointCloud2 sensorMessagePointCloud;
    pcl::toROSMsg(*cloud,sensorMessagePointCloud);

    //build segmented object and add point cloud to it
    relationship_detector::SegmentedObject segmentedObject;
    segmentedObject.segmentedObjectID = 1;
    segmentedObject.segmentedObjectPointCloud = sensorMessagePointCloud;

    //build segmentedObjectList and add segmented object to it.
    relationship_detector::SegmentedObjectList segmentedObjectsList;
    segmentedObjectsList.segmentedObjects.push_back(segmentedObject);
    return segmentedObjectsList;
}


TEST(RELATIONSHIP_DETECTOR_TEST_NODE, TestEmptySegmentedObjectsList) {

  relationship_detector_node_test::TestRelationshipDetectorNode node = buildTestNode();
  relationship_detector::SegmentedObjectList segmentedObjectsList = buildAppleSegmentedObjectsList();

  node.segmentedObjectsPublisher.publish(segmentedObjectsList);

  double startTimeInSeconds =ros::Time::now().toSec();
  while(!node.hasReceivedMessage)
  {
      ros::spinOnce();
      double currentTimeInSeconds =ros::Time::now().toSec();

      //we should have received a message by now, something is wrong.
      if(currentTimeInSeconds-startTimeInSeconds > 5)
      {
          break;
      }
  }

  EXPECT_EQ(node.hasReceivedMessage, true);

  double absErrorBound = .0001;
  ASSERT_NEAR(node.receivedMsg.objectCenter.x, -0.0127071, absErrorBound);
  ASSERT_NEAR(node.receivedMsg.objectCenter.y, 0.699493, absErrorBound);
  ASSERT_NEAR(node.receivedMsg.objectCenter.z, -0.0152639, absErrorBound);
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "test_relationship_detector_node");
  ros::NodeHandle nh;

  //give the relationship_detector_node time to start up.
  ros::Duration(1).sleep();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
