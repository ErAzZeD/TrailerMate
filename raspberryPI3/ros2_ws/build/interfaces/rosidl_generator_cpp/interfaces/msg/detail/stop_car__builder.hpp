// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/StopCar.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__STOP_CAR__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__STOP_CAR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "interfaces/msg/detail/stop_car__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_StopCar_stop_car_front
{
public:
  explicit Init_StopCar_stop_car_front(::interfaces::msg::StopCar & msg)
  : msg_(msg)
  {}
  ::interfaces::msg::StopCar stop_car_front(::interfaces::msg::StopCar::_stop_car_front_type arg)
  {
    msg_.stop_car_front = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::StopCar msg_;
};

class Init_StopCar_stop_car_rear
{
public:
  Init_StopCar_stop_car_rear()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StopCar_stop_car_front stop_car_rear(::interfaces::msg::StopCar::_stop_car_rear_type arg)
  {
    msg_.stop_car_rear = std::move(arg);
    return Init_StopCar_stop_car_front(msg_);
  }

private:
  ::interfaces::msg::StopCar msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::StopCar>()
{
  return interfaces::msg::builder::Init_StopCar_stop_car_rear();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__STOP_CAR__BUILDER_HPP_
