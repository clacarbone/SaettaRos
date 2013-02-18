"""autogenerated by genpy from Saetta_Messages/saetta_odom.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class saetta_odom(genpy.Message):
  _md5sum = "aa394c33a3c739445971620289a118d3"
  _type = "Saetta_Messages/saetta_odom"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """float32 x_position
float32 y_position
float32 angle

"""
  __slots__ = ['x_position','y_position','angle']
  _slot_types = ['float32','float32','float32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       x_position,y_position,angle

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(saetta_odom, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.x_position is None:
        self.x_position = 0.
      if self.y_position is None:
        self.y_position = 0.
      if self.angle is None:
        self.angle = 0.
    else:
      self.x_position = 0.
      self.y_position = 0.
      self.angle = 0.

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_3f.pack(_x.x_position, _x.y_position, _x.angle))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 12
      (_x.x_position, _x.y_position, _x.angle,) = _struct_3f.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_3f.pack(_x.x_position, _x.y_position, _x.angle))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 12
      (_x.x_position, _x.y_position, _x.angle,) = _struct_3f.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_3f = struct.Struct("<3f")
