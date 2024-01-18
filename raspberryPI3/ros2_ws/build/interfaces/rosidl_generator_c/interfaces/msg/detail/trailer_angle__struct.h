// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/TrailerAngle.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__TRAILER_ANGLE__STRUCT_H_
#define INTERFACES__MSG__DETAIL__TRAILER_ANGLE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/TrailerAngle in the package interfaces.
typedef struct interfaces__msg__TrailerAngle
{
  float trailer_angle;
} interfaces__msg__TrailerAngle;

// Struct for a sequence of interfaces__msg__TrailerAngle.
typedef struct interfaces__msg__TrailerAngle__Sequence
{
  interfaces__msg__TrailerAngle * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__TrailerAngle__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__TRAILER_ANGLE__STRUCT_H_
