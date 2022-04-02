/*  LunaPnR Source Code
 
    SPDX-License-Identifier: GPL-3.0-only
    SPDX-FileCopyrightText: 2022 Niels Moseley <asicsforthemasses@gmail.com>
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <memory>
#include <array>
#include <iostream>

#include "../converters.h"
#include "typetemplate.h"
#include "pyinstance.h"
#include "pypininfolist.h"

/** container for LunaCore::Cell */
struct PyInstance : public Python::TypeTemplate<ChipDB::InstanceBase>
{
    static PyObject* getName(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->name());
        }
        
        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");
        return nullptr;
    };

    static PyObject* getArchetype(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->getArchetypeName());
        }

        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");
        return nullptr;
    }

    static PyObject* getPosition(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_pos);
        }

        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");
        return nullptr;
    }

    static int setPosition(PyInstance *self, PyObject *value, void *closure)
    {
        if (self->ok())
        {
            if (!PyArg_ParseTuple(value, "LL", &self->obj()->m_pos.m_x, &self->obj()->m_pos.m_y))
            {
                return -1;
            }
            return 0; // success
        }

        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");
        return -1;
    }

    static PyObject* getPlacementInfo(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(ChipDB::toString(self->obj()->m_placementInfo));
        }

        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");
        return nullptr;
    }

    static PyObject* getOrientation(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(ChipDB::toString(self->obj()->m_orientation));
        }

        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");
        return nullptr;
    }

    static PyObject* getSize(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->instanceSize());
        }
        
        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");        
        return nullptr;        
    };

    static PyObject* getArea(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->getArea());
        }
        
        PyErr_Format(PyExc_RuntimeError, "Self is uninitialized");        
        return nullptr;        
    };


#if 0
    static PyObject* getLeakagePower(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_leakagePower);
        }
        
        return nullptr;        
    };

    static PyObject* getArea(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_area);
        }
        
        return nullptr;        
    };


    static PyObject* getOffset(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_offset);
        }
        
        return nullptr;        
    };

    static PyObject* getSite(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_site);
        }
        
        return nullptr;        
    };

    static PyObject* getClass(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_class);
        }
        
        return nullptr;        
    };

    static PyObject* getSubClass(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_subclass);
        }
        
        return nullptr;        
    };

    static PyObject* getSymmetry(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            return Python::toPython(self->obj()->m_symmetry);
        }
        
        return nullptr;
    };


    static PyObject* getPins(PyInstance *self, void *closure)
    {
        if (self->ok())
        {
            auto pinInfoList = Python::toPython( &(self->obj()->m_pins));
            return pinInfoList;
        }
        
        return nullptr;
    }
#endif

    /** set internal values of PyInstance */
    static int pyInit(PyInstance *self, PyObject *args, PyObject *kwds)
    {
        return 0;   /* success */
    };

    /** set internal values of PyInstance */
    static PyObject* pyStr(PyObject *self)
    {
        //std::cout << "pyStr\n";
        return Python::toPython(PyInstance::PythonObjectName);
    };

    static constexpr const char *PythonObjectName = "Instance";
    static constexpr const char *PythonObjectDoc  = "Instance object";
};

// cppcheck-suppress "suppressed_error_id"
static PyMemberDef PyInstanceMembers[] =    // NOLINT(modernize-avoid-c-arrays)
{/*
    {"first", T_OBJECT_EX, offsetof(Noddy, first), nullptr,
    "first name"},
    {"last", T_OBJECT_EX, offsetof(Noddy, last), nullptr,
    "last name"},
    {"number", T_INT, offsetof(Noddy, number), nullptr,
    "noddy number"},
*/
    {nullptr}  /* Sentinel */
};

static PyGetSetDef PyInstanceGetSet[] =     // NOLINT(modernize-avoid-c-arrays)
{
    {"name", (getter)PyInstance::getName, nullptr, "instance name", nullptr /* closure */},
    {"archetype", (getter)PyInstance::getArchetype, nullptr, "archetype name", nullptr /* closure */},
    {"position", (getter)PyInstance::getPosition, (setter)PyInstance::setPosition, "lower left position in nm", nullptr /* closure */},
    {"pos", (getter)PyInstance::getPosition, (setter)PyInstance::setPosition, "lower left position in nm", nullptr /* closure */},
    {"placementInfo", (getter)PyInstance::getPlacementInfo, nullptr, "placement status", nullptr /* closure */},
    {"orientation", (getter)PyInstance::getOrientation, nullptr, "orientation of instance", nullptr /* closure */},
    {"size", (getter)PyInstance::getSize, nullptr, "size in nm", nullptr /* closure */},
    {"area", (getter)PyInstance::getArea, nullptr, "area in um^2", nullptr /* closure */},
    {nullptr}
};

static PyMethodDef PyInstanceMethods[] =    // NOLINT(modernize-avoid-c-arrays)
{
//    {"name", (PyCFunction)PyInstance::name, METH_NOARGS, "Return the cell name"},
    {nullptr}  /* Sentinel */
};

PyTypeObject PyInstanceType = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    PyInstance::PythonObjectName,       /* tp_name */
    sizeof(PyInstance),                 /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyInstance::pyDeAlloc,  /* tp_dealloc */
    0,                              /* tp_print */
    nullptr,                        /* tp_getattr */
    nullptr,                        /* tp_setattr */
    nullptr,                        /* tp_reserved */
    nullptr,                        /* tp_repr */
    nullptr,                        /* tp_as_number */
    nullptr,                        /* tp_as_sequence */
    nullptr,                        /* tp_as_mapping */
    nullptr,                        /* tp_hash  */
    nullptr,                        /* tp_call */
    PyInstance::pyStr,                  /* tp_str */
    nullptr,                        /* tp_getattro */
    nullptr,                        /* tp_setattro */
    nullptr,                        /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,        /* tp_flags */
    PyInstance::PythonObjectDoc,        /* tp_doc */
    nullptr,                        /* tp_traverse */
    nullptr,                        /* tp_clear */
    nullptr,                        /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    nullptr,                        /* tp_iter */
    nullptr,                        /* tp_iternext */
    PyInstanceMethods,                  /* tp_methods */
    PyInstanceMembers,                  /* tp_members */
    PyInstanceGetSet,                   /* tp_getset */
    nullptr,                        /* tp_base */
    nullptr,                        /* tp_dict */
    nullptr,                        /* tp_descr_get */
    nullptr,                        /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyInstance::pyInit,       /* tp_init */
    nullptr,                        /* tp_alloc */
    PyInstance::pyNewCall
};

PyObject* Python::toPython(std::shared_ptr<ChipDB::InstanceBase> instancePtr)
{
    // create a new PyInstance oject
    auto instanceObject = reinterpret_cast<PyInstance*>(PyObject_CallObject((PyObject*)&PyInstanceType, nullptr));
    if (instanceObject->m_holder != nullptr)
    {
        *instanceObject->m_holder = instancePtr;
        return (PyObject*)instanceObject;
    }
    return nullptr;
};
