# Format resistor ohms
#
# Author: topquark22

def res_format(ohms):
    if ohms <= 0:
        return "invalid"
    elif ohms < 1000:
        return f"{ohms}"
    elif ohms < 1000000:
        return f"{round(ohms / 1000.0, 1)}K"
    else :
        return f"{round(ohms / 1000000.0)}M"
