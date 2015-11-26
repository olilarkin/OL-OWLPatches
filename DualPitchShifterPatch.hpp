/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "Dual Pitch Shifter"
version: "0.1"
Code generated with Faust 2.0.a41 (http://faust.grame.fr)
------------------------------------------------------------ */

#ifndef  __DualPitchShifter_H__
#define  __DualPitchShifter_H__
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

#ifndef __DualPitchShifterPatch_h__
#define __DualPitchShifterPatch_h__

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


#ifndef FAUSTCLASS 
#define FAUSTCLASS DualPitchShifter
#endif

class DualPitchShifter : public dsp {
	
  private:
	
	float fVec0[65536];
	float fVec1[65536];
	float fRec0[2];
	float fRec2[2];
	float fRec1[2];
	float fRec3[2];
	int IOTA;
	int fSamplingFreq;
	int iConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fConst4;
	FAUSTFLOAT fHslider3;
	
  public:
	
	void static metadata(Meta* m) { 
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("copyright", "Oliver Larkin");
		m->declare("description", "Dual Channel pitch shifter, based on Faust pitch_shifter.dsp by Grame");
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
		m->declare("name", "Dual Pitch Shifter");
		m->declare("version", "0.1");
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
		IOTA = 0;
		for (int i0 = 0; (i0 < 65536); i0 = (i0 + 1)) {
			fVec0[i0] = 0.f;
			
		}
		for (int i1 = 0; (i1 < 65536); i1 = (i1 + 1)) {
			fVec1[i1] = 0.f;
			
		}
		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = expf((0.f - (200.f / float(iConst0))));
		fConst2 = (1.f - fConst1);
		fHslider0 = FAUSTFLOAT(0.5);
		for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
			fRec0[i2] = 0.f;
			
		}
		fHslider1 = FAUSTFLOAT(0.);
		fConst3 = (0.001f * (float(iConst0) * fConst2));
		fHslider2 = FAUSTFLOAT(50.);
		for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
			fRec2[i3] = 0.f;
			
		}
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec1[i4] = 0.f;
			
		}
		fConst4 = (50.f / float(iConst0));
		fHslider3 = FAUSTFLOAT(0.);
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fRec3[i5] = 0.f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	
	virtual void buildUserInterface(UI* interface) {
		interface->openVerticalBox("0x00");
		interface->declare(&fHslider0, "OWL", "PARAMETER_D");
		interface->addHorizontalSlider("Mix", &fHslider0, 0.5f, 0.f, 1.f, 0.01f);
		interface->declare(&fHslider1, "OWL", "PARAMETER_A");
		interface->declare(&fHslider1, "unit", "semitones");
		interface->addHorizontalSlider("Shift L", &fHslider1, 0.f, -12.f, 12.f, 0.1f);
		interface->declare(&fHslider3, "OWL", "PARAMETER_B");
		interface->declare(&fHslider3, "unit", "semitones");
		interface->addHorizontalSlider("Shift R", &fHslider3, 0.f, -12.f, 12.f, 0.1f);
		interface->declare(&fHslider2, "OWL", "PARAMETER_C");
		interface->declare(&fHslider2, "unit", "ms");
		interface->addHorizontalSlider("Window Size", &fHslider2, 50.f, 20.f, 1000.f, 1.f);
		interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (fConst2 * float(fHslider0));
		float fSlow1 = (1.f - powf(2.f, (0.0833333f * float(fHslider1))));
		float fSlow2 = (fConst3 * float(fHslider2));
		float fSlow3 = (1.f - powf(2.f, (0.0833333f * float(fHslider3))));
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			fVec0[(IOTA & 65535)] = fTemp0;
			float fTemp1 = float(input1[i]);
			fVec1[(IOTA & 65535)] = fTemp1;
			fRec0[0] = ((fConst1 * fRec0[1]) + fSlow0);
			float fTemp2 = (1.f - fRec0[0]);
			fRec2[0] = (fSlow2 + (fConst1 * fRec2[1]));
			fRec1[0] = fmodf((fSlow1 + (fRec2[0] + fRec1[1])), fRec2[0]);
			int iTemp3 = int(fRec1[0]);
			int iTemp4 = (1 + iTemp3);
			float fTemp5 = min((fConst4 * fRec1[0]), 1.f);
			float fTemp6 = (fRec2[0] + fRec1[0]);
			int iTemp7 = int(fTemp6);
			int iTemp8 = (1 + iTemp7);
			output0[i] = FAUSTFLOAT(((fTemp0 * fTemp2) + (fRec0[0] * ((((fVec0[((IOTA - (iTemp3 & 65535)) & 65535)] * (float(iTemp4) - fRec1[0])) + ((fRec1[0] - float(iTemp3)) * fVec0[((IOTA - (iTemp4 & 65535)) & 65535)])) * fTemp5) + (((fVec0[((IOTA - (iTemp7 & 65535)) & 65535)] * (0.f - (fTemp6 - float(iTemp8)))) + ((fTemp6 - float(iTemp7)) * fVec0[((IOTA - (iTemp8 & 65535)) & 65535)])) * (1.f - fTemp5))))));
			fRec3[0] = fmodf((fSlow3 + (fRec2[0] + fRec3[1])), fRec2[0]);
			int iTemp9 = int(fRec3[0]);
			int iTemp10 = (1 + iTemp9);
			float fTemp11 = min((fConst4 * fRec3[0]), 1.f);
			float fTemp12 = (fRec2[0] + fRec3[0]);
			int iTemp13 = int(fTemp12);
			int iTemp14 = (1 + iTemp13);
			output1[i] = FAUSTFLOAT(((fTemp1 * fTemp2) + (fRec0[0] * ((((fVec1[((IOTA - (iTemp9 & 65535)) & 65535)] * (float(iTemp10) - fRec3[0])) + ((fRec3[0] - float(iTemp9)) * fVec1[((IOTA - (iTemp10 & 65535)) & 65535)])) * fTemp11) + (((fVec1[((IOTA - (iTemp13 & 65535)) & 65535)] * (0.f - (fTemp12 - float(iTemp14)))) + ((fTemp12 - float(iTemp13)) * fVec1[((IOTA - (iTemp14 & 65535)) & 65535)])) * (1.f - fTemp11))))));
			IOTA = (IOTA + 1);
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			
		}
		
	}

	
};


/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/



/**************************************************************************************

	DualPitchShifterPatch : an OWL patch that calls Faust generated DSP code
	
***************************************************************************************/

class DualPitchShifterPatch : public Patch
{
    DualPitchShifter   fDSP;
    OwlUI	fUI;
    
public:

    DualPitchShifterPatch() : fUI(this)
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

#endif // __DualPitchShifterPatch_h__


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
