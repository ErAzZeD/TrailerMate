// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/AngleTrailer.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__ANGLE_TRAILER__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__ANGLE_TRAILER__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "interfaces/msg/detail/angle_trailer__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_AngleTrailer_trailer_angle
{
public:
  Init_AngleTrailer_trailer_angle()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::msg::AngleTrailer trailer_angle(::interfaces::msg::AngleTrailer::_trailer_angle_type arg)
  {
    msg_.trailer_angle = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::AngleTrailer msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::AngleTrailer>()
{
  return interfaces::msg::builder::Init_AngleTrailer_trailer_angle();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__ANGLE_TRAILER__BUILDER_HPP_
