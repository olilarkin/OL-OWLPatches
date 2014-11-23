import("oscillator.lib");
import("IIRHilbert.dsp");

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

ssb(shift, x) = freqshift(x, shift) : _ , !; // only take one sideband