# LM317 resistor value calculator


## Overview

Trimpots are a pain, expensive, and take up 3 rows on your breadboard. Wouldn't it be nice to use fixed resistors instead? This script lets you do exactly that!

### LM317

The LM317 is a versatile adjustable voltage regulator.

![LM317 TO-220](LM317.jpg)

In its most basic configuration as a voltage regulator, the LM317 is wired like this:

![LM317 simple voltage regulator](LM317-vreg-circuit.jpg)

The output voltage is given by the formula: **V_out = 1.25 * (1 + R2 / R1)**

The minimum value of R1 is 120 ohms. A typical value found in many examples is 240 ohms. Note that R1 and R2 form a voltage divider that draws **V_out/(R1 + R2)** amperes, so you don't want them to be too small.

V_in must be at least 3 volts greater than V_out. There is no maximum input voltage; however, the difference between the input and output voltage must not exceed 40V. (There is a high-voltage version, the LM317HV, which allows a differential of up to 60V.)

## Software requirements

You must have Python 3.6 or greater.

## Script usage

The script is prepopulated with a list of common resistor values, known in the industry
as E12. You may wish to edit the list to include any resistor values that are available to you,
but be aware of cost and production feasibility considerations.

The command to run the script is:

`./lm317calculator` *target_voltage*

The output will be the values of the fixed resistors R1, R2 that should be used,along with the actual output voltage that will be produced.

![sample invocation](sample-run.png)

The resistor list in the script allows for accurate results up to V_out = 72. If you need higher voltage, edit the resistor list.
