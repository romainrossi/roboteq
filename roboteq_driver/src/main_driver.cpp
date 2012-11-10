#include "ros/ros.h"
#include "roboteq/main_driver.h"
#include "roboteq/roboteq.h"
#include "geometry_msgs/Twist.h"


#define LIN_MAX 1000
#define ANG_MAX 1000
#define l 2	//need to put the real value here.


 roboteq* motor_controller_FR;
volatile int globali=0;


void setMotorSpeedsFromControler(const geometry_msgs::Twist::ConstPtr& msg)
 {
	float ls_vel,rs_vel;
	float norm_vel,norm_ang; 
 	ROS_INFO("I heard: (%lf,%lf)", msg->linear.x,msg->angular.z);
	
	norm_vel=msg->linear.x;
	norm_ang=msg->angular.z;

	norm_vel=norm_vel/sqrt(norm_vel*norm_vel+norm_ang*norm_ang);
	norm_ang=norm_ang/sqrt(norm_vel*norm_vel+norm_ang*norm_ang);

	//ls_vel=norm_vel*LIN_MAX - norm_ang*ANG_MAX;
	//rs_vel=norm_vel*LIN_MAX + norm_ang*ANG_MAX;
	ls_vel=msg->linear.x*LIN_MAX - msg->angular.z*ANG_MAX;
	rs_vel=msg->linear.x*LIN_MAX + msg->angular.z*ANG_MAX;

	ROS_INFO("LS: %lf,  RS: %lf",ls_vel,rs_vel);
 }

void test_function(const ros::TimerEvent&)
{
	ROS_INFO("Getting Voltages");
	motor_controller_FR->getVoltages();
	ROS_INFO("Motor Drive voltage %f \n Bat Voltage %f analog Voltage %f\n",motor_controller_FR->drive_voltage,motor_controller_FR->battery_voltage,motor_controller_FR->analog_voltage);
/*	motor_controller_FR->getMotorCurrent();
	ROS_INFO("Motor Drive Current 1 %f \n Motor Drive Current 2 %f \n",motor_controller_FR->Motor1Current,motor_controller_FR->Motor2Current);

*/
globali++;
	motor_controller_FR->setVAR(1, globali);


}

void test_function2(const ros::TimerEvent&)
{
	ROS_INFO("Wuddup 2");
//	ROS_INFO("Wuddup 2 ,var = %d", motor_controller_FR->readVAR(1));

}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "thor_driver");
	ros::NodeHandle n;
	
	//Publishers subscribers here
	
	//Subscribe to twist messages for commanded velocity.
	ros::Subscriber commandedTwist = n.subscribe("/husky/cmd_vel", 100, setMotorSpeedsFromControler);
	//rospy.Subscriber("plan_cmd_vel", Twist, self.planned_callback)
	//roboteq motor_controller_RR;
	//motor_controller setup here
	//motor_controller_RR = new roboteq("/dev/ttyACM0", 115200);  // Front left controller
	
	motor_controller_FR = new roboteq("/dev/ttyACM0", 115200);  // Front left controller
	//roboteq* motor_controller_FL = new roboteq("/dev/ttyACM1", 115200);  // Front right controller
	//roboteq* motor_controller_RR = new roboteq("/dev/ttyACM2", 115200);  // Front right controller
	//roboteq* motor_controller_RL = new roboteq("/dev/ttyACM3", 115200);  // Front right controller
	
	motor_controller_FR->setupComm();
	//motor_controller_FL->setupComm();
	//roboteq::roboteq("ttyACM0", 115200) 

	

	ros::Rate loop_rate (1);

	//Setup Serial Port

	//setup timers for getting data from roboteq
	ros::Timer timer = n.createTimer(ros::Duration(1), test_function);
	ros::Timer timer2 = n.createTimer(ros::Duration(2), test_function2);
	//ros::Timer timer3 = n.createTimer(ros::Duration(2), test_function2);

	ros::spin();

	return 0;
}


	