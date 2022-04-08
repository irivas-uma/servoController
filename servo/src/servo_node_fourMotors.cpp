#include "servo_node_fourMotors.h"


namespace srcHals {
	
	 servo_node2::servo_node2(int argc, char **argv, const char *node_name): ROSCommonNode(argc, argv, node_name),
	 servoOne(false),
	 servoTwo(false),
	 servoThree(false),
	 servoFour(false){
	 
        // Valores máximos y mínimos de los servos. Para establecerlos, abrir ./MaestroServoController
        oneMin = 922; 
        oneMax = 1922;
        twoMin = 922;
        twoMax = 1922;
        threeMin = 922;
        threeMax = 1922;
        fourMin = 922;
        fourMax = 1922; 
/*
		nh->param("servo1_min", oneMin, 922);
		nh->param("servo1_max", oneMax, 1922);
		
		nh->param("servo2_min", twoMin, 922);
		nh->param("servo2_max", twoMax, 1922);
		
		nh->param("servo3_min", threeMin, 922);
		nh->param("servo3_max", threeMax, 1922);
		
		nh->param("servo4_min", fourMin, 922);
		nh->param("servo4_max", fourMax, 1922);*/
		
		//Node publishers
		pubs[0] = nh->advertise<std_msgs::Int16>("/EndoWrist/servo1/pos", 1000);
		pubs[1] = nh->advertise<std_msgs::Int16>("/EndoWrist/servo2/pos", 1000);
		pubs[2] = nh->advertise<std_msgs::Int16>("/EndoWrist/servo3/pos", 1000);
		pubs[3] = nh->advertise<std_msgs::Int16>("/EndoWrist/servo4/pos", 1000);
		
		//Node subscribers
		subs[0] = nh->subscribe("/EndoWrist/servo1/target", 1000, &servo_node2::cbServoOneTarget, this);		
		subs[1] = nh->subscribe("/EndoWrist/servo2/target", 1000, &servo_node2::cbServoTwoTarget, this);	
		subs[2] = nh->subscribe("/EndoWrist/servo3/target", 1000, &servo_node2::cbServoThreeTarget, this);	
		subs[3] = nh->subscribe("/EndoWrist/servo4/target", 1000, &servo_node2::cbServoFourTarget, this);
	 
		run();
	 
	 }
	 
	 int servo_node2::getServoPosition (){
		 
		 short positions[4];
		 
		 controller.getServoPosition(positions, 4);
		 
		 std_msgs::Int16 posOne, posTwo, posThree, posFour;
		 posOne.data = positions[0];
		 posTwo.data = positions[1];
		 posThree.data = positions[2];
		 posFour.data = positions [3];
		 
		 pubs[0].publish(posOne);
		 pubs[1].publish(posTwo);
		 pubs[2].publish(posThree);
		 pubs[3].publish(posFour);
	 }
	 
	 void servo_node2::run(){
		int pos1,pos2,pos3,pos4;	 
	    ros::Rate loop_rate(125);
	     
		controller.setTarget(oneMin, 0);
		controller.setTarget(twoMin, 1);
		controller.setTarget(threeMin, 2);
		controller.setTarget(fourMin, 3);
		
		 
		 while (ros::ok()){
			 
			getServoPosition();
			
			if (servoOne){
				pos1 = oneMin + positionOne*(oneMax-oneMin)/100;
				controller.setTarget(pos1, 0);
				std::cout<<"setTarget servo1 to "<<pos1<<std::endl;
				servoOne = false;
			}
			
			if (servoTwo){
				pos2 = twoMin + positionTwo*(twoMax-twoMin)/100;
				controller.setTarget(pos2, 1);
				std::cout<<"setTarget servo2 to "<<pos2<<std::endl;
				servoTwo = false;
			}
			
			if (servoThree){
				pos3 = threeMin + positionThree*(threeMax-threeMin)/100;
				controller.setTarget(pos3, 2);
				std::cout<<"setTarget servo3 to "<<pos3<<std::endl;
				servoThree = false;
			}
			
			if (servoFour){
				pos4 = fourMin + positionFour*(fourMax-fourMin)/100;
				controller.setTarget(pos4, 3);
				std::cout<<"setTarget servo4 to "<<pos4<<std::endl;
				servoFour = false;
			}
			 			
			
			ros::spinOnce();
			loop_rate.sleep();			 
		 }
		 
	 }
	 
	 void servo_node2::cbServoOneTarget(const std_msgs::Int16::ConstPtr& msg){
		 positionOne = msg->data;
		 if (positionOne>=0 & positionOne<=100){
			servoOne = true;}
		 else{std::cout<<"[ERROR] Target must be bewteen 0 and 100"<<std::endl;;}			
	 }
	 
	 void servo_node2::cbServoTwoTarget(const std_msgs::Int16::ConstPtr& msg){
		 positionTwo = msg->data;
		 if (positionTwo>=0 & positionTwo<=100){
			servoTwo = true;}
		 else{std::cout<<"[ERROR] Target must be bewteen 0 and 100"<<std::endl;;}			
	 }
	 
	  void servo_node2::cbServoThreeTarget(const std_msgs::Int16::ConstPtr& msg){
		 positionThree = msg->data;
		 if (positionThree>=0 & positionThree<=100){
			servoThree = true;}
		 else{std::cout<<"[ERROR] Target must be bewteen 0 and 100"<<std::endl;;}			
	 }
	 
	 void servo_node2::cbServoFourTarget(const std_msgs::Int16::ConstPtr& msg){
		 positionFour = msg->data;
		 if (positionFour>=0 & positionFour<=100){
			servoFour = true;}
		 else{std::cout<<"[ERROR] Target must be bewteen 0 and 100"<<std::endl;;}			
	 }
	 
	 
}

// START OF THE ROS NODE
int main(int argc, char **argv) {
 
  srcHals::servo_node2 node(argc, argv, "servo_node");

  return 0;
}

