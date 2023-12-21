// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from interfaces:msg/StopCar.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "interfaces/msg/detail/stop_car__struct.h"
#include "interfaces/msg/detail/stop_car__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool interfaces__msg__stop_car__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[33];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("interfaces.msg._stop_car.StopCar", full_classname_dest, 32) == 0);
  }
  interfaces__msg__StopCar * ros_message = _ros_message;
  {  // stop_car_rear
    PyObject * field = PyObject_GetAttrString(_pymsg, "stop_car_rear");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->stop_car_rear = (Py_True == field);
    Py_DECREF(field);
  }
  {  // stop_car_front
    PyObject * field = PyObject_GetAttrString(_pymsg, "stop_car_front");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->stop_car_front = (Py_True == field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * interfaces__msg__stop_car__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of StopCar */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("interfaces.msg._stop_car");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "StopCar");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  interfaces__msg__StopCar * ros_message = (interfaces__msg__StopCar *)raw_ros_message;
  {  // stop_car_rear
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->stop_car_rear ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "stop_car_rear", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // stop_car_front
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->stop_car_front ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "stop_car_front", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
