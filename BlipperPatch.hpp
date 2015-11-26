/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "Blipper"
version: "0.2"
Code generated with Faust 2.0.a41 (http://faust.grame.fr)
------------------------------------------------------------ */

#ifndef  __Blipper_H__
#define  __Blipper_H__
/************************************************************************

	IMPORTANT NOTE : this file contains two clearly delimited sections :
	the ARCHITECTURE section (in two parts) and the USER section. Each section
	is governed by its own copyright and license. Please check individually
	each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2014 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

	EXCEPTION : As a special exception, you may create a larger work
	that contains this FAUST architecture section and distribute
	that work under terms of your choice, so long as this FAUST
	architecture section is not modified.


 ************************************************************************
 ************************************************************************/

#ifndef __BlipperPatch_h__
#define __BlipperPatch_h__

#include "StompBox.h"
#include <cstddef>
#include <string.h>
#include <strings.h>


#ifndef __FaustCommonInfrastructure__
#define __FaustCommonInfrastructure__


/************************************************************************
	IMPORTANT NOTE : this file contains two clearly delimited sections :
	the ARCHITECTURE section (in two parts) and the USER section. Each section
	is governed by its own copyright and license. Please check individually
	each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

	EXCEPTION : As a special exception, you may create a larger work
	that contains this FAUST architecture section and distribute
	that work under terms of your choice, so long as this FAUST
	architecture section is not modified.


 ************************************************************************
 ************************************************************************/
 
/******************************************************************************
*******************************************************************************

								FAUST DSP

*******************************************************************************
*******************************************************************************/

#ifndef __dsp__
#define __dsp__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;

//----------------------------------------------------------------
//  Signal processor definition
//----------------------------------------------------------------

class dsp {

 protected:
	int fSamplingFreq;
    
 public:
	dsp() {}
	virtual ~dsp() {}

	virtual int getNumInputs() 										= 0;
	virtual int getNumOutputs() 									= 0;
	virtual void buildUserInterface(UI* ui_interface) 				= 0;
	virtual void init(int samplingRate) 							= 0;
 	virtual void compute(int len, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) 	= 0;
};

// On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
// flags to avoid costly denormals
#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
#ifndef FAUST_UI_H
#define FAUST_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust User Interface
 * This abstract class contains only the method that the faust compiler can
 * generate to describe a DSP interface.
 ******************************************************************************/

class UI
{

 public:

	UI() {}

	virtual ~UI() {}

    // -- widget's layouts

    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;

    // -- active widgets

    virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

    // -- passive widgets

    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;

	// -- metadata declarations

    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}
};

#endif



struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
};



/**************************************************************************************

	OwlWidget : object used by OwlUI to ensures the connection between an owl parameter 
	and a faust widget
	
***************************************************************************************/

class OwlWidget
{
  protected:
	Patch* 	fPatch;		// needed to register and read owl parameters
	PatchParameterId	fParameter;		// OWL parameter code : PARAMETER_A,...
	FAUSTFLOAT* 		fZone;			// Faust widget zone
	const char*			fLabel;			// Faust widget label 
	float				fMin;			// Faust widget minimal value
	float				fSpan;			// Faust widget value span (max-min)
	
  public:
	OwlWidget() :
		fPatch(0), fParameter(PARAMETER_A), fZone(0), fLabel(""), fMin(0), fSpan(1) {}
	OwlWidget(const OwlWidget& w) :
		fPatch(w.fPatch), fParameter(w.fParameter), fZone(w.fZone), fLabel(w.fLabel), fMin(w.fMin), fSpan(w.fSpan) {}
	OwlWidget(Patch* pp, PatchParameterId param, FAUSTFLOAT* z, const char* l, float lo, float hi) :
		fPatch(pp), fParameter(param), fZone(z), fLabel(l), fMin(lo), fSpan(hi-lo) {}
	void bind() 	{ fPatch->registerParameter(fParameter, fLabel); }
	void update()	{ *fZone = fMin + fSpan*fPatch->getParameterValue(fParameter); }
	
};


/**************************************************************************************

	OwlUI : Faust User Interface builder. Passed to buildUserInterface OwlUI ensures
	the mapping between owl parameters and faust widgets. It relies on specific
	metadata "...[OWL:PARAMETER_X]..." in widget's label for that. For example any 
	faust widget with metadata [OWL:PARAMETER_B] will be controlled by PARAMETER_B 
	(the second knob).
	
***************************************************************************************/

