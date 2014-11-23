declare name "Dual Pitch Shifter";
declare description "Dual Channel pitch shifter, based on Faust pitch_shifter.dsp by Grame";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("music.lib");
import("filter.lib");

msec = SR/1000.0;
shiftl = hslider("Shift L [unit:semitones] [OWL:PARAMETER_A]", 0, -12, +12, 0.1);
shiftr = hslider("Shift R [unit:semitones] [OWL:PARAMETER_B]", 0, -12, +12, 0.1);
ws = hslider("Window Size [unit:ms] [OWL:PARAMETER_C]", 50, 20, 1000, 1) * msec : smooth(tau2pole(0.005));
mix = hslider("Mix[OWL:PARAMETER_D]", 0.5, 0, 1, 0.01) : smooth(tau2pole(0.005));

xf = 20 * msec;

transpose (w, x, s, sig) = fdelay(65536, d,sig)*fmin(d/x,1) + fdelay(65536,d+w,sig)*(1-fmin(d/x,1))
with {
  i = 1 - pow(2, s/12);
  d = i : (+ : +(w) : fmod(_,w)) ~ _;

  fdelay(n,d,x) = delay(n,int(d),x)*(1 - frac(d)) + delay(n,int(d)+1,x)*frac(d)
  with {
    frac(n) = n-int(n);
    index(n) = &(n-1) ~ +(1);
    delay(n,d,x) = x@(int(d)&(n-1));
  };
};
	
process(l,r) = l,r <: *(1-mix), *(1-mix), transpose(ws, xf, shiftl, l)*mix, transpose(ws, xf, shiftr, r)*mix :> _,_;
