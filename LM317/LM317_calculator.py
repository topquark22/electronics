#!/usr/bin/python3

# Determine resistor combinations R1, R2 for LM317 regulator
# to provide a desired output voltage
#
# Note: Use resistors ideally within 1K order of magnitude,
# otherwise if too high (10K+) the result may be unreliable
#
# Author: topquark22

import sys

if (2 != len(sys.argv)) :
    print(f"Usage: {sys.argv[0]} [target voltage]")
    sys.exit()

# Target voltage for LM317 regulator output, from command line
v_target = float(sys.argv[1])

# resistor values available (mantissa)
E12 = [1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2]
E12_plus = [1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.3, 4.7, 5.1, 5.6, 6.8, 8.2]
E12_ext = [1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.3, 4.7, 5.1, 5.6, 6.8, 8.2,
          10, 12, 15, 18, 20, 22, 27, 33, 39, 43, 47, 50, 51, 56, 68, 82]

# output of regulator depending on resistor values
def V(R1, R2) :
    return 1.25 * (1 + R2 / R1)

closest_value = float('inf')
closest_pair = (None, None)

for R1 in E12_plus:
    for R2 in E12_plus:
        v = V(R1, R2)
        if abs(v - v_target) < abs(closest_value - v_target) :
            closest_value = v
            closest_pair = (R1, R2)

R1 = round(closest_pair[0] * 100)
R2 = round(closest_pair[1] * 100)

print(f"Target voltage: {v_target}")
print(f"Best resistor values: R1={R1}, R2={R2}")
print(f"Expected voltage: {round(closest_value, 2)}")

if R1 < 120:
    print("Warning: R1 < 120")
