# SPDX-FileCopyrightText: 2021-2022 Niels Moseley <asicsforthemasses@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-only

## create floorplan
createRegion("core", 10000, 10000, 170000, 30000)
createRows("core", 0, 10000, 2)

setTopModule("adder8")

## place the pins
xpos = 0
xinc = 10000
x = 10000

for idx in range(0,8): 
    pinName = "a[" + str(idx) + "]"
    placeInstance(pinName, "adder8", x, 30000)
    x = x + xinc
    pinName = "b[" + str(idx) + "]"
    placeInstance(pinName, "adder8", x, 30000)
    x = x + xinc    

xinc = 20000
x = 10000

for idx in range(0,8):
    pinName = "y[" + str(idx) + "]"
    placeInstance(pinName, "adder8", x, 10000)
    x = x + xinc

placeInstance("carryout", "adder8", x, 10000)
