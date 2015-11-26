/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "Thru Zero Flanger"
version: "0.1"
Code generated with Faust 2.0.a41 (http://faust.grame.fr)
------------------------------------------------------------ */

#ifndef  __ThruZeroFlanger_H__
#define  __ThruZeroFlanger_H__
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

#ifndef __ThruZeroFlangerPatch_h__
#define __ThruZeroFlangerPatch_h__

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


class ThruZeroFlangerSIG0 {
	
  private:
	
	int iRec2[2];
	
  public:
	
	int getNumInputsThruZeroFlangerSIG0() {
		return 0;
		
	}
	int getNumOutputsThruZeroFlangerSIG0() {
		return 1;
		
	}
	int getInputRateThruZeroFlangerSIG0(int channel) {
		int rate;
		switch (channel) {
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	int getOutputRateThruZeroFlangerSIG0(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	void instanceInitThruZeroFlangerSIG0(int samplingFreq) {
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			iRec2[i4] = 0;
			
		}
		
	}
	
	void fillThruZeroFlangerSIG0(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec2[0] = (1 + iRec2[1]);
			float fTemp2 = float((iRec2[0] - 1));
			float fTemp3 = (0.00195312f * fTemp2);
			float fTemp4 = (0.00390625f * fTemp2);
			output[i] = (2.f * ((float(((0.f <= fTemp3) & (fTemp3 <= 0.5f))) * (fTemp4 - 0.5f)) + (float(((0.5f < fTemp3) & (fTemp3 <= 1.f))) * (1.5f - fTemp4))));
			iRec2[1] = iRec2[0];
			
		}
		
	}
};

ThruZeroFlangerSIG0* newThruZeroFlangerSIG0() {return (ThruZeroFlangerSIG0*) new ThruZeroFlangerSIG0(); }
void deleteThruZeroFlangerSIG0(ThruZeroFlangerSIG0* dsp) {delete dsp; }

static float ftbl0ThruZeroFlangerSIG0[513];

#ifndef FAUSTCLASS 
#define FAUSTCLASS ThruZeroFlanger
#endif

class ThruZeroFlanger : public dsp {
	
  private:
	
	float fVec0[4096];
	float fVec1[4096];
	float fRec0[2];
	float fRec1[2];
	float fRec3[2];
	float fRec4[2];
	int IOTA;
	int fSamplingFreq;
	int iConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fHslider0;
	float fConst5;
	FAUSTFLOAT fHslider1;
	float fConst6;
	FAUSTFLOAT fHslider2;
	float fConst7;
	FAUSTFLOAT fHslider3;
	
  public:
	
	void static metadata(Meta* m) { 
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("copyright", "Oliver Larkin");
		m->declare("description", "Stereo Thru Zero Flanger - warning can ZERO the sound!");
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
		m->declare("name", "Thru Zero Flanger");
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
		ThruZeroFlangerSIG0* sig0 = newThruZeroFlangerSIG0();
		sig0->instanceInitThruZeroFlangerSIG0(samplingFreq);
		sig0->fillThruZeroFlangerSIG0(513, ftbl0ThruZeroFlangerSIG0);
		deleteThruZeroFlangerSIG0(sig0);
		
	}
	
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		IOTA = 0;
		for (int i0 = 0; (i0 < 4096); i0 = (i0 + 1)) {
			fVec0[i0] = 0.f;
			
		}
		for (int i1 = 0; (i1 < 4096); i1 = (i1 + 1)) {
			fVec1[i1] = 0.f;
			
		}
		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = float(iConst0);
		fConst2 = (0.001f * fConst1);
		fConst3 = expf((0.f - (200.f / float(iConst0))));
		fConst4 = (1.f - fConst3);
		fHslider0 = FAUSTFLOAT(10.);
		for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
			fRec0[i2] = 0.f;
			
		}
		fConst5 = (0.01f * fConst4);
		fHslider1 = FAUSTFLOAT(20.);
		for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
			fRec1[i3] = 0.f;
			
		}
		fConst6 = (1.f / fConst1);
		fHslider2 = FAUSTFLOAT(0.1);
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fRec3[i5] = 0.f;
			
		}
		fConst7 = (0.5f * fConst4);
		fHslider3 = FAUSTFLOAT(0.);
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec4[i6] = 0.f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	
	virtual void buildUserInterface(UI* interface) {
		interface->openVerticalBox("0x00");
		interface->declare(&fHslider0, "OWL", "PARAMETER_B");
		interface->declare(&fHslider0, "unit", "ms");
		interface->addHorizontalSlider("Delay", &fHslider0, 10.f, 0.5f, 20.f, 0.01f);
		interface->declare(&fHslider1, "OWL", "PARAMETER_D");
		interface->declare(&fHslider1, "unit", "%");
		interface->addHorizontalSlider("Depth", &fHslider1, 20.f, 3.f, 100.f, 1.f);
		interface->declare(&fHslider3, "OWL", "PARAMETER_C");
		interface->addHorizontalSlider("L-R Offset", &fHslider3, 0.f, 0.f, 1.f, 0.001f);
		interface->declare(&fHslider2, "OWL", "PARAMETER_A");
		interface->declare(&fHslider2, "unit", "hz");
		interface->addHorizontalSlider("Rate", &fHslider2, 0.1f, 0.f, 1.f, 0.001f);
		interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (fConst4 * float(fHslider0));
		float fSlow1 = (fConst5 * float(fHslider1));
		float fSlow2 = (fConst6 * float(fHslider2));
		float fSlow3 = (fConst7 * float(fHslider3));
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			fVec0[(IOTA & 4095)] = fTemp0;
			float fTemp1 = float(input1[i]);
			fVec1[(IOTA & 4095)] = fTemp1;
			fRec0[0] = (fSlow0 + (fConst3 * fRec0[1]));
			fRec1[0] = (fSlow1 + (fConst3 * fRec1[1]));
			float fTemp5 = (fSlow2 + fRec3[1]);
			fRec3[0] = (fTemp5 - floorf(fTemp5));
			float fTemp6 = (512.f * fmodf(fRec3[0], 1.f));
			int iTemp7 = int(fTemp6);
			float fTemp8 = (fConst2 * (fRec0[0] * (1.f + (fRec1[0] * (ftbl0ThruZeroFlangerSIG0[iTemp7] + ((fTemp6 - floorf(fTemp6)) * (ftbl0ThruZeroFlangerSIG0[(1 + iTemp7)] - ftbl0ThruZeroFlangerSIG0[iTemp7])))))));
			int iTemp9 = int(fTemp8);
			int iTemp10 = (1 + iTemp9);
			float fTemp11 = (fConst2 * fRec0[0]);
			int iTemp12 = int(fTemp11);
			int iTemp13 = (iTemp12 & 4095);
			int iTemp14 = (1 + iTemp12);
			float fTemp15 = (float(iTemp14) - fTemp11);
			float fTemp16 = (fTemp11 - float(iTemp12));
			int iTemp17 = (iTemp14 & 4095);
			output0[i] = FAUSTFLOAT((0.f - (((fVec0[((IOTA - (iTemp9 & 4095)) & 4095)] * (float(iTemp10) - fTemp8)) + ((fTemp8 - float(iTemp9)) * fVec0[((IOTA - (iTemp10 & 4095)) & 4095)])) - ((fVec0[((IOTA - iTemp13) & 4095)] * fTemp15) + (fTemp16 * fVec0[((IOTA - iTemp17) & 4095)])))));
			fRec4[0] = (fSlow3 + (fConst3 * fRec4[1]));
			float fTemp18 = (512.f * fmodf((fRec3[0] + fRec4[0]), 1.f));
			int iTemp19 = int(fTemp18);
			float fTemp20 = (fConst2 * (fRec0[0] * (1.f + (fRec1[0] * (ftbl0ThruZeroFlangerSIG0[iTemp19] + ((fTemp18 - floorf(fTemp18)) * (ftbl0ThruZeroFlangerSIG0[(1 + iTemp19)] - ftbl0ThruZeroFlangerSIG0[iTemp19])))))));
			int iTemp21 = int(fTemp20);
			int iTemp22 = (1 + iTemp21);
			output1[i] = FAUSTFLOAT((0.f - (((fVec1[((IOTA - (iTemp21 & 4095)) & 4095)] * (float(iTemp22) - fTemp20)) + ((fTemp20 - float(iTemp21)) * fVec1[((IOTA - (iTemp22 & 4095)) & 4095)])) - ((fTemp15 * fVec1[((IOTA - iTemp13) & 4095)]) + (fTemp16 * fVec1[((IOTA - iTemp17) & 4095)])))));
			IOTA = (IOTA + 1);
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			
		}
		
	}

	
};


/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/



/**************************************************************************************

	ThruZeroFlangerPatch : an OWL patch that calls Faust generated DSP code
	
***************************************************************************************/

class ThruZeroFlangerPatch : public Patch
{
    ThruZeroFlanger   fDSP;
    OwlUI	fUI;
    
public:

    ThruZeroFlangerPatch() : fUI(this)
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

#endif // __ThruZeroFlangerPatch_h__


////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
