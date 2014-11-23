#Patches for the OWL Pedal/Eurorack Module

Copyright Oli Larkin 2014

Licence GPL, unless otherwise stated

<http://www.olilarkin.co.uk>

<http://hoxtonowl.com>

##DroneBox OWL
DroneBox OWL is a sympathetic resonance generator: a bank of four virtual tuned strings that resonate in response to the audio input. If the input signal contains energy at the frequency of one of the four strings (or one of their harmonics) it will start the string resonating. The strings are tuned to multiples 1., 1.5, 2. and 3. times the fundamental frequency. It’s a stereo effect and will process stereo signals and works well with many different kinds of audio input. This is a (very) simplified version of my forthcoming plug-in DroneBox v3.

###Parameters:
* A) Coarse pitch (stepped in semitones) -- Range C1 (65Hz) to C3 (261Hz)
* B) Fine pitch (cents)  -- Range 0 to 100
* C) Decay time (ms) -- Range 200 ms to 30 seconds
* D) Dry/Wet mix


##Dual Frequency Shifter
Dual Frequency Shifter is a twin frequency shifter with independent L-R control of frequency shift. Frequency shifting in small amounts can cause phasing when the dry signal is present. Parameter C is a multiplier which will allow more extreme frequency shifting effects. In order to get subtle phasing effects make sure Parameter C is turned fully left.

NOTE: Parameter A and B are bi-polar controls so put it in the middle to set the frequency shift for that channel to 0.

###Parameters:

* A) Left frequency shift (Hz) -- Range -1 to 1
* B) Right frequency shift (Hz) -- Range -1 to 1
* C) Shift scalar -- Range 1 to 100
* D) Dry/Wet mix

##Stereo Frequency Shifter
Stereo Frequency Shifter is a twin frequency shifter with linked control of frequency shift for each channel, that can be slightly offset for lush stereo phasing. In order to get subtle phasing effects make sure Parameter B is turned fully left.

NOTE: Parameter A is a bi-polar control so put it in the middle to set the frequency shift to 0.

###Parameters:
* A) Frequency shift (Hz) -- Range -1 to 1
* B) Shift scalar -- Range 1 to 100
* C) L-R offset (Hz) -- Range 0 to 1
* D) Dry/Wet mix

##XFM Oscillator
XFM Oscillator is a cross coupled FM oscillator, which is ported from Tom Schouten's xfm~ pd object <http://zwizwa.be/darcs/creb/>

###Parameters:
* A) Oscillator 1 frequency (Hz) -- Range 0 to 1000
* B) Oscillator 2 frequency (Hz) -- Range 0 to 1000
* C) Oscillator 1 cross modulation depth (Hz) -- Range 0 to 1000
* D) Oscillator 2 cross modulation depth (Hz) -- Range 0 to 1000

##Thru Zero Flanger
Thru Zero Flanger is a stereo flanger, that can completely cancel left and right channels at a certain point during the LFO’s cycle resulting in some interesting spatial effects, especially with a wide-band noise source as the input signal.

###Parameters:
* A) Rate (Hz) -- Range 0 to 1
* B) Delay time (ms) -- Range 0.5 to 20
* C) L-R offset -- Range 0 to 1
* D) Depth (%) -- Range 0 to 100

##Dual Pitch Shifter
Dual Pitch Shifter is a dual channel pitch shifter, based on Faust’s pitch_shifter.dsp by Grame. Pitch shifting is (intentionally) extremely basic and introduces significant delay based on window size.

NOTE: Parameters A and B are bi-polar controls so put them in the middle to set the pitch shift to 0 semitones.

###Parameters:
* A) Left shift (semitones) -- Range -12 to +12
* B) Right shift (semitones) -- Range -12 to +12
* C) Window size (ms) — Range 20 to 1000
* D) Dry/Wet mix

##Blipper
Blipper is an Envelope Follower which controls the pitch of a triangle oscillator. It is a rough emulation of some of the features of the BOSS PC-2 Percussion Synthesiser. Arthur Russell used something like this to create interesting percussion parts on his records e.g. <https://www.youtube.com/watch?v=LEZevPziZhY>. It works nicely with drums. 

NOTE: Parameter C (Pitch mod depth) is a bi-polar control so put it in the middle to set the depth to 0.

###Parameters:
* A) Base pitch (semitones) -- Range C0 (32Hz) to C6 (2093Hz)
* B) Pitch mod depth (semitones) -- Range -64 to +64 semitones.
* C) Release time (ms) -- Range 2 ms to 100 ms
* D) Dry/Wet mix


