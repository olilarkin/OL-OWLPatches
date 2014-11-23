declare name "Dual Frequency Shifter";
declare description "Dual Channel Frequency Shifter";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("effect.lib");
import("filter.lib"); 

import("FrequencyShifter.dsp");

shiftL = hslider("Shift L [unit:hz] [OWL:PARAMETER_A]", 0., -1., 1, 0.001);
shiftR = hslider("Shift R [unit:hz] [OWL:PARAMETER_B]", 0., -1., 1, 0.001);
shiftScalar = hslider("Shift Scalar [OWL:PARAMETER_C]", 1., 1., 100, 0.1);
mix = hslider("Mix [OWL:PARAMETER_D]",0.5,0,1,0.01) : smooth(tau2pole(0.005));

process(l,r) = l,r <: *(1-mix), *(1-mix), ssb(shiftL*shiftScalar,l)*mix, ssb(shiftR*shiftScalar,r)*mix :> _,_;
