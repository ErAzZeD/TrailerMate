import rclpy
from rclpy.node import Node
import time
import requests

from interfaces.msg import Ultrasonic
from interfaces.msg import AngleTrailer
from interfaces.msg import MotorsFeedback
from interfaces.msg import Gnss
from interfaces.msg import GeneralData

class SendData (Node):

    def __init__(self):
        super().__init__('send_data_node')

        # Subscribers
        self.subscription_us = self.create_subscription(Ultrasonic, 'us_data', self.us_callback, 10)

        self.subscription_motors_feedback = self.create_subscription(MotorsFeedback, 'motors_feedback', self.motors_feedback_callback, 10)

        self.subscription_trailer_angle = self.create_subscription(AngleTrailer, 'trailer_angle', self.trailer_angle_callback, 10)

        self.subscription_gnss = self.create_subscription(Gnss, 'gnss', self.gnss_callback, 10)

        self.subscription_general_data = self.create_subscription(GeneralData, 'general_data', self.general_data_callback, 10)

        # Create a timer with a 100 ms period and bind it to the send_all_data method
        self.timer = self.create_timer(0.1, self.send_all_data)

        # Initialize variables
        self.us_front_left = 0
        self.us_front_center = 0
        self.us_front_right = 0
        self.us_back_left = 0
        self.us_back_center = 0
        self.us_back_right = 0
        self.trailer_angle = 0
        self.currentRightSpeed = 0
        self.latitude = 43.509548901418347
        self.longitude = 1.4662544183069595
        self.battery_level = 0

    def us_callback(self, us: Ultrasonic):
        # Construire les données à envoyer
        self.us_front_left = us.front_left
        self.us_front_center =  us.front_center
        self.us_front_right = us.front_right
        self.us_back_left = us.rear_left
        self.us_back_center =  us.rear_center
        self.us_back_right = us.rear_right

    def motors_feedback_callback(self, motorsFeedback: MotorsFeedback):
        self.currentRightSpeed = motorsFeedback.right_rear_speed

    def trailer_angle_callback(self, angle: AngleTrailer):
        self.trailer_angle = angle.trailer_angle
    
    def gnss_callback(self, gps: Gnss):
        self.latitude = gps.latitude
        self.longitude = gps.longitude
    
    def general_data_callback(self, gd: GeneralData):
        self.battery_level = gd.battery_level

    def send_all_data(self):

        api_url = "http://138.197.181.206/api/add/all"

        data = [
            {"us_front_left": self.us_front_left},
            {"us_front_center": self.us_front_center},
            {"us_front_right": self.us_front_right},
            {"us_back_left": self.us_back_left},
            {"us_back_center": self.us_back_center},
            {"us_back_right": self.us_back_right},
            {"trailer_angle": self.trailer_angle},
            {"speed": self.currentRightSpeed},
            {"latitude": self.latitude},
            {"longitude": self.longitude},
            {"battery_level": self.battery_level}
        ]

        base_api_url = "http://138.197.181.206/api/data/"

        for item in data:
            api_url = f"{base_api_url}{list(item.keys())[0]}"
            response = requests.put(api_url, json={list(item.keys())[0]: list(item.values())[0]})


def main(args=None):
    rclpy.init(args=args)
    send_data_node= SendData()
    rclpy.spin(send_data_node)
    send_data_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
