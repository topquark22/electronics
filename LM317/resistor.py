# Format resistor ohms
#
# Author: topquark22

def res_format(ohms, decimals=1):
    if ohms <= 0:
        return "invalid"
    elif ohms < 1000:
        return f"{round(ohms, decimals)}"
    elif ohms < 1000000:
        return f"{round(ohms / 1000.0, decimals)}K"
    else :
        return f"{round(ohms / 1000000.0, decimals)}M"
