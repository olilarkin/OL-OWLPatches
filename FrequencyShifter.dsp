import("IIRHilbert.dsp");

freqshift(x, shift) = negative(x), positive(x)
with {
  negative(x) = real(x)*cosv - imag(x)*sinv;
  positive(x) = real(x)*cosv + imag(x)*sinv;
  real(x) = hilbert(x) : _ , !;
  imag(x) = hilbert(x) : ! , _;
  
  phasor(x) = fmod((x/float(SR) : (+ : decimal) ~ _), 1.)  * (PI * 2);

  sinv = sin(phasor(shift));
  cosv = cos(phasor(shift));

  hilbert = hilbertef;
};

ssb(shift, x) = freqshift(x, shift) : _ , !; // only take one sideband