declare name "Blipper";
declare description "Envelope Follower controlling pitch of a triangle oscillator, good with percussive input";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("music.lib"); 
import("oscillator.lib");  
import("effect.lib"); 

basepitch = hslider("BasePitch [unit:semitones] [OWL:PARAMETER_A]", 60, 24, 96, 0.1) : smooth(tau2pole(0.01));
pitchmod = hslider("PitchMod [unit:semitones] [OWL:PARAMETER_B]", 24, -64, 64, 1) : smooth(tau2pole(0.005));
attack = hslider("Attack [unit:ms] [OWL:PARAMETER_C]", 2, 2, 1000, 1) : *(0.001) : max(1.0/float(SR));
release = hslider("Release [unit:ms] [OWL:PARAMETER_D]", 20, 2, 1000, 1) : *(0.001) : max(1.0/float(SR));

process = + : pc2 <: _,_
with {
 pc2 = amp_follower_ud(attack, release) <: (midikey2hz(basepitch + (pitchmod * _)): triangle), _ : *;
};