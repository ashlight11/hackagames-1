#!env python3
from ctypes import cdll, c_uint, cast, POINTER, c_double

core = cdll.LoadLibrary("./libtoghap.a")

def uintArrayType( size ):
    return c_uint * size

def uintArray( size, value=0 ):
    Array= c_uint * size
    cArray= Array()
    for i in range(size) :
        cArray[i]= (c_uint)( value )
    return cArray

def uintArrayAs( size, pythonLst ):
    Array= c_uint * size
    cArray= Array()
    for i in range(size) :
        cArray[i]= (c_uint)( pythonLst[i] )
    return cArray

def readUintLst( size, uintPointer ):
    pLst= cast(uintPointer, POINTER(c_uint))
    return [ (int)(pLst[i]) for i in range(size) ]

def readDouble(pDouble):
    return (float)(cast(pDouble, POINTER(c_double))[0])

