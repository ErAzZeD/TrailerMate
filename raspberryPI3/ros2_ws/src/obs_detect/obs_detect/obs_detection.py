import rclpy
from rclpy.node import Node

from interfaces.msg import Ultrasonic
from interfaces.msg import StopCar
from interfaces.msg import Direction
from interfaces.msg import MotorsOrder


class ObstacleDetection(Node):

    MINIMAL_DISTANCE = 50
    STOP = 50

    def __init__(self):
        super().__init__('obs_detection')


        # Publishers
        # publish informations to StopCar topic
        self.publish_stop_car = self.create_publisher(StopCar, 'stop_car', 10)
        self.publish_direction = self.create_publisher(Direction, 'direction', 10)

        # Subscribers
        self.subscription_motors_order = self.create_subscription(MotorsOrder, 'motors_order', self.motors_order_callback, 10)
        self.subscription_us = self.create_subscription(Ultrasonic, 'us_data', self.us_callback, 10)



    def motors_order_callback(self, motorsOrder: MotorsOrder):

        car_direction = Direction()

        # Get car direction
        if motorsOrder.right_rear_pwm > self.STOP and motorsOrder.left_rear_pwm > self.STOP :
            car_direction.car_direction_rear = False
            car_direction.car_direction_front = True
            car_direction.car_direction_stop = False  
            
        elif motorsOrder.right_rear_pwm < self.STOP and motorsOrder.left_rear_pwm < self.STOP :
            car_direction.car_direction_rear = True
            car_direction.car_direction_front = False
            car_direction.car_direction_stop = False 

        else :
            car_direction.car_direction_rear = False
            car_direction.car_direction_front = False
            car_direction.car_direction_stop = True

        self.publish_direction.publish(car_direction)

    def us_callback(self, us: Ultrasonic):

        stop = StopCar()

        # Get obstacle position
        if us.front_left < self.MINIMAL_DISTANCE or us.front_center < self.MINIMAL_DISTANCE or us.front_right < self.MINIMAL_DISTANCE :
            stop.stop_car_front = True
        else :
            stop.stop_car_front = False

        if us.rear_left < self.MINIMAL_DISTANCE or us.rear_center < self.MINIMAL_DISTANCE or us.rear_right < self.MINIMAL_DISTANCE :
            stop.stop_car_rear = True
        else :
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

