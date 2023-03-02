import rclpy
from rclpy.node import Node
import time
import requests

from interfaces.msg import AppliOrder
import json

class ReceiveData (Node):

    def __init__(self):
        super().__init__('receive_data_node')

        # Publishers
        self.publish_appli_order = self.create_publisher(AppliOrder, 'appli_order', 10)

        # Create a timer with a 100 ms period and bind it to the send_all_data method
        self.timer = self.create_timer(0.1, self.receive_all_data)

    def receive_all_data(self):

        order = AppliOrder()

        api_url = f"http://138.197.181.206/api/allData"
        response = requests.get(api_url)

        if response.status_code == 200:
            
            json_data = response.json()["data"]

            for item in json_data:
                if list(item.keys())[0] == "button_emergency":
                    order.button_emergency = list(item.values())[0]
                elif list(item.keys())[0] == "button_record":
                    order.button_record = list(item.values())[0]
                elif list(item.keys())[0] == "button_replay":
                    order.button_replay = list(item.values())[0]
                elif list(item.keys())[0] == "button_reverse":
                    order.button_reverse = list(item.values())[0]
                elif list(item.keys())[0] == "button_forward":
                    order.button_forward = list(item.values())[0]
                elif list(item.keys())[0] == "button_sensors":
                    order.button_sensors = list(item.values())[0]
            
            self.publish_appli_order.publish(order)
        else:
            print(response.status_code)


def main(args=None):
    rclpy.init(args=args)
    receive_data_node= ReceiveData()
    rclpy.spin(receive_data_node)
    receive_data_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
