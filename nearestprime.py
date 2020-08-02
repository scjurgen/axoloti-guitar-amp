#!/usr/bin/env python3

import sys
import math


# very primitve prime test, however good enough
def isPrime(value: int) -> bool:
    if value > 1:
        for i in range(2, (int)(value / 2) + 1):
            if (value % i) == 0:
                return False
        return True
    return False


def nearestPrime(value: int, step: int) -> int:
    if step == 2:
        if (value % 2) == 0:
            value += 1
    else:
        if (value % 2) == 0:
            value -= 1
    while not isPrime(value):
        value += step
    return value


def noteToFreq(note: float) -> float:
    return 440.0 * math.pow(math.pow(2.0, 1.0 / 12.0), note - 69.0)


def allPassBufferSize(f: float, samplerate: float) -> float:
    return round(samplerate / f)


if len(sys.argv) != 2:
    print("Usage: {} samplerate".format(sys.argv[0]))
    print("    will create a table containing buffersize for resonance and prime corrected buffersizes")
    print("    the table starts with a low C at 8.1758Hz")
    print("    use awk to filter table. e.g.: ")
    print("    ./nearestprime.py 48000 | awk '{printf ($4 \",\") }'")
    sys.exit(-1)

samplerate = float(sys.argv[1])

def noteName(i:int) -> str:
    notenameList = ['C','Db','D','Eb','E','F','F#','G','Ab','A','Bb','B']
    return "{}{}".format(notenameList[i%12], int(i/12)-1)



for i in range(0, 128):
    f = noteToFreq(i)
    buffersize = allPassBufferSize(f, samplerate)
    if not isPrime(buffersize):
        nearUp = nearestPrime(buffersize, 2)
        deltaUp = nearUp - buffersize
        nearDown = nearestPrime(buffersize, -2)
        deltaDown = buffersize - nearDown
        if deltaDown < deltaUp:
            primebuffersize = nearDown
        else:
            primebuffersize = nearUp
    else:
        primebuffersize = buffersize
    fTarget = samplerate / primebuffersize
    cents = (12 * math.log(fTarget / 440.0) / math.log(2.0) + 69.0) * 100.0
    centDifference = round(cents - i * 100.0)
    print("{}\t{}\t{}\t{}\t{}\t{}".format(i, f, buffersize, primebuffersize, centDifference, noteName(i)))
