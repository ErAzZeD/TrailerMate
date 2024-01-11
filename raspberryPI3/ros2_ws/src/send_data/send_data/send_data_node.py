import rclpy
from rclpy.node import Node
import time
import requests

from interfaces.msg import Ultrasonic
from interfaces.msg import AngleTrailer
from interfaces.msg import MotorsFeedback

class SendData (Node):

    us_front_left, us_front_center, us_front_right, us_back_left, us_back_center, us_back_right, currentLeftSpeed, currentRightSpeed, trailer_angle

    def __init__(self):
        super().__init__('send_data_node')
        
        # Subscribers
        self.subscription_us = self.create_subscription(Ultrasonic, 'us_data', self.us_callback, 10)

        self.subscription_motors_feedback = self.create_subscription(MotorsFeedback, 'motors_feedback', self.motors_feedback_callback, 10)

        self.subscription_trailer_angle = self.create_subscription(AngleTrailer, 'trailer_angle', self.trailer_angle_callback, 10)


    def us_callback(self, us: Ultrasonic):
        # Construire les données à envoyer
        self.us_front_left = us.us_front_left
        self.us_front_center =  us.us_front_center
        self.us_front_right = us.us_front_right
        self.us_back_left = us.us_back_left
        self.us_back_center =  us.us_back_center
        self.us_back_right = us.us_back_right

        
     def motors_feedback_callback(self, motors_feedback: MotorsFeedback):
        #currentAngle = motorsFeedback.steering_angle
        #currentLeftSpeed = motorsFeedback.left_rear_speed
        self.currentRightSpeed = motorsFeedback.right_rear_speed

    def trailer_angle_callback(self, angle: AngleTrailer):
        self.trailer_angle = angle.trailer_angle

    def send_all_data(self):

        api_url = "http://138.197.181.206/api/add/all"

        while(1):
            data = [
                {"us_front_left": self.us_front_left},
                {"us_front_center": self.us_front_center},
                {"us_front_right": self.us_front_right},
                {"us_back_left": self.us_back_left},
                {"us_back_center": self.us_back_center},
                {"us_back_right": self.us_back_right},
                {"trailer_angle": self.trailer_angle},
                {"speed": self.currentRightSpeed}
            ]
            response = requests.put(api_url, json=data)
            time.sleep(100)
	   

def main(args=None):
    rclpy.init(args=args)
    send_data_node= SendData()
    rclpy.spin(send_data_node)
    send_data_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
