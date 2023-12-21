// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from interfaces:msg/AngleTrailer.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "interfaces/msg/detail/angle_trailer__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void AngleTrailer_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) interfaces::msg::AngleTrailer(_init);
}

void AngleTrailer_fini_function(void * message_memory)
{
  auto typed_message = static_cast<interfaces::msg::AngleTrailer *>(message_memory);
  typed_message->~AngleTrailer();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember AngleTrailer_message_member_array[1] = {
  {
    "trailer_angle",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(interfaces::msg::AngleTrailer, trailer_angle),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers AngleTrailer_message_members = {
  "interfaces::msg",  // message namespace
  "AngleTrailer",  // message name
  1,  // number of fields
  sizeof(interfaces::msg::AngleTrailer),
  AngleTrailer_message_member_array,  // message members
  AngleTrailer_init_function,  // function to initialize message memory (memory has to be allocated)
  AngleTrailer_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t AngleTrailer_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &AngleTrailer_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<interfaces::msg::AngleTrailer>()
{
  return &::interfaces::msg::rosidl_typesupport_introspection_cpp::AngleTrailer_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, interfaces, msg, AngleTrailer)() {
  return &::interfaces::msg::rosidl_typesupport_introspection_cpp::AngleTrailer_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
