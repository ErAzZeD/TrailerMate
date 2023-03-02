
import rclpy
from rclpy.node import Node


from interfaces.msg import MotorsOrder, JoystickOrder

class parking(Node):

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
