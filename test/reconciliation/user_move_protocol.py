
from ctypes import Structure, c_int32


class UserMoveProtocol(Structure):
    _pack_ = 1
    _fields_ = [("op", c_int32), ("id", c_int32), ("seq", c_int32), ("pos", c_int32)]
