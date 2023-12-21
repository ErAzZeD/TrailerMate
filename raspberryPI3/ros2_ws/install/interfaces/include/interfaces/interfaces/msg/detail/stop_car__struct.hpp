// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from interfaces:msg/StopCar.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__STOP_CAR__STRUCT_HPP_
#define INTERFACES__MSG__DETAIL__STOP_CAR__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__interfaces__msg__StopCar __attribute__((deprecated))
#else
# define DEPRECATED__interfaces__msg__StopCar __declspec(deprecated)
#endif

namespace interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct StopCar_
{
  using Type = StopCar_<ContainerAllocator>;

  explicit StopCar_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->stop_car_rear = false;
      this->stop_car_front = false;
    }
  }

  explicit StopCar_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->stop_car_rear = false;
      this->stop_car_front = false;
    }
  }

  // field types and members
  using _stop_car_rear_type =
    bool;
  _stop_car_rear_type stop_car_rear;
  using _stop_car_front_type =
    bool;
  _stop_car_front_type stop_car_front;

  // setters for named parameter idiom
  Type & set__stop_car_rear(
    const bool & _arg)
  {
    this->stop_car_rear = _arg;
    return *this;
  }
  Type & set__stop_car_front(
    const bool & _arg)
  {
    this->stop_car_front = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    interfaces::msg::StopCar_<ContainerAllocator> *;
  using ConstRawPtr =
    const interfaces::msg::StopCar_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<interfaces::msg::StopCar_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<interfaces::msg::StopCar_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::StopCar_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::StopCar_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      interfaces::msg::StopCar_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<interfaces::msg::StopCar_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<interfaces::msg::StopCar_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<interfaces::msg::StopCar_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__interfaces__msg__StopCar
    std::shared_ptr<interfaces::msg::StopCar_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__interfaces__msg__StopCar
    std::shared_ptr<interfaces::msg::StopCar_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const StopCar_ & other) const
  {
    if (this->stop_car_rear != other.stop_car_rear) {
      return false;
    }
    if (this->stop_car_front != other.stop_car_front) {
      return false;
    }
    return true;
  }
  bool operator!=(const StopCar_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct StopCar_

// alias to use template instance with default allocator
using StopCar =
  interfaces::msg::StopCar_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__STOP_CAR__STRUCT_HPP_
