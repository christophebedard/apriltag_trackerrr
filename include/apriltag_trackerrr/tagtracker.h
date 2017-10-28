#ifndef TAG_TRACKER_H
#define TAG_TRACKER_H

#include "apriltags_ros/AprilTagDetectionArray.h"
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_listener.h>
#include <image_transport/image_transport.h>
#include <image_geometry/pinhole_camera_model.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/JointState.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

static const double LOOP_RATE = 10.0;
static const std::string CAMERA_NAMESPACE = "camera";
static const std::string IMAGE_RECT_TOPIC_NAME = "image_rect";
static const std::string TAG_DETECTIONS_TOPIC_NAME = "tag_detections";
static const std::string BASE_TF_NAME = "/joint0";
static const std::string TAG_TF_NAME_PREFIX = "/tag_";
static const std::string JOINT_STATE_COMMAND_TOPIC_NAME = "/robotis/dynamixel/goal_states"; // /command
static const std::string TAG_TARGET_POSE_TOPIC_NAME = "/target";
static const int TARGET_TAG_ID = 27;

class TagTracker
{
    public:
        /*
         * Constructor
         */
        TagTracker(ros::NodeHandle& n);
        /*
         * Destructor
         */
        ~TagTracker();

        /*
         * ROS spin. Called only once (by node); contains ROS while loop
         */
        void spin();


    protected:
        /*===========================
         * Update
         *===========================*/
        /*
         * Update; called every spinOnce()
         */
        void update();

        /*
         * ROS spin once, called on every loop
         */
        void spinOnce();


    private:
        ros::NodeHandle n_;
        bool is_running_slowly_;
        double loop_hz_;

        std::vector<apriltags_ros::AprilTagDetection> detected_tags_;

        tf::TransformListener tf_listener_;
        image_transport::ImageTransport it_;
        image_geometry::PinholeCameraModel cam_model_;
        /*===========================
         * Subscribers
         *===========================*/
        /* Camera image subscriber */
        image_transport::CameraSubscriber image_sub_;
        /* Apriltag detection subscriber */
        ros::Subscriber tag_detect_sub_;

        /*===========================
         * Publishers
         *===========================*/
        /* Publisher for image with tracking error info */
        image_transport::Publisher track_image_pub_;
        /* Publisher for joint state command */
        ros::Publisher joint_state_command_pub_;
        /* Publisher for tag target pose */
        ros::Publisher tag_target_pose_pub_;

        /*===========================
         * Callbacks
         *===========================*/
        /*
         * Callback class method for camera image
         */
        void imageCallback(const sensor_msgs::ImageConstPtr& msg, const sensor_msgs::CameraInfoConstPtr& cam_info);

        /*
         * Callback class method for apriltag detection
         */
        void tagPositionCallback(const apriltags_ros::AprilTagDetectionArray::ConstPtr& msg);

        /*===========================
         * Other utilities
         *===========================*/
        /*
         * Create PoseStamped message from position vector and yaw
         */
        geometry_msgs::PoseStamped createPoseStampedFromPosYaw(double yaw, std::string frame);

        sensor_msgs::JointState createJointStateFromAngle(double angle);

};

#endif  // APRILTAG_TRACKERRR_TAG_TRACKER_H