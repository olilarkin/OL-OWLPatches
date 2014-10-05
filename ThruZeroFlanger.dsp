declare name "Thru Zero Flanger";
declare description "Stereo Thru Zero Flanger - warning can ZERO the sound!";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

//import("effect.lib"); 
//import("filter.lib"); 
import("math.lib");

// OWL ram overflow filter.lib bug? so declare these ---------------
time = (+(1)~_ ) - 1;
decimal(x) = x - floor(x);
tablesize = 1 << 9;
phase(freq) = freq/float(samplingfreq) : (+ : decimal) ~ _ : *(float(tablesize));
sintable = float(time)*(2.0*PI)/float(tablesize) : sin;
smooth(s) = *(1.0 - s) : + ~ *(s);
tau2pole(tau) = exp(-1.0/(tau*SR));

frac(n) = n-int(n);
index(n) = &(n-1) ~ +(1); // n = 2**i
delay(n,d,x) = x@(int(d)&(n-1));
fdelay(n,d,x) = delay(n,int(d),x)*(1 - frac(d)) + delay(n,int(d)+1,x)*frac(d);
// ---------------

maxdtms = 20;

rate = hslider("Rate [unit:hz] [OWL:PARAMETER_A]", 0.1, 0., 1, 0.001);
dt = hslider("Delay [unit:ms] [OWL:PARAMETER_B]", 10., 0.5, maxdtms, 0.01) : smooth(tau2pole(0.005));
lr_offset = hslider("L-R Offset [OWL:PARAMETER_C]", 0, 0., 1, 0.001) *(0.5) : smooth(tau2pole(0.005));
depth = hslider("Depth [unit:ms] [OWL:PARAMETER_D]", 20., 3., 100., 1) *(0.01): smooth(tau2pole(0.005));

tbllookup(phase)=s1+d*(s2-s1)
with {
  i = int(phase * tablesize); 
  d = decimal(phase * tablesize);
  triwaveform = triangle((float(time)/float(tablesize))*2*PI);
  triangle(t) = ((0<=t) & (t<=PI))*((2*t-PI)/PI) + ((PI<t) & (t<=2*PI))*((3*PI-2*t)/PI);
  s1 = rdtable(tablesize+1,triwaveform,i);
  s2 = rdtable(tablesize+1,triwaveform,i+1);
};

tzflangeunit(x, offset) = staticdelay(x) + moddelay(x)
with {
  dtsamples = dt * (float(SR)/1000.);
  staticdelay = fdelay(4096, dtsamples);
  moddelay = fdelay(4096, modulation) *(-1.); // inverted 
  phasor = fmod((rate/float(SR) : (+ : decimal) ~ _)+offset, 1.);
  modulation = dtsamples + ((tbllookup(phasor)*depth) * dtsamples);
};

process(l,r) = tzflangeunit(l, 0.), tzflangeunit(r, lr_offset);