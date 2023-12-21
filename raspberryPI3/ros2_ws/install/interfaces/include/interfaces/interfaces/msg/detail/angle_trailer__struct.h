// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/AngleTrailer.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__ANGLE_TRAILER__STRUCT_H_
#define INTERFACES__MSG__DETAIL__ANGLE_TRAILER__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/AngleTrailer in the package interfaces.
typedef struct interfaces__msg__AngleTrailer
{
  float trailer_angle;
} interfaces__msg__AngleTrailer;

// Struct for a sequence of interfaces__msg__AngleTrailer.
typedef struct interfaces__msg__AngleTrailer__Sequence
{
  interfaces__msg__AngleTrailer * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__AngleTrailer__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__ANGLE_TRAILER__STRUCT_H_