// The maximun number of mappings between owl parameters and faust widgets 
#define MAXOWLWIDGETS 8

class OwlUI : public UI
{
	Patch* 	fPatch;
	PatchParameterId	fParameter;					// current parameter ID, value PARAMETER_F means not set
	int					fIndex;						// number of OwlWidgets collected so far
	OwlWidget			fTable[MAXOWLWIDGETS];		// kind of static list of OwlWidgets
	
	// check if the widget is an Owl parameter and, if so, add the corresponding OwlWidget
	void addOwlWidget(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT lo, FAUSTFLOAT hi) {
		if ((fParameter >= PARAMETER_A) && (fParameter <= PARAMETER_E) && (fIndex < MAXOWLWIDGETS)) {
			fTable[fIndex] = OwlWidget(fPatch, fParameter, zone, label, lo, hi);
			fTable[fIndex].bind();
			fIndex++;
		}
		fParameter = PARAMETER_F; 		// clear current parameter ID
	}

	// we dont want to create a widget by-ut we clear the current parameter ID just in case
	void skip() {
		fParameter = PARAMETER_F; 		// clear current parameter ID
	}

 public:

	OwlUI(Patch* pp) : fPatch(pp), fParameter(PARAMETER_F), fIndex(0) {}
	
	virtual ~OwlUI() {}
	
	// should be called before compute() to update widget's zones registered as Owl parameters
	void update() {
		for (int i=0; i<fIndex; i++)  fTable[i].update();
	}

	//---------------------- virtual methods called by buildUserInterface ----------------
	
    // -- widget's layouts

    virtual void openTabBox(const char* label) {}
    virtual void openHorizontalBox(const char* label) {}
    virtual void openVerticalBox(const char* label) {}
    virtual void closeBox() {}

    // -- active widgets

    virtual void addButton(const char* label, FAUSTFLOAT* zone) 																			{ skip(); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) 																		{ skip(); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT lo, FAUSTFLOAT hi, FAUSTFLOAT step) 	{ addOwlWidget(label, zone, lo, hi); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT lo, FAUSTFLOAT hi, FAUSTFLOAT step) 	{ addOwlWidget(label, zone, lo, hi); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT lo, FAUSTFLOAT hi, FAUSTFLOAT step) 			{ addOwlWidget(label, zone, lo, hi); }

    // -- passive widgets

    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT lo, FAUSTFLOAT hi) 									{ skip(); }
    virtual void addVerticalBargraph  (const char* label, FAUSTFLOAT* zone, FAUSTFLOAT lo, FAUSTFLOAT hi) 									{ skip(); }

	// -- metadata declarations

    virtual void declare(FAUSTFLOAT* z, const char* k, const char* id) {
    	if (strcasecmp(k,"OWL") == 0) {
    		     if (strcasecmp(id,"PARAMETER_A") == 0)  fParameter = PARAMETER_A;
    		else if (strcasecmp(id,"PARAMETER_B") == 0)  fParameter = PARAMETER_B;
    		else if (strcasecmp(id,"PARAMETER_C") == 0)  fParameter = PARAMETER_C;
    		else if (strcasecmp(id,"PARAMETER_D") == 0)  fParameter = PARAMETER_D;
    		else if (strcasecmp(id,"PARAMETER_E") == 0)  fParameter = PARAMETER_E;
    	}
    }
};

#endif // __FaustCommonInfrastructure__

/**************************BEGIN USER SECTION **************************/


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

#include <math.h>

float faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS Blipper
#endif

class Blipper : public dsp {
	
  private:
	
	float fVec2[4096];
	int iVec0[2];
	float fRec0[2];
	float fRec2[2];
	float fRec1[2];
	float fRec5[2];
	float fRec6[2];
	float fRec4[2];
	float fVec1[2];
	float fRec3[2];
	int fSamplingFreq;
	int iConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	FAUSTFLOAT fHslider1;
	float fConst7;
	float fConst8;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	int IOTA;
	float fConst9;
	
  public:
	
