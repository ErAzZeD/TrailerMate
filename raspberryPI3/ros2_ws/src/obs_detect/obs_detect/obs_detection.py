import rclpy
from rclpy.node import Node

from interfaces.msg import Ultrasonic
from interfaces.msg import StopCar
from interfaces.msg import MotorsOrder


class ObstacleDetection(Node):


    def __init__(self):
        super().__init__('obs_detection')

        self.MINIMAL_DISTANCE = 50
        self.STOP = 50

        self.GO_FRONT = False
        self.GO_REAR = False
        self.STOPPED = False

        self.FRONT_OBSTACLE = False
        self.REAR_OBSTACLE = False

        # Publishers
        # publish informations to StopCar topic
        self.publish_stop_car = self.create_publisher(StopCar, 'stop_car', 10)
        
        # Subscribers
        self.subscription_us = self.create_subscription(Ultrasonic, 'us_data', self.us_callback, 10)

        self.subscription_motors_order = self.create_subscription(MotorsOrder, 'motors_order', self.motors_order_callback, 10)


    def motors_order_callback(self, motorsOrder: MotorsOrder):

        # Get car direction
        if motorsOrder.right_rear_pwm > self.STOP and motorsOrder.left_rear_pwm > self.STOP :
            self.GO_FRONT = True
            self.GO_REAR = False
            self.STOPPED = False 
        elif motorsOrder.right_rear_pwm < self.STOP and motorsOrder.left_rear_pwm < self.STOP :
            self.GO_FRONT = False
            self.GO_REAR = True
            self.STOPPED = False 
        else :
            self.GO_FRONT = False
            self.GO_REAR = False
            self.STOPPED = True 

        self.get_logger().info("Car stopped: " + str(self.STOPPED))
        self.get_logger().info("Car direction front: " + str(self.GO_FRONT))
        self.get_logger().info("Car direction rear: " + str(self.GO_REAR))

    def us_callback(self, us: Ultrasonic):

        stop = StopCar()
        
        # Get obstacle position
        if us.front_left < self.MINIMAL_DISTANCE or us.front_center < self.MINIMAL_DISTANCE or us.front_right < self.MINIMAL_DISTANCE :
            self.FRONT_OBSTACLE = True
        else :
            self.FRONT_OBSTACLE = False

        if us.rear_left < self.MINIMAL_DISTANCE or us.rear_center < self.MINIMAL_DISTANCE or us.rear_right < self.MINIMAL_DISTANCE :
            self.REAR_OBSTACLE = True
        else :
            self.REAR_OBSTACLE = False

        
        # Stop the car
        if self.FRONT_OBSTACLE :
            stop.stop_car_front = True
        elif self.REAR_OBSTACLE : 
            stop.stop_car_rear = True
        else :
            stop.stop_car_front = False
            stop.stop_car_rear = False
        

        # Publish stop_car topic
        self.publish_stop_car.publish(stop)


def main(args=None):
    rclpy.init(args=args)
    obs_detection = ObstacleDetection()
    rclpy.spin(obs_detection)
    obs_detection.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

