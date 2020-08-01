#!/usr/bin/env python3

import math


def getRatio(x:float):
    return math.pow(math.pow(2,1/12),x)

for i in range(-12,14):

    for cents in range(-2,3):
        idx = (i+12)*5+(cents+2)
        print("{v}, // {idx}".format(idx=idx, v=getRatio(i+cents/10.0)))


