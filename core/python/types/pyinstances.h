// SPDX-FileCopyrightText: 2021-2022 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include <Python.h>
#include "celllib/pin.h"

extern PyTypeObject PyInstancesType;

namespace Python
{
    PyObject* toPythonAsInstances(std::shared_ptr<ChipDB::Netlist> netlistPtr);
};
