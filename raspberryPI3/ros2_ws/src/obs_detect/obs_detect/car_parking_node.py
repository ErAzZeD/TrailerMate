
import rclpy
from rclpy.node import Node
from interfaces.msg import Ultrasonic
from interfaces.msg import ObstacleDetection
from interfaces.msg import MotorsOrder, JoystickOrder

class parking(Node):
    
    MINIMAL_DISTANCE = 50
    def __init__(self):
        super().__init__('filerecorder')

        self.subscription_motors_order = self.create_subscription(
            MotorsOrder,
            'motors_order',
            self.motors_order_callback,
            10
        )

        self.subscription_joystick_order = self.create_subscription(
            JoystickOrder,
            'joystick_order',
            self.joystick_order_callback,
            10
        )

        self.file_path = '/home/pi/motors_order_values.txt'
        self.file_handle = None
        self.is_recording = False
        
        # publish informations to ObstacleDetection topic
        self.publish_obstacle_detection = self.create_publisher(ObstacleDetection, 'obstacle_detection', 10)

        # Subscribers
        self.subscription_us = self.create_subscription(Ultrasonic, 'us_data', self.us_callback, 10)



    def motors_order_callback(self, motors_order: MotorsOrder):
        right_rear_pwm = motors_order.right_rear_pwm
        left_rear_pwm = motors_order.left_rear_pwm
        steering_pwm = motors_order.steering_pwm

        if self.is_recording:
            # Enregistre les données dans le fichier texte
            self.file_handle.write('%d %d %d\n' %
                (right_rear_pwm, left_rear_pwm, steering_pwm)
            )
    
    def joystick_order_callback(self, joystick_order: JoystickOrder):
        record = joystick_order.record

        if record:
            # Démarre un nouvel enregistrement
            if not self.is_recording:
                # Ferme le fichier précédent s'il était ouvert
                if self.file_handle:
                    self.file_handle.close()
                # Ouvre un nouveau fichier pour l'enregistrement
                self.file_handle = open(self.file_path, 'w')
                self.is_recording = True
                self.get_logger().info('Recording started.')
        else:
            # Arrête l'enregistrement en cours
            if self.is_recording:
                self.file_handle.close()
                self.is_recording = False
                self.get_logger().info('Recording stopped.')

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
    filerecorder = parking()

    try:
        while rclpy.ok():  # Continue tant que le système ROS 2 est actif
            rclpy.spin_once(filerecorder, timeout_sec=1)

    except KeyboardInterrupt:
        pass

    finally:
        if filerecorder.is_recording:
            filerecorder.file_handle.close()
        filerecorder.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
