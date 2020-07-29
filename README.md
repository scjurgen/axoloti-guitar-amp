# axoloti-guitar-amp
amp simulator for Axoloti (vs 2.0.0)

# Synopsis

This is a POC for an amp simulator with  Axoloti (STM32F427 based platform)[http://www.axoloti.com/]

It implements the following components:

- Overdrive: 3 channel frequency split with dc bias, gain and level for every level
- Amp: very simple implementation with 10 biquads of a deluxe-verb amp
- Stereo plate reverb: a version of Griesinger topology with some additions for damping and shimmer
- Stereo Cab: a simple implementation with 2 times 20 biquads of a deluxe reverb cabinet

This implementation drives the axoloti to its limits of CPU usage and program size.

# Controlling

parameters can be controlled by using external MIDI-CC's
patch settings are recalled with MIDI-PC's

