import rclpy
from rclpy.node import Node

from interfaces.msg import Ultrasonic
from interfaces.msg import ObstacleDetection
from interfaces.msg import MotorsOrder


class ObsDetection(Node):

    MINIMAL_DISTANCE = 50

    def __init__(self):
        super().__init__('obs_detection')

        # Publishers
        # publish informations to ObstacleDetection topic
        self.publish_obstacle_detection = self.create_publisher(ObstacleDetection, 'obstacle_detection', 10)

        # Subscribers
        self.subscription_us = self.create_subscription(Ultrasonic, 'us_data', self.us_callback, 10)


    def us_callback(self, us: Ultrasonic):

        obstacle = ObstacleDetection()

        # Get obstacle position
        if us.front_left < self.MINIMAL_DISTANCE :
            obstacle.obstacle_detected_front_left = True
        elif us.front_center < self.MINIMAL_DISTANCE :
            obstacle.obstacle_detected_front_center = True
        elif us.front_right < self.MINIMAL_DISTANCE :
            obstacle.obstacle_detected_front_right = True
        else :
            obstacle.obstacle_detected_front_left = False
            obstacle.obstacle_detected_front_center = False
            obstacle.obstacle_detected_front_right = False

        if us.rear_left < self.MINIMAL_DISTANCE :
            obstacle.obstacle_detected_rear_left = True
        elif us.rear_center < self.MINIMAL_DISTANCE :
            obstacle.obstacle_detected_rear_center = True
        elif us.rear_right < self.MINIMAL_DISTANCE :
            obstacle.obstacle_detected_rear_right = True
        else :
            obstacle.obstacle_detected_rear_left = False
            obstacle.obstacle_detected_rear_center = False
            obstacle.obstacle_detected_rear_right = False

        if obstacle.obstacle_detected_rear_left or obstacle.obstacle_detected_rear_center or obstacle.obstacle_detected_rear_right :
            obstacle.obstacle_detected_rear = True
        else :
            obstacle.obstacle_detected_rear = False
        
        if obstacle.obstacle_detected_front_left or obstacle.obstacle_detected_front_center or obstacle.obstacle_detected_front_right :
            obstacle.obstacle_detected_front = True
        else :
            obstacle.obstacle_detected_front = False

        self.publish_obstacle_detection.publish(obstacle)


def main(args=None):
    rclpy.init(args=args)
    obs_detection = ObsDetection()
    rclpy.spin(obs_detection)
    obs_detection.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
