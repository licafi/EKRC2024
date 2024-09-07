# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from depthcam/position3d.msg. Do not edit."""
import codecs
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class position3d(genpy.Message):
  _md5sum = "0c2f0543d8404bbb5cf654877be20b1f"
  _type = "depthcam/position3d"
  _has_header = False  # flag to mark the presence of a Header object
  _full_text = """int64 x3d
int64 y3d
int64 z3d"""
  __slots__ = ['x3d','y3d','z3d']
  _slot_types = ['int64','int64','int64']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       x3d,y3d,z3d

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(position3d, self).__init__(*args, **kwds)
      # message fields cannot be None, assign default values for those that are
      if self.x3d is None:
        self.x3d = 0
      if self.y3d is None:
        self.y3d = 0
      if self.z3d is None:
        self.z3d = 0
    else:
      self.x3d = 0
      self.y3d = 0
      self.z3d = 0

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
      buff.write(_get_struct_3q().pack(_x.x3d, _x.y3d, _x.z3d))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      end = 0
      _x = self
      start = end
      end += 24
      (_x.x3d, _x.y3d, _x.z3d,) = _get_struct_3q().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_get_struct_3q().pack(_x.x3d, _x.y3d, _x.z3d))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      end = 0
      _x = self
      start = end
      end += 24
      (_x.x3d, _x.y3d, _x.z3d,) = _get_struct_3q().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_3q = None
def _get_struct_3q():
    global _struct_3q
    if _struct_3q is None:
        _struct_3q = struct.Struct("<3q")
    return _struct_3q
