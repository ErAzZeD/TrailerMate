// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from interfaces:msg/StopCar.idl
// generated code does not contain a copyright notice
#include "interfaces/msg/detail/stop_car__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
interfaces__msg__StopCar__init(interfaces__msg__StopCar * msg)
{
  if (!msg) {
    return false;
  }
  // stop_car_rear
  // stop_car_front
  return true;
}

void
interfaces__msg__StopCar__fini(interfaces__msg__StopCar * msg)
{
  if (!msg) {
    return;
  }
  // stop_car_rear
  // stop_car_front
}

bool
interfaces__msg__StopCar__are_equal(const interfaces__msg__StopCar * lhs, const interfaces__msg__StopCar * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // stop_car_rear
  if (lhs->stop_car_rear != rhs->stop_car_rear) {
    return false;
  }
  // stop_car_front
  if (lhs->stop_car_front != rhs->stop_car_front) {
    return false;
  }
  return true;
}

bool
interfaces__msg__StopCar__copy(
  const interfaces__msg__StopCar * input,
  interfaces__msg__StopCar * output)
{
  if (!input || !output) {
    return false;
  }
  // stop_car_rear
  output->stop_car_rear = input->stop_car_rear;
  // stop_car_front
  output->stop_car_front = input->stop_car_front;
  return true;
}

interfaces__msg__StopCar *
interfaces__msg__StopCar__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__StopCar * msg = (interfaces__msg__StopCar *)allocator.allocate(sizeof(interfaces__msg__StopCar), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(interfaces__msg__StopCar));
  bool success = interfaces__msg__StopCar__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
interfaces__msg__StopCar__destroy(interfaces__msg__StopCar * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    interfaces__msg__StopCar__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
interfaces__msg__StopCar__Sequence__init(interfaces__msg__StopCar__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__StopCar * data = NULL;

  if (size) {
    data = (interfaces__msg__StopCar *)allocator.zero_allocate(size, sizeof(interfaces__msg__StopCar), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = interfaces__msg__StopCar__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        interfaces__msg__StopCar__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
interfaces__msg__StopCar__Sequence__fini(interfaces__msg__StopCar__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      interfaces__msg__StopCar__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

interfaces__msg__StopCar__Sequence *
interfaces__msg__StopCar__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  interfaces__msg__StopCar__Sequence * array = (interfaces__msg__StopCar__Sequence *)allocator.allocate(sizeof(interfaces__msg__StopCar__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = interfaces__msg__StopCar__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
interfaces__msg__StopCar__Sequence__destroy(interfaces__msg__StopCar__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    interfaces__msg__StopCar__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
interfaces__msg__StopCar__Sequence__are_equal(const interfaces__msg__StopCar__Sequence * lhs, const interfaces__msg__StopCar__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!interfaces__msg__StopCar__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
interfaces__msg__StopCar__Sequence__copy(
  const interfaces__msg__StopCar__Sequence * input,
  interfaces__msg__StopCar__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(interfaces__msg__StopCar);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    interfaces__msg__StopCar * data =
      (interfaces__msg__StopCar *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!interfaces__msg__StopCar__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          interfaces__msg__StopCar__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!interfaces__msg__StopCar__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
