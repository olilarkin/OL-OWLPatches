#ifndef __DroneBox_hpp__
#define __DroneBox_hpp__

#include "StompBox.h"

/*

 DroneBox OWL
 
 Sympathetic Resonance Generator
 A bank of four tuned comb filters / virtual strings which resonate in response to the audio input
 A (very) simplified version of my forthcoming plug-in DroneBox v3
 http://www.olilarkin.co.uk
 
 Ramping code from martind's OWL patches https://github.com/dekstop
 
 Parameters:
 
 A) Coarse pitch
 B) Fine pitch
 C) Decay time
 D) Dry/Wet mix
 
 Version 1.2
 
 CHANGES:
 
 - more efficient
 - damping filters
 - doesn't feedback
 - now works in stereo
 - put everything in patch class
 
 TODO:
 
 - expression pedal support
 
 Copyright (C) 2013-2014  Oliver Larkin
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
*/

#define DB_CLIP(a,lo, hi) ( (a)>(lo)?( (a)<(hi)?(a):(hi) ):(lo) )


class DroneBoxPatch : public Patch
{
private:
  static const unsigned int BUF_SIZE;
  static const unsigned int BUF_MASK;
  static const float MAX_FBK;
  static const float MIN_DT_SAMPLES;
  static const int NUM_COMBS;
  static const float MIN_PITCH;
  static const float MAX_PITCH;
  static const float PITCH_RANGE;
  static const float MIN_DECAY;
  static const float MAX_DECAY;
  static const float DECAY_RANGE;
  static const float FREQ_RATIOS[4];

  inline float midi2CPS(float pitch, float tune = 440.f)
  {
    return tune * powf(2.f, (pitch - 69.f) / 12.f);
  }

  class PSmooth
  {
  private:
    float mA, mB;
    float mOutM1;
    
  public:
    PSmooth(float coeff = 0.99f, float initalValue = 0.f)
    : mA(coeff)
    , mB(1.f - mA)
    , mOutM1(initalValue)
    {
    }
  
    inline float process(float input)
    {
      mOutM1 = (input * mB) + (mOutM1 * mA);
      return mOutM1;
    }
  };

  class DCBlocker
  {
  private:
    float mInM1, mOutM1;
    float mSampleRate;
    float mC;
  public:
    DCBlocker()
    : mInM1(0.f)
    , mOutM1(0.f)
    , mSampleRate(AUDIO_SAMPLINGRATE)
    , mC(1.f - ( 126.f / mSampleRate))
    {
    }
  
    inline float process(float input)
    {
      mOutM1 = input - mInM1 + mC * mOutM1;
      mInM1 = input;
      return mOutM1;
    }
    
    void setSampleRate(float sr)
    {
      mSampleRate = sr;
      mC = 1.f - ( 126.f / mSampleRate);
    }
  };

  class DBCombFilter 
  {
    class DBLPF
    {
    private:
      float mInM1, mInM2, mInM3;
      const float mCoeffA;
      const float mCoeffB;
    
    public:
      DBLPF(float dampCoeff)
      : mInM1(0.f)
      , mInM2(0.f)
      , mInM3(0.f)
      , mCoeffA(0.5f * (1.f + dampCoeff))
      , mCoeffB(0.25f * (1.f - dampCoeff))
      {
      }

      inline float process(float input)
      {
        mInM1 = input;
        float output = ((mCoeffB * (mInM1 + mInM3)) + (mCoeffA * mInM2));
        mInM3 = mInM2; 
        mInM2 = mInM1;
        return output;
      }
    };
  
    inline float wrap(float x, const float lo = 0.f, const float hi = 1.f)
    {
      while (x >= hi) x -= hi;
      while (x < lo)  x += hi - lo;
  
      return x;
    }
  
  private:
    int mDTSamples;
    float mFbkScalar;
    int mWriteAddr;
    float mSampleRate;
    DBLPF mDampingFilterL, mDampingFilterR;
    float* mBufferL; // left input
    float* mBufferR; // right input
  public:
    DBCombFilter()
    : mDTSamples(0)
    , mFbkScalar(0.5f)
    , mWriteAddr(0)
    , mSampleRate(AUDIO_SAMPLINGRATE)
    , mDampingFilterL(0.3f)
    , mDampingFilterR(0.35f)
    , mBufferL(NULL)
    , mBufferR(NULL)
    {
      setFreqCPS(440.f);
      setDecayTimeMs(10.f);
    }
  
    void setBuffer(float* bufferL, float* bufferR)
    {
      mBufferL = bufferL;
      mBufferR = bufferR;
    }
  
    void clearBuffer()
    {
      memset(mBufferL, 0.f, BUF_SIZE*sizeof(float));
      memset(mBufferR, 0.f, BUF_SIZE*sizeof(float));
    }
  
    void setFreqCPS(float freqCPS)
    {
      mDTSamples = DB_CLIP(mSampleRate/freqCPS, MIN_DT_SAMPLES, BUF_SIZE);
    }
  
    // call after setting the frequency
    void setDecayTimeMs(float decayTimeMs)
    {
      float fbk = powf(10.f, ((-60.f / ( ( fabsf(decayTimeMs) * (mSampleRate / 1000.f) ) / mDTSamples)) / 20.f));
      mFbkScalar = DB_CLIP(fbk, 0.f, MAX_FBK);
    }
    
    void setSampleRate(float sr)
    {
      mSampleRate = sr;
    }
  
