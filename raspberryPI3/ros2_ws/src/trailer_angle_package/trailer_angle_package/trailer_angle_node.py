import rclpy
import serial
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

        type_export_topic = AngleTrailer()


        baudrate = 115200
        serial_addr = "/dev/ttyACM0"
        serial_connection = serial.Serial(port=serial_addr, baudrate=baudrate)
        
        while(True):
            size = serial_connection.inWaiting()
            if size:
                data = serial_connection.read(size)
                type_export_topic.trailer_angle = float(ord(data) - 90)
                self.publish_trailer_angle.publish(type_export_topic)        

def main(args=None):
    rclpy.init(args=args)
    trailer_angle_node = TrailerAngle()
    rclpy.spin(trailer_angle_node)
    trailer_angle_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
