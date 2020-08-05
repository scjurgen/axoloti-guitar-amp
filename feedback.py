#!/usr/bin/env python3


import math

def magResponse(alpha: float) -> float:
    a = 1+alpha*alpha
    b = 2*alpha * 1 # 1=cos(omega K);
    return 1/math.sqrt(a-b)


#decay -60dB, impulse 1
# 10^(-60db/20) 10^-3 = 0.001
def decaytime(alpha: float, f:float) -> float:
    pulse = 1.0
    mag = magResponse(alpha)
    for c in range(1000000000):
        if pulse < 0.001:
            return c
        pulse*=alpha
    return pulse

def feedUp(alpha: float) -> float:
    pulse = 1.0
    for c in range(1000000):

        pulse=1.0+pulse*alpha
    return pulse


for i in [0.0, 0.1,0.2,0.3,0.4,0.5,0.6,0.7, 0.75,0.8,0.85,0.9,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.99,0.999,0.9999,0.99999,0.999999]:
    print("{:20}: {:20}  {:20} {:20}".format(i, magResponse(i), decaytime(i,1), feedUp(i)))

