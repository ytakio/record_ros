#include "record_ros/record.h"
#include <ros/ros.h>
#include <boost/thread.hpp>

Record::Record(ros::NodeHandle &nh,rosbag::RecorderOptions const& options):
    rosbag::Recorder(options)
{
    service_srv = nh.advertiseService("cmd",&Record::string_command,this);
    b_record    = false;

}

void Record::wait_for_callback(){
    ros::Rate r(100); // 60 hz
    while (!b_record && ros::ok())
    {
        ros::spinOnce();
        r.sleep();
    }
}

bool Record::string_command(record_ros::String_cmd::Request& req, record_ros::String_cmd::Response& res){
    bool f_record = req.f_record;
    ROS_INFO("Record callback");
    if(f_record == true){
        if(b_record){
            res.msg = "stopping recorder";
        }else{
            b_record = true;
            res.msg  = "starting recorder";
        }
        return true;
    }
	else{
        ros::shutdown();
        res.msg = "stopping recorder";
        return true;
    }
	return true;
}
