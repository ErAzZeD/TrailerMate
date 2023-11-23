import rclpy
from rclpy.node import Node

from interfaces.msg import Ultrasonic
from interfaces.msg import StopCar
from interfaces.msg import MotorsOrder


class ObstacleDetection(Node):
    MINIMAL_DISTANCE = 50

    GO_FRONT = False
    GO_REAR = False
    STOPPED = False

    FRONT_OBSTACLE = False
    REAR_OBSTACLE = False


    def __init__(self):
        super().__init__('obs_detection')

        # Publishers
        # publish informations to StopCar topic
        self.publish_stop_car = self.create_publisher(StopCar, 'stop_car', 10)
        
        # Subscribers
        self.subscription_us = self.create_subscription(Ultrasonic, 'us_data', self.us_callback, 10)

        self.subscription_motors_order = self.create_subscription(MotorsOrder, 'motors_order', self.motors_order_callback, 10)


    def motors_order_callback(self, motorsOrder: MotorsOrder):

        if motorsOrder.right_rear_pwm < 50 and motorsOrder.left_rear_pwm < 50 :
            self.GO_FRONT = True
        elif motorsOrder.right_rear_pwm > 50 and motorsOrder.left_rear_pwm > 50 :
            self.GO_REAR = True
        else :
            self.STOPPED = True


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

        
        # Get car direction
        if self.GO_FRONT and self.FRONT_OBSTACLE :
            stop.stop_car_front = True
        elif self.GO_REAR and REAR_OBSTACLE : 
            stop.stop_car_rear = True
        else :
            stop.stop_car_front = False
            stop.stop_car_rear = False
        

        # Publish stop_car topic
        self.publish_stop_car.publish(stop)

   """  def obstacle_detection(self):
        
        stop = StopCar()
        
        if self.GO_FRONT and self.FRONT_OBSTACLE :
            stop.stop_car_front = True
        elif self.GO_REAR and REAR_OBSTACLE : 
            stop.stop_car_rear = True
        else :
            stop.stop_car_front = False
            stop.stop_car_rear = False
        
        self.publish_stop_car.publish(stop) """

def main(args=None):
    rclpy.init(args=args)
    obs_detection = ObstacleDetection()
    rclpy.spin(obs_detection)
    obs_detection.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

