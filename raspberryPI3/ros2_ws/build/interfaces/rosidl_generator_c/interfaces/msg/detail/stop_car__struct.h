// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/StopCar.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__STOP_CAR__STRUCT_H_
#define INTERFACES__MSG__DETAIL__STOP_CAR__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/StopCar in the package interfaces.
typedef struct interfaces__msg__StopCar
{
  /// TRUE for car to stop moving backward
  bool stop_car_rear;
  /// TRUE for car to stop moving forward
  bool stop_car_front;
} interfaces__msg__StopCar;

// Struct for a sequence of interfaces__msg__StopCar.
typedef struct interfaces__msg__StopCar__Sequence
{
  interfaces__msg__StopCar * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__StopCar__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__STOP_CAR__STRUCT_H_
