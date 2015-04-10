#include <iostream>
#include <stdio.h>

#include "ros/ros.h"
#include "sensor_msgs/Imu.h"

ros::Publisher imuPublisher;

#include "serial/serial.h"

int main(int argc, char **argv)
{
	ROS_INFO("Starting ros_arduino_imu_node");

	ros::init(argc, argv, "ros_arduino_imu");
	ros::NodeHandle node;

	ros::Publisher imuPublisher = node.advertise<sensor_msgs::Imu>("/imu", 1);

	// port, baudrate, timeout in milliseconds
	serial::Serial imuSerial("/dev/ttyACM1", 115200, serial::Timeout::simpleTimeout(1000));

	if(imuSerial.isOpen())
	{
		ROS_INFO("Successfully connected to IMU Arduino.");
	}
	else
	{
		ROS_INFO("Shit. Failed to connect.");
	}

	ROS_INFO("Press Ctrl-C to kill node.");

	// Spin
	ros::Rate loopRate(10); // 10 hz

	while(ros::ok())
	{
		// Send IMU data message
		sensor_msgs::Imu msg;

		// Data Index
		imuSerial.readline(20, " ");

		// Linear Acceleration

		// x
		std::stringstream ssX(imuSerial.readline(20, " "));
		float xAcc;
		ssX >> xAcc;
		msg.linear_acceleration.x = xAcc;

		// y
		std::stringstream ssY(imuSerial.readline(20, " "));
		float yAcc;
		ssY >> yAcc;
		msg.linear_acceleration.y = yAcc;

		// z
		std::stringstream ssZ(imuSerial.readline(20, " "));
		float zAcc;
		ssZ >> zAcc;
		msg.linear_acceleration.z = zAcc;

		// x_gyro
		std::stringstream ssX_gyro(imuSerial.readline(20, " "));
		float xGyro;
		ssX_gyro >> xGyro;
		msg.angular_velocity.x = xGyro;

		// y_gyro
		std::stringstream ssY_gyro(imuSerial.readline(20, " "));
		float yGyro;
		ssY_gyro >> yGyro;
		msg.angular_velocity.y = yGyro;

		// z_gyro
		std::stringstream ssZ_gyro(imuSerial.readline(20, " "));
		float zGyro;
		ssZ_gyro >> zGyro;
		msg.angular_velocity.z = zGyro;

		imuPublisher.publish(msg);

		// Altitude & rest of line
		imuSerial.readline();

		// ROS Spin & Sleep
		ros::spinOnce();
		loopRate.sleep();
	}

	ros::shutdown();

	return 0;
}