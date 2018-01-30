# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.8
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_NeuroMem', [dirname(__file__)])
        except ImportError:
            import _NeuroMem
            return _NeuroMem
        if fp is not None:
            try:
                _mod = imp.load_module('_NeuroMem', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _NeuroMem = swig_import_helper()
    del swig_import_helper
else:
    import _NeuroMem
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        if _newclass:
            object.__setattr__(self, name, value)
        else:
            self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



def GetNetworkInfo(neuronAvailable, neuronMemorySize):
    return _NeuroMem.GetNetworkInfo(neuronAvailable, neuronMemorySize)
GetNetworkInfo = _NeuroMem.GetNetworkInfo

def Forget(*args):
    return _NeuroMem.Forget(*args)
Forget = _NeuroMem.Forget

def ClearNeurons():
    return _NeuroMem.ClearNeurons()
ClearNeurons = _NeuroMem.ClearNeurons

def CountNeuronsAvailable():
    return _NeuroMem.CountNeuronsAvailable()
CountNeuronsAvailable = _NeuroMem.CountNeuronsAvailable

def setContext(context, minif, maxif):
    return _NeuroMem.setContext(context, minif, maxif)
setContext = _NeuroMem.setContext

def getContext(context, minif, maxif):
    return _NeuroMem.getContext(context, minif, maxif)
getContext = _NeuroMem.getContext

def setRBF():
    return _NeuroMem.setRBF()
setRBF = _NeuroMem.setRBF

def setKNN():
    return _NeuroMem.setKNN()
setKNN = _NeuroMem.setKNN

def Broadcast(vector, length):
    return _NeuroMem.Broadcast(vector, length)
Broadcast = _NeuroMem.Broadcast

def Learn(vector, length, category):
    return _NeuroMem.Learn(vector, length, category)
Learn = _NeuroMem.Learn

def BestMatch(vector, length, distance, category, nid):
    return _NeuroMem.BestMatch(vector, length, distance, category, nid)
BestMatch = _NeuroMem.BestMatch

def Recognize(vector, length, K, distance, category, nid):
    return _NeuroMem.Recognize(vector, length, K, distance, category, nid)
Recognize = _NeuroMem.Recognize

def ReadNeurons(neurons):
    return _NeuroMem.ReadNeurons(neurons)
ReadNeurons = _NeuroMem.ReadNeurons

def WriteNeurons(neurons, ncount):
    return _NeuroMem.WriteNeurons(neurons, ncount)
WriteNeurons = _NeuroMem.WriteNeurons

def ReadNeuron(*args):
    return _NeuroMem.ReadNeuron(*args)
ReadNeuron = _NeuroMem.ReadNeuron
# This file is compatible with both classic and new-style classes.


