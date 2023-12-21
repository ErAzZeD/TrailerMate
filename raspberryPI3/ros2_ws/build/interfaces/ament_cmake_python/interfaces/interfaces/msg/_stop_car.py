# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:msg/StopCar.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_StopCar(type):
    """Metaclass of message 'StopCar'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'interfaces.msg.StopCar')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__stop_car
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__stop_car
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__stop_car
            cls._TYPE_SUPPORT = module.type_support_msg__msg__stop_car
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__stop_car

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class StopCar(metaclass=Metaclass_StopCar):
    """Message class 'StopCar'."""

    __slots__ = [
        '_stop_car_rear',
        '_stop_car_front',
    ]

    _fields_and_field_types = {
        'stop_car_rear': 'boolean',
        'stop_car_front': 'boolean',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.stop_car_rear = kwargs.get('stop_car_rear', bool())
        self.stop_car_front = kwargs.get('stop_car_front', bool())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.stop_car_rear != other.stop_car_rear:
            return False
        if self.stop_car_front != other.stop_car_front:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def stop_car_rear(self):
        """Message field 'stop_car_rear'."""
        return self._stop_car_rear

    @stop_car_rear.setter
    def stop_car_rear(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'stop_car_rear' field must be of type 'bool'"
        self._stop_car_rear = value

    @builtins.property
    def stop_car_front(self):
        """Message field 'stop_car_front'."""
        return self._stop_car_front

    @stop_car_front.setter
    def stop_car_front(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'stop_car_front' field must be of type 'bool'"
        self._stop_car_front = value
