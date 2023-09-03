#!/usr/bin/python3

# Format resistor ohms
#
# Author: topquark22

def res_format(ohms) :
    if ohms <= 0 :
        return "invalid"
    elif ohms <= 10 :
        return f"{ohms}"
    elif ohms < 1000 :
        return f"{int(ohms)}"
    elif ohms < 1000000 :
        return f"{round(ohms / 1000.0, 1)}K"
    else :
        return f"{round(ohms / 1000000.0, 1)}M"

import sys

if (2 != len(sys.argv)) :
    print(f"Usage: {sys.argv[0]} [ohms]")
    sys.exit()

ohms = float(sys.argv[1])

print(f"{res_format(ohms)}")