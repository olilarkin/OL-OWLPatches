declare name "Dual Frequency Shifter";
declare description "Stereo Frequency Shifting";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("IIRHilbert.dsp");

import("oscillator.lib");
import("effect.lib");
import("filter.lib"); 
//import("math.lib");

rate = hslider("Rate [unit:hz] [OWL:PARAMETER_A]", 0.01, 0., 1, 0.001);
rateScalar = hslider("Rate Scalar [OWL:PARAMETER_B]", 1., 1., 100, 0.1);
lr_offset = hslider("L-R Offset [OWL:PARAMETER_C]", 0., 0., 1., 0.00001);
mix = hslider("Mix [OWL:PARAMETER_D]",0.5,0,1,0.01) : smooth(tau2pole(0.005));

freqshift(x, shift) = negative(x), positive(x)
with {
  negative(x) = real(x)*cosv - imag(x)*sinv;
  positive(x) = real(x)*cosv + imag(x)*sinv;
  real(x) = hilbert(x) : _ , !;
  imag(x) = hilbert(x) : ! , _;
  sinv = oscrq(shift) : _ , !;
  cosv = oscrq(shift) : ! , _;
  hilbert = hilbertef;
};

monofreqshifter(shift, x) = freqshift(_, shift) : _ , !; // only take one sideband
shift_amount = rate*rateScalar;
process(l,r) = l,r <: *(1-mix), *(1-mix), monofreqshifter(shift_amount,l)*mix, monofreqshifter(shift_amount+lr_offset,r)*mix :> _,_;
