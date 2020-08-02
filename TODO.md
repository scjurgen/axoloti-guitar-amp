
# Ideas to develop

Distortion:

- precalc atan table (15bit=65k), we have the memory, use it
- use signal lowest bit for dither on result:
  out=atanTbl[__SSAT(in>>14,15)+32768]+in & 1)<<14;

use linkwitz riley crossover for splitting low high (Q=0.5, recheck coefficients)



