#ifndef __XFMPatch_hpp__
#define __XFMPatch_hpp__

#include "StompBox.h"

/*

 XFM Osc - port of Tom Schouten's xfm~ pd object
 coupled frequency modulation
  
 Parameters:
 
 A) Osc1 Freq
 B) Osc2 Freq
 C) Osc1 Fbk
 D) Osc2 Fbk
 
 
 CHANGES:
 
 TODO:

  -
  
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

class XFMPatch : public Patch
{
private:
  
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
  
  inline float fastClip(float x, const float lo, const float hi)
  {
    const float x1 = fabsf(x-lo);
    const float x2 = fabsf(x-hi);
    return (x1+lo+hi-x2) * 0.5f;
  }

  float mFreqA, mFreqB;
  float mFbkA, mFbkB;
  float mX1, mY1;
  float mX2, mY2;
  float mScale;
  
  PSmooth mFbkASmoother;
  PSmooth mFbkBSmoother;
  PSmooth mFreqASmoother;
  PSmooth mFreqBSmoother;
  
public:
  XFMPatch(float initFreq = 100.f)
  : mFreqA(initFreq)
  , mFreqB(initFreq)
  , mFbkA(initFreq)
  , mFbkB(initFreq) 
  , mX1(1.f)
  , mY1(0.f)
  , mX2(1.f)
  , mY2(0.f)
  {
    mScale = 6.283185307179586 / getSampleRate();
    
    registerParameter(PARAMETER_A, "Osc1 Freq", "Osc1 Freq");
    registerParameter(PARAMETER_B, "Osc2 Freq", "Osc2 Freq");
    registerParameter(PARAMETER_C, "Osc1 Fbk", "Osc1 Fbk");
    registerParameter(PARAMETER_D, "Osc2 Fbk", "Osc2 Fbk");
  }
  
  void processAudio(AudioBuffer &buffer)
  {
    mFreqA = getParameterValue(PARAMETER_A) * 1000.;
    mFreqB = getParameterValue(PARAMETER_B) * 1000.;
    mFbkA = getParameterValue(PARAMETER_C) * 1000.;
    mFbkB = getParameterValue(PARAMETER_D) * 1000.;

    const int size = buffer.getSize();
    float z1, dx1, dy1;
    float z2, dx2, dy2;
    
    float* bufL = buffer.getSamples(0);
    float* bufR = buffer.getSamples(1);

    for(int i = 0; i < size; i++, bufL++, bufR++)
    {
      // osc 1
      z1 = mScale * (mX2 * mFbkASmoother.process(mFbkA) + mFreqASmoother.process(mFreqA));
      
      dx1 = mX1 - z1*mY1;
      dy1 = mY1 + z1*mX1;
      
      mX1 = fastClip(dx1, -1., 1.);
      mY1 = fastClip(dy1, -1., 1.);
      
      // osc 2
      z2 = mScale * (mX1 * mFbkBSmoother.process(mFbkB) + mFreqBSmoother.process(mFreqB));
      
      dx2 = mX2 - z2*mY2;
      dy2 = mY2 + z2*mX2;
      
      mX2 = fastClip(dx2, -1., 1.);
      mY2 = fastClip(dy2, -1., 1.);
      
      *bufL = mX1;
      *bufR = mX2;
    }
  }
  
};

#endif // __XFMPatch_hpp__
