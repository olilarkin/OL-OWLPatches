declare name "Weird Phaser";
declare description "Stereo Phaser based on SSB Modulation";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("IIRHilbert.dsp");
import("filter.lib");

lutsize = 1 << 9;
sintable = float(time)*(2.0*PI)/float(lutsize) : sin;

rate = hslider("Rate [unit:hz] [OWL:PARAMETER_A]", 0, 0., 1, 0.001);
rateScalar = hslider("Rate Scalar [OWL:PARAMETER_B]", 1., 1., 100, 0.001);
offset = hslider("L-R Offset [OWL:PARAMETER_C]", 0., 0., 1., 0.001) * 0.5;
mix = hslider("Mix [OWL:PARAMETER_D]",0.5,0,1,0.01) : smooth(tau2pole(0.005));

quadlookup(phase)=ss1+d*(ss2-ss1), sc1+d*(sc2-sc1)
with {
  sini = int(phase * lutsize); 
  d = decimal(phase * lutsize);
  cosi = int(fmod((phase * lutsize)+(lutsize*0.25), lutsize));  
  ss1 = rdtable(lutsize+1,sintable,sini);
  ss2 = rdtable(lutsize+1,sintable,sini+1);
  sc1 = rdtable(lutsize+1,sintable,cosi);
  sc2 = rdtable(lutsize+1,sintable,cosi+1);
};

hilbert = hilbertef;

ssbfreqshift(x, offset) = negative
with {
  softclip = clip(-1,1);
  fbkfilter = _;
  negative(x) = real(x)*cosv - imag(x)*sinv;
  positive(x) = real(x)*cosv + imag(x)*sinv;
  real(x) = hilbert(x) : _ , !;
  imag(x) = hilbert(x) : ! , _;
  phasor = fmod(((rate*rateScalar)/float(SR) : (+ : decimal) ~ _)+offset, 1.);
  sinv = quadlookup(phasor) : _ , !;
  cosv = quadlookup(phasor) : ! , _;
};

process(l,r) = l,r <: *(1-mix), *(1-mix), ssbfreqshift(l, 0.)*mix, ssbfreqshift(r, offset)*mix :> _,_;