	void static metadata(Meta* m) { 
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("copyright", "Oliver Larkin");
		m->declare("description", "Envelope Follower controlling pitch of a triangle oscillator, good with percussive input");
		m->declare("effect.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("effect.lib/copyright", "Julius O. Smith III");
		m->declare("effect.lib/exciter_author", "Priyanka Shekar (pshekar@ccrma.stanford.edu)");
		m->declare("effect.lib/exciter_copyright", "Copyright (c) 2013 Priyanka Shekar");
		m->declare("effect.lib/exciter_license", "MIT License (MIT)");
		m->declare("effect.lib/exciter_name", "Harmonic Exciter");
		m->declare("effect.lib/exciter_version", "1.0");
		m->declare("effect.lib/license", "STK-4.3");
		m->declare("effect.lib/name", "Faust Audio Effect Library");
		m->declare("effect.lib/version", "1.33");
		m->declare("filter.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("filter.lib/copyright", "Julius O. Smith III");
		m->declare("filter.lib/license", "STK-4.3");
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/reference", "https://ccrma.stanford.edu/~jos/filters/");
		m->declare("filter.lib/version", "1.29");
		m->declare("licence", "GPL");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/license", "LGPL with exception");
		m->declare("math.lib/name", "Math Library");
		m->declare("math.lib/version", "1.0");
		m->declare("music.lib/author", "GRAME");
		m->declare("music.lib/copyright", "GRAME");
		m->declare("music.lib/license", "LGPL with exception");
		m->declare("music.lib/name", "Music Library");
		m->declare("music.lib/version", "1.0");
		m->declare("name", "Blipper");
		m->declare("oscillator.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("oscillator.lib/copyright", "Julius O. Smith III");
		m->declare("oscillator.lib/license", "STK-4.3");
		m->declare("oscillator.lib/name", "Faust Oscillator Library");
		m->declare("oscillator.lib/version", "1.11");
		m->declare("version", "0.2");
	}

	virtual int getNumInputs() {
		return 2;
		
	}
	virtual int getNumOutputs() {
		return 2;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	static void classInit(int samplingFreq) {
		
	}
	
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		for (int i0 = 0; (i0 < 2); i0 = (i0 + 1)) {
			iVec0[i0] = 0;
			
		}
		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = expf((0.f - (200.f / float(iConst0))));
		fConst2 = (1.f - fConst1);
		fHslider0 = FAUSTFLOAT(0.5);
		for (int i1 = 0; (i1 < 2); i1 = (i1 + 1)) {
			fRec0[i1] = 0.f;
			
		}
		fConst3 = (1760.f / float(iConst0));
		fConst4 = (1.f / float(iConst0));
		fConst5 = float(iConst0);
		fConst6 = (1.f / fConst5);
		fHslider1 = FAUSTFLOAT(20.);
		for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
			fRec2[i2] = 0.f;
			
		}
		for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
			fRec1[i3] = 0.f;
			
		}
		fConst7 = expf((0.f - (100.f / float(iConst0))));
		fConst8 = (1.f - fConst7);
		fHslider2 = FAUSTFLOAT(60.);
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec5[i4] = 0.f;
			
		}
		fHslider3 = FAUSTFLOAT(24.);
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fRec6[i5] = 0.f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec4[i6] = 0.f;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fVec1[i7] = 0.f;
			
		}
		IOTA = 0;
		for (int i8 = 0; (i8 < 4096); i8 = (i8 + 1)) {
			fVec2[i8] = 0.f;
			
		}
		fConst9 = (0.5f * fConst5);
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec3[i9] = 0.f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	
	virtual void buildUserInterface(UI* interface) {
		interface->openVerticalBox("0x00");
		interface->declare(&fHslider2, "OWL", "PARAMETER_A");
		interface->declare(&fHslider2, "unit", "semitones");
		interface->addHorizontalSlider("BasePitch", &fHslider2, 60.f, 24.f, 96.f, 0.1f);
		interface->declare(&fHslider0, "OWL", "PARAMETER_D");
		interface->addHorizontalSlider("Mix", &fHslider0, 0.5f, 0.f, 1.f, 0.01f);
		interface->declare(&fHslider3, "OWL", "PARAMETER_B");
		interface->declare(&fHslider3, "unit", "semitones");
		interface->addHorizontalSlider("PitchMod", &fHslider3, 24.f, -64.f, 64.f, 1.f);
		interface->declare(&fHslider1, "OWL", "PARAMETER_C");
		interface->declare(&fHslider1, "unit", "ms");
		interface->addHorizontalSlider("Release", &fHslider1, 20.f, 2.f, 100.f, 1.f);
		interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (fConst2 * float(fHslider0));
		float fSlow1 = expf((0.f - (fConst4 / max(fConst6, (0.001f * float(fHslider1))))));
		float fSlow2 = (1.f - fSlow1);
		float fSlow3 = (fConst8 * float(fHslider2));
		float fSlow4 = (fConst2 * float(fHslider3));
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = float(input1[i]);
			iVec0[0] = 1;
			fRec0[0] = ((fConst1 * fRec0[1]) + fSlow0);
			float fTemp2 = (1.f - fRec0[0]);
			float fTemp3 = fabsf((fTemp0 + fTemp1));
			fRec2[0] = max(fTemp3, ((fSlow1 * fRec2[1]) + (fSlow2 * fTemp3)));
			fRec1[0] = ((fConst1 * fRec1[1]) + (fConst2 * fRec2[0]));
			fRec5[0] = ((fConst7 * fRec5[1]) + fSlow3);
			fRec6[0] = ((fConst1 * fRec6[1]) + fSlow4);
			float fTemp4 = powf(2.f, (0.0833333f * ((fRec5[0] + (fRec6[0] * fRec1[0])) - 69.f)));
			float fTemp5 = max((440.f * fTemp4), 23.4489f);
			fRec4[0] = fmodf((fRec4[1] + (fConst6 * fTemp5)), 1.f);
			float fTemp6 = faustpower2_f(((2.f * fRec4[0]) - 1.f));
			fVec1[0] = fTemp6;
			float fTemp7 = ((float(iVec0[1]) * (fTemp6 - fVec1[1])) / fTemp5);
			fVec2[(IOTA & 4095)] = fTemp7;
			float fTemp8 = max(0.f, min(2047.f, (fConst9 / fTemp5)));
			int iTemp9 = int(fTemp8);
			int iTemp10 = (1 + iTemp9);
			fRec3[0] = ((0.999f * fRec3[1]) + (fConst5 * (((0.25f * fTemp7) - (0.25f * (fVec2[((IOTA - iTemp9) & 4095)] * (float(iTemp10) - fTemp8)))) - (0.25f * ((fTemp8 - float(iTemp9)) * fVec2[((IOTA - iTemp10) & 4095)])))));
			float fTemp11 = (fConst3 * (((fRec0[0] * fRec1[0]) * fRec3[0]) * fTemp4));
			output0[i] = FAUSTFLOAT(((fTemp0 * fTemp2) + fTemp11));
			output1[i] = FAUSTFLOAT(((fTemp1 * fTemp2) + fTemp11));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fVec1[1] = fVec1[0];
			IOTA = (IOTA + 1);
			fRec3[1] = fRec3[0];
			
		}
		
	}

	
};


