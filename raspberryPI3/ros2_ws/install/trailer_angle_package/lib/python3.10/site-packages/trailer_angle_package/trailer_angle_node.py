import rclpy
from rclpy.node import Node

from interfaces.msg import AngleTrailer



class TrailerAngle(Node):
    
    ANGLE_ZERO = 0 # Value for which zero is set
    
    
    def __init__(self):
        super().__init__('trailer_angle_node')

        # Publishers
        # publish informations to TrailerAngleDegree topic
        self.publish_trailer_angle = self.create_publisher(AngleTrailer, 'trailer_angle', 10)
        print("OKkkk")
        self.call_node()

        # Subscribers
        # self.subscription_us = self.create_subscription(PotentiometerData, 'us_data', self.us_callback, 10)

    def call_node(self):
        # TODO : Récupérer valeur du potentiomètre via ADC
        PotentiometerData = 100
        if PotentiometerData == self.ANGLE_ZERO:
            TrailerAngleDegree = 0
        elif PotentiometerData > self.ANGLE_ZERO: #la remorque est à droite
            TrailerAngleDegree = 100
        elif PotentiometerData < self.ANGLE_ZERO: #la remorque est à gauche
            TrailerAngleDegree = 200

        self.publish_trailer_angle.publish()
        print("The code has compiled successfully")


def main(args=None):
    rclpy.init(args=args)
    trailer_angle_node = TrailerAngle()
    rclpy.spin(trailer_angle_node)
    trailer_angle_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