    inline void process(float inputL, float inputR, float* acccumOutputL, float* acccumOutputR)
    {
      float readAddrF = ( (float) mWriteAddr ) - mDTSamples;
      readAddrF = wrap(readAddrF, 0.f, (float) BUF_SIZE);
    
      // Linear interpolation
      const int intPart = (int) readAddrF;
      const float fracPart = readAddrF-intPart;
      const int wrappedIntPart = intPart & BUF_MASK;
      const int wrappedIntPartPlusOne = (intPart+1) & BUF_MASK;

      float a = mBufferL[wrappedIntPart];
      float b = mBufferL[wrappedIntPartPlusOne];
      const float outputL = a + (b - a) * fracPart;

      a = mBufferR[wrappedIntPart];
      b = mBufferR[wrappedIntPartPlusOne];
      const float outputR = a + (b - a) * fracPart;
    
      mBufferL[mWriteAddr] = inputL + (mDampingFilterL.process(outputL) * mFbkScalar);
      mBufferR[mWriteAddr] = inputR + (mDampingFilterR.process(outputR) * mFbkScalar);
      mWriteAddr++;
      mWriteAddr &= BUF_MASK;
    
      *acccumOutputL += outputL;
      *acccumOutputR += outputR;
    }
  };

private:
  DBCombFilter mCombs[4];
  PSmooth mMixSmoother;
  DCBlocker mDCBlockerL, mDCBlockerR;
  float mOldValues[4];
  float mRamp;
  float mPrevCoarsePitch;
  float mPrevFinePitch;
  float mPrevDecay;
  
  inline float getRampedParameterValue(PatchParameterId id) 
  {
    float val = getParameterValue(id);
    float result = val * mRamp + mOldValues[id] * (1.f-mRamp);
    mOldValues[id] = val;
    return result;
  }
  
public:
  DroneBoxPatch()
  : mRamp(0.1)
  , mPrevCoarsePitch(-1.)
  , mPrevFinePitch(-1.)
  , mPrevDecay(-1.)
  {
    registerParameter(PARAMETER_A, "Coarse Pitch", "Coarse Pitch");
    registerParameter(PARAMETER_B, "Fine Pitch", "Fine Pitch");
    registerParameter(PARAMETER_C, "Decay", "Decay");
    registerParameter(PARAMETER_D, "Mix", "Mix");

    mOldValues[0] = 0.f; 
    mOldValues[1] = 0.f;
    mOldValues[2] = 0.f;
    mOldValues[3] = 0.f;
    
    for (int c=0;c<NUM_COMBS;c++)
    {
      AudioBuffer* buffer = createMemoryBuffer(2, BUF_SIZE);
      mCombs[c].setBuffer(buffer->getSamples(0), buffer->getSamples(1));
      mCombs[c].clearBuffer();
    }
  }
  
  void processAudio(AudioBuffer &buffer)
  {
    const int size = buffer.getSize();
    const float coarsePitch = getRampedParameterValue(PARAMETER_A);
    const float finePitch = getRampedParameterValue(PARAMETER_B);
    const float decay = getRampedParameterValue(PARAMETER_C);
    const float mix = getRampedParameterValue(PARAMETER_D);
    
    if (coarsePitch != mPrevCoarsePitch || finePitch != mPrevFinePitch || decay != mPrevDecay)
    {
      const float freq = midi2CPS(MIN_PITCH + floorf(mPrevCoarsePitch * PITCH_RANGE) + finePitch);
      
      for (int c = 0; c < NUM_COMBS; c++) 
      {
        mCombs[c].setFreqCPS(freq * FREQ_RATIOS[c]);
        mCombs[c].setDecayTimeMs(MIN_DECAY + (decay * DECAY_RANGE));
      }
      
      mPrevCoarsePitch = coarsePitch;
      mPrevFinePitch = finePitch;
      mPrevDecay = decay;
    }
        
    float* bufL = buffer.getSamples(0);
    float* bufR = buffer.getSamples(1);

    for(int i = 0; i < size; i++)
    {
      float ipsL = bufL[i];
      float ipsR = bufR[i];
      float opsL = 0.f;
      float opsR = 0.f;
            
      const float smoothMix = mMixSmoother.process(mix);
      const float invSmoothMix = 1.f-smoothMix;
      
      for (int c = 0; c < NUM_COMBS; c++) 
      {
         mCombs[c].process(ipsL, ipsR, &opsL, &opsR);
      }
      
      bufL[i] = mDCBlockerL.process( ((opsL * 0.1f) * smoothMix) + (ipsL * invSmoothMix) );
      bufR[i] = mDCBlockerR.process( ((opsR * 0.1f) * smoothMix) + (ipsR * invSmoothMix) );
    }
  }  
};

const unsigned int DroneBoxPatch::BUF_SIZE = 1024; // = 4096 bytes per comb
const unsigned int DroneBoxPatch::BUF_MASK = 1023;
const float DroneBoxPatch::MAX_FBK = 0.999999f;
const float DroneBoxPatch::MIN_DT_SAMPLES = 2.5f;
const int DroneBoxPatch::NUM_COMBS = 4;
const float DroneBoxPatch::MIN_PITCH = 36.f; // MIDI notenumber
const float DroneBoxPatch::MAX_PITCH = 60.f; // MIDI notenumber
const float DroneBoxPatch::PITCH_RANGE = MAX_PITCH - MIN_PITCH; // semitones
const float DroneBoxPatch::MIN_DECAY = 200.f; // milliseconds
const float DroneBoxPatch::MAX_DECAY = 30000.f; // milliseconds
const float DroneBoxPatch::DECAY_RANGE = MAX_DECAY - MIN_DECAY;
const float DroneBoxPatch::FREQ_RATIOS[NUM_COMBS] = {1.f, 1.5f, 2.f, 3.f}; 
//const float DroneBoxPatch::FREQ_RATIOS[NUM_COMBS] = {1., 1.01, 1.02, 1.03}; 

#endif // __DroneBox_hpp__
