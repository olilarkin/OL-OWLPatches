declare name "Stereo Frequency Shifter";
declare description "Stereo Frequency Shifting";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("effect.lib");
import("filter.lib"); 

import("FrequencyShifter.dsp");

shift = hslider("Shift [unit:hz] [OWL:PARAMETER_A]", 0.0, -1., 1, 0.001);
shift_scalar = hslider("Shift Scalar [OWL:PARAMETER_B]", 1., 1., 100, 0.1);
lr_offset = hslider("L-R Offset [OWL:PARAMETER_C]", 0., 0., 1., 0.00001);
mix = hslider("Mix [OWL:PARAMETER_D]",0.5,0,1,0.01) : smooth(tau2pole(0.005));

shift_amount = shift*shift_scalar;
process(l, r) = l, r <: *(1-mix), *(1-mix), ssb(shift_amount,l)*mix, ssb(shift_amount+lr_offset,r)*mix :> _,_;