/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/



/**************************************************************************************

	BlipperPatch : an OWL patch that calls Faust generated DSP code
	
***************************************************************************************/

class BlipperPatch : public Patch
{
    Blipper   fDSP;
    OwlUI	fUI;
    
public:

    BlipperPatch() : fUI(this)
    {
        fDSP.init(int(getSampleRate()));		// Init Faust code with the OWL sampling rate
        fDSP.buildUserInterface(&fUI);			// Maps owl parameters and faust widgets 
    }
    
    void processAudio(AudioBuffer &buffer)
    {
        // Reasonably assume we will not have more than 32 channels
        float*  ins[32];
        float*  outs[32];
        int     n = buffer.getChannels();
        
        if ( (fDSP.getNumInputs() < 32) && (fDSP.getNumOutputs() < 32) ) {
            
            // create the table of input channels
            for(int ch=0; ch<fDSP.getNumInputs(); ++ch) {
                ins[ch] = buffer.getSamples(ch%n);
            }
            
            // create the table of output channels
            for(int ch=0; ch<fDSP.getNumOutputs(); ++ch) {
                outs[ch] = buffer.getSamples(ch%n);
            }
            
            // read OWL parameters and updates corresponding Faust Widgets zones
            fUI.update(); 
            
            // Process the audio samples
            fDSP.compute(buffer.getSize(), ins, outs);
        }
    }

};

#endif // __BlipperPatch_h__


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
