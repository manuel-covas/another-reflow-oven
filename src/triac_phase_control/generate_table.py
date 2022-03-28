from cmath import pi
import math

f = 0.5
power_levels = 100
percision = 0.005


def power(t):
    return 1 -2*f*t + math.sin(4*f*math.pi*t) / (2 * math.pi)


csv = "Power,TimeDelay\n"
t = 1

# No need for a propper approximation method, this is quick enough.

for p in range(101):
    
    p = p / 100
    tries = 1

    while abs(power(t) - p) > 0.0000001:
        t -= 0.0000001
        tries += 1

    
    print("Found t = " + str(t) + " (" + str(tries) + " tries) ")
    csv += str(p) + "," + str(t) + "\n"  

open("result.csv", "w").write(csv)