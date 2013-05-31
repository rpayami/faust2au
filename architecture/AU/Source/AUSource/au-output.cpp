//-----------------------------------------------------
// name: "echo"
// version: "1.0"
// author: "Grame"
// license: "BSD"
// copyright: "(c)GRAME 2006"
//
// Code generated with Faust 0.9.58 (http://faust.grame.fr)
//-----------------------------------------------------
/* link with  */
#include <math.h>
/************************************************************************
 
 IMPORTANT NOTE : this file contains two clearly delimited sections :
 the ARCHITECTURE section (in two parts) and the USER section. Each section
 is governed by its own copyright and license. Please check individually
 each section for license and copyright information.
 *************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2013 Reza Payami
 All rights reserved.
 ----------------------------BSD License------------------------------
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above
 copyright notice, this list of conditions and the following
 disclaimer in the documentation and/or other materials provided
 with the distribution.
 * Neither the name of Remy Muller nor the names of its
 contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.
 
 ----------------------------Audio Unit SDK----------------------------------
 In order to compile an AU (TM) plugin with this architecture file
 you will need the proprietary AU SDK from Apple. Please check
 the corresponding license.
 
 ************************************************************************
 ************************************************************************/



#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <time.h>
//#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include <vector>


#include "AUEffectBase.h"
#include <AudioToolbox/AudioUnitUtilities.h>
#include "FaustAUVersion.h"
#include <math.h>

using namespace std ;

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

struct Meta
{
    void declare (const char* key, const char* value) {  }
};



// abs is now predefined
//template<typename T> T abs (T a)			{ return (a<T(0)) ? -a : a; }


inline int		lsr (int x, int n)			{ return int(((unsigned int)x) >> n); }

inline int int2pow2 (int x)	{ int r=0; while ((1<<r)<x) r++; return r; }


/******************************************************************************
 *******************************************************************************
 *
 *							       VECTOR INTRINSICS
 *
 *******************************************************************************
 *******************************************************************************/

//inline void *aligned_calloc(size_t nmemb, size_t size) { return (void*)((unsigned)(calloc((nmemb*size)+15,sizeof(char)))+15 & 0xfffffff0); }
//inline void *aligned_calloc(size_t nmemb, size_t size) { return (void*)((size_t)(calloc((nmemb*size)+15,sizeof(char)))+15 & ~15); }


/******************************************************************************
 *******************************************************************************
 *
 *								USER INTERFACE
 *
 *******************************************************************************
 *******************************************************************************/

class UI
{
    bool	fStopped;
    
public:
    
    UI() : fStopped(false) {}
    virtual ~UI() {}
    
    virtual void addButton(char* label, float* zone) = 0;
    virtual void addToggleButton(char* label, float* zone) = 0;
    virtual void addCheckButton(char* label, float* zone) = 0;
    virtual void addVerticalSlider(char* label, float* zone, float init, float min, float max, float step) = 0;
    virtual void addHorizontalSlider(char* label, float* zone, float init, float min, float max, float step) = 0;
    virtual void addNumEntry(char* label, float* zone, float init, float min, float max, float step) = 0;
	
	virtual void addNumDisplay(char* label, float* zone, int precision) = 0;
	virtual void addTextDisplay(char* label, float* zone, char* names[], float min, float max) = 0;
	virtual void addHorizontalBargraph(char* label, float* zone, float min, float max) = 0;
	virtual void addVerticalBargraph(char* label, float* zone, float min, float max) = 0;
    
    virtual void openFrameBox(char* label) = 0;
    virtual void openTabBox(char* label) = 0;
    virtual void openHorizontalBox(char* label) = 0;
    virtual void openVerticalBox(char* label) = 0;
    virtual void closeBox() = 0;
    
    virtual void run() {};
    
    void stop()		{ fStopped = true; }
    bool stopped() 	{ return fStopped; }
    
    virtual void declare(float* zone, const char* key, const char* value) {}
};


/******************************************************************************
 *******************************************************************************
 *
 *								FAUST DSP
 *
 *******************************************************************************
 *******************************************************************************/


class dsp {
    
protected:
    
    int fSamplingFreq;
    
public:
    
    dsp() {}
	virtual ~dsp() {}
    virtual int getNumInputs() 										= 0;
    virtual int getNumOutputs() 									= 0;
    virtual void buildUserInterface(UI* interface) 					= 0;
    virtual void init(int samplingRate) 							= 0;
    virtual void compute(int len, float** inputs, float** outputs) 	= 0;
};


/////////////////////////////////////////////////////////////////////////


/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

typedef long double quad;

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
};



/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/

/******************************************************************************
 *
 * AU wrapper
 *
 ******************************************************************************/

class auUI;


/*--------------------------------------------------------------------------*/
class auUIObject {
    //TODO protected:
public:
    string fLabel;
    float* fZone;
    
    float range(float min, float max, float val)
    {	// AU parameters are normalized in the range [0;1]
        val = min + val * (max - min);
        return (val < min) ? min : (val > max) ? max : val;
    }
    
public:
    auUIObject(char* label, float* zone):fLabel(label),fZone(zone) {}
    virtual ~auUIObject() {}
    
    virtual void  GetName(char *text){std::strcpy(text,fLabel.c_str());}
    virtual void  SetValue(double f) {*fZone = range(0.0f,1.0f,(float)f);}
    virtual float GetValue() {return *fZone;}
    virtual void  GetDisplay(char *text){std::sprintf(text,"%f",*fZone);}
    virtual long  GetID()
    {	/* returns the sum of all the ASCII characters  contained in the parameter's label */
        int i;
        long acc;
        for(i=0,acc = 0;i<fLabel.length();i++) acc += (fLabel.c_str())[i];
        return acc;
    }
};

/*--------------------------------------------------------------------------*/
class auToggleButton : public auUIObject {
	
public:
	
    auToggleButton(char* label, float* zone):auUIObject(label,zone) {}
    virtual ~auToggleButton() {}
    virtual float GetValue() {return *fZone;}
    virtual void SetValue(double f) {*fZone = (f>0.5f)?1.0f:0.0f;}
    virtual void  GetDisplay(char *text){(*fZone>0.5f)? std::strcpy(text,"ON"): std::strcpy(text,"OFF");}
};

/*--------------------------------------------------------------------------*/
class auCheckButton : public auUIObject {
	
public:
	
    auCheckButton(char* label, float* zone):auUIObject(label,zone) {}
    virtual ~auCheckButton() {}
    virtual float GetValue() {return *fZone;}
    virtual void SetValue(double f) {*fZone = (f>0.5f)?1.0f:0.0f;}
    virtual void  GetDisplay(char *text){(*fZone>0.5f)? std::strcpy(text,"ON"): std::strcpy(text,"OFF");}
};

/*--------------------------------------------------------------------------*/
class auButton : public auUIObject {
	
public:
	
    auButton(char* label, float* zone):auUIObject(label,zone) {}
    virtual ~auButton() {}
    virtual float GetValue() {return *fZone;}
    virtual void SetValue(double f) {*fZone = (f>0.5f)?1.0f:0.0f;}
    virtual void  GetDisplay(char *text){(*fZone>0.5f)? std::strcpy(text,"ON"): std::strcpy(text,"OFF");}
};

/*--------------------------------------------------------------------------*/
class auSlider : public auUIObject{
    
    //TODO private:
	
public:
    float fInit;
    float fMin;
    float fMax;
    float fStep;
	
public:
	
    auSlider(char* label, float* zone, float init, float min, float max, float step)
    :auUIObject(label,zone), fInit(init), fMin(min), fMax(max),fStep(step) {}
    virtual ~auSlider() {}
    
    virtual float GetValue() {return (*fZone-fMin)/(fMax-fMin);}	// normalize
    virtual void SetValue(double f) {*fZone = range(fMin,fMax,(float)f);} // expand
};

/*--------------------------------------------------------------------------*/
class auUI : public UI
{
    //TODO
public:
    //private:
	
    vector<auUIObject*> fUITable;
    
public:
    
    auUI(){}
    virtual ~auUI()
    {
        for (vector<auUIObject*>::iterator iter = fUITable.begin(); iter != fUITable.end(); iter++) delete *iter;
    }
    
    void addButton(char* label, float* zone) {fUITable.push_back(new auButton(label, zone));}
    
    void addToggleButton(char* label, float* zone) {fUITable.push_back(new auToggleButton(label, zone));}
    
    void addCheckButton(char* label, float* zone) {fUITable.push_back(new auCheckButton(label, zone));}
    
    void addVerticalSlider(char* label, float* zone, float init, float min, float max, float step)
    {
        fUITable.push_back(new auSlider(label, zone, init, min, max, step));
    }
    
    void addHorizontalSlider(char* label, float* zone, float init, float min, float max, float step)
    {
        fUITable.push_back(new auSlider(label, zone, init, min, max, step));
    }
    
    void addNumEntry(char* label, float* zone, float init, float min, float max, float step)
    {
        fUITable.push_back(new auSlider(label, zone, init, min, max, step));
    }
    
    void openFrameBox(char* label) {}
    void openTabBox(char* label) {}
    void openHorizontalBox(char* label) {}
    void openVerticalBox(char* label) {}
    void closeBox() {}
    
    void  SetValue(int index, double f) {assert(index<fUITable.size()); fUITable[index]->SetValue(f);}
    float GetValue(long index)	{assert(index<fUITable.size()); return fUITable[index]->GetValue();}
    void  GetDisplay(long index, char *text) {assert(index<fUITable.size()); fUITable[index]->GetDisplay(text);}
    void  GetName(long index, char *text) {assert(index<fUITable.size()); fUITable[index]->GetName(text);}
    long  GetNumParams() {return fUITable.size();}
    
    long  makeID()
    /* Creates a (unique?)id by summing all the parameter's labels,
     * then wrapping it in the range [0;maxNumberOfId] and adding
     * this number to the offset made by the Four Character ID: 'FAUS'
     */
    {
        const long maxNumberOfId = 128;
        long baseid = 'FAUS';
        long id=0;
        for(int i=0;i<fUITable.size();i++) id += fUITable[i]->GetID();
        return baseid + id % maxNumberOfId;
    }
    
	// To be implemented
	void addNumDisplay(char* label, float* zone, int precision){}
	void addTextDisplay(char* label, float* zone, char* names[], float min, float max){}
	void addHorizontalBargraph(char* label, float* zone, float min, float max){}
	void addVerticalBargraph(char* label, float* zone, float min, float max){}
};



/********************END ARCHITECTURE SECTION (part 2/2)****************/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____FaustKernel

//TODO
class Faust;

/*
 class FaustKernel : public AUKernelBase		// the actual DSP happens here
 {
 public:
 FaustKernel(AUEffectBase *inAudioUnit );
 virtual ~FaustKernel();
 
 // processes one channel of non-interleaved samples
 virtual void 		Process(	const Float32 	*inSourceP,
 Float32		 	*inDestP,
 UInt32 			inFramesToProcess,
 UInt32			inNumChannels,
 bool &			ioSilence);
 
 // resets the state
 virtual void		Reset();
 
 private:
 
 //TODO
 Faust* au;
 };
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Faust

#define MAX_OUT_CHANNELS 48

class Faust : public AUEffectBase
{
public:
	Faust(AudioUnit component);
    ~Faust();
    
    
    // virtual UInt32 SupportedNumChannels (const AUChannelInfo** outInfo);
    
	virtual OSStatus			Version() { return kFaustAUVersion; }
    
	virtual OSStatus			Initialize();
    
    //    virtual AUKernelBase *		NewKernel() { return new FaustKernel(this); }
    
	// for custom property
	virtual OSStatus			GetPropertyInfo(	AudioUnitPropertyID		inID,
                                                AudioUnitScope			inScope,
                                                AudioUnitElement		inElement,
                                                UInt32 &				outDataSize,
                                                Boolean	&				outWritable );
    
	virtual OSStatus			GetProperty(		AudioUnitPropertyID 	inID,
                                            AudioUnitScope 			inScope,
                                            AudioUnitElement 		inElement,
                                            void 					* outData );
    
    
	virtual OSStatus			GetParameterInfo(	AudioUnitScope			inScope,
                                                 AudioUnitParameterID	inParameterID,
                                                 AudioUnitParameterInfo	&outParameterInfo );
    
	void	SetParameter(			AudioUnitParameterID			paramID,
                         AudioUnitParameterValue			value);
    
	virtual OSStatus 	SetParameter(AudioUnitParameterID			inID,
                                     AudioUnitScope 				inScope,
                                     AudioUnitElement 				inElement,
                                     AudioUnitParameterValue		inValue,
                                     UInt32);
    
    
	
    // handle presets:
    virtual OSStatus			GetPresets(	CFArrayRef	*outData	)	const;
    virtual OSStatus			NewFactoryPresetSet (	const AUPreset & inNewFactoryPreset	);
    
	// we'll report a 1ms tail.   A reverb effect would have a much more substantial tail on
	// the order of several seconds....
	//
	virtual	bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return 3.0;}
    
	// we have no latency
	//
	// A lookahead compressor or FFT-based processor should report the true latency in seconds
    virtual Float64				GetLatency() {return 0.5;}
    
    // Constructor, other overridden methods, etc.
    virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags &ioActionFlags,
                                        const AudioBufferList &inBuffer,
                                        AudioBufferList &outBuffer,
                                        UInt32 inFramesToProcess);
    
    
    
    
private:
	auUI*	dspUI;
    
protected:
	float*					outBuffer[MAX_OUT_CHANNELS];
    
    //TODO
public:
    mydsp*	dsp;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Standard DSP AudioUnit implementation

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Faust)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Construction_Initialization


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Faust::Faust
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Faust::Faust(AudioUnit component)
: AUEffectBase(component)
{
    // all the parameters must be set to their initial values here
	//
	// these calls have the effect both of defining the parameters for the first time
	// and assigning their initial values
    
    //TODO
	
    /*	CreateElements();
     
     CAStreamBasicDescription streamDescIn;
     */
    //TODO
	dspUI = new auUI();
    
    dsp = new mydsp();
    
    //    int inChannels = dsp->getNumInputs();
    int outChannels = dsp->getNumOutputs();
    /*
     streamDescIn.SetCanonical(inChannels, false);	// number of input channels
     streamDescIn.mSampleRate = GetSampleRate();
     
     CAStreamBasicDescription streamDescOut;
     streamDescOut.SetCanonical(outChannels, false);	// number of output channels
     streamDescOut.mSampleRate = GetSampleRate();
     
     Inputs().GetIOElement(0)->SetStreamFormat(streamDescIn);
     Outputs().GetIOElement(0)->SetStreamFormat(streamDescOut);
     */
    
    
    //	UInt32 frames = GetMaxFramesPerSlice();
    //	for ( int i=0; i<outChannels; ++i )
    //		m_pBuffers[i] = new float[ frames ];
    
    
    // kFaustParam_ max value depends on sample-rate
	//SetParamHasSampleRateDependency(true );
    
    SetParamHasSampleRateDependency(true);
    
	UInt32 frames = GetMaxFramesPerSlice();
    
    for (int i = 0; i < outChannels; i++)
        outBuffer[i] = new float[frames];
    
    
	dsp->buildUserInterface(dspUI);
    //	dsp->init(long(GetSampleRate()));
    
    if (dspUI)
        for (int i = 0; i < dspUI->fUITable.size(); i++)
            if (dspUI->fUITable[i] && dspUI->fUITable[i]->fZone)
            {
                if (dynamic_cast<auButton*>(dspUI->fUITable[i])) {
                    //auButton* button = (auButton*)dspUI->fUITable[i];
                    SetParameter(i, 0);//TODO button->->fInit );
                }
                else if (dynamic_cast<auToggleButton*>(dspUI->fUITable[i])) {
                    //auToggleButton* button = (auToggleButton*)dspUI->fUITable[i];
                    SetParameter(i, 0);
                }
                else if (dynamic_cast<auCheckButton*>(dspUI->fUITable[i])) {
                    //auCheckButton* button = (auCheckButton*)dspUI->fUITable[i];
                    SetParameter(i, 0);
                }
                else {
                    auSlider* slider = (auSlider*)dspUI->fUITable[i];
                    SetParameter(i, slider->fInit );
                }
            }
    
}

Faust::~Faust()
{
    
    int outChannels = dsp->getNumOutputs();
    
    for (int i = 0; i < outChannels; i++)
        if (outBuffer[i])
            delete [] outBuffer[i];
    
}

/*
 UInt32 Faust::SupportedNumChannels (const AUChannelInfo** outInfo)
 {
 // set an array of arrays of different combinations of supported numbers
 // of ins and outs
 int inChannels = dsp->getNumInputs();
 int outChannels = dsp->getNumInputs();
 
 static const AUChannelInfo sChannels[1] = {{ inChannels, outChannels}};
 if (outInfo) *outInfo = sChannels;
 return sizeof (sChannels) / sizeof (AUChannelInfo);
 
 
 }
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Faust::Initialize
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Faust::Initialize()
{
	OSStatus result = AUEffectBase::Initialize();
	/*
     
     if(result == noErr )
     {
     // in case the AU was un-initialized and parameters were changed, the view can now
     // be made aware it needs to update the frequency response curve
     PropertyChanged(kAudioUnitCustomProperty_, kAudioUnitScope_Global, 0 );
     }
     */
	
    //TODO constructor?
    dsp->init(long(GetSampleRate()));
    
    return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Parameters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Faust::GetParameterInfo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Faust::GetParameterInfo(	AudioUnitScope			inScope,
                                            AudioUnitParameterID	inParameterID,
                                            AudioUnitParameterInfo	&outParameterInfo )
{
	OSStatus result = noErr;
    
    char name[100];
    CFStringRef str;
    
	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
    +		kAudioUnitParameterFlag_IsReadable;
    
	if (inScope == kAudioUnitScope_Global) {
        
        
        if (dspUI && dspUI->fUITable[inParameterID] && dspUI->fUITable[inParameterID]->fZone)
        {
            
            if (dynamic_cast<auButton*>(dspUI->fUITable[inParameterID])) {
                auToggleButton* toggle = (auToggleButton*)dspUI->fUITable[inParameterID];
                toggle->GetName(name);
                str = CFStringCreateWithCString(kCFAllocatorDefault, name, 0);
                
                AUBase::FillInParameterName(outParameterInfo, str, false);
                outParameterInfo.unit =  kAudioUnitParameterUnit_Boolean;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 1;
                outParameterInfo.defaultValue = 0;
            }
            else if (dynamic_cast<auToggleButton*>(dspUI->fUITable[inParameterID])) {
                auToggleButton* toggle = (auToggleButton*)dspUI->fUITable[inParameterID];
                toggle->GetName(name);
                
                str = CFStringCreateWithCString(kCFAllocatorDefault, name, 0);
                
                AUBase::FillInParameterName(outParameterInfo, str, false);
                outParameterInfo.unit =  kAudioUnitParameterUnit_Boolean;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 1;
                outParameterInfo.defaultValue = 0;
                
            }
            else if (dynamic_cast<auCheckButton*>(dspUI->fUITable[inParameterID])) {
                auCheckButton* check = (auCheckButton*)dspUI->fUITable[inParameterID];
                check->GetName(name);
                
                str = CFStringCreateWithCString(kCFAllocatorDefault, name, 0);
                
                AUBase::FillInParameterName(outParameterInfo, str, false);
                outParameterInfo.unit =  kAudioUnitParameterUnit_Boolean;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 1;
                outParameterInfo.defaultValue = 0;
                
            }
            else {
                
                auSlider* slider = (auSlider*)dspUI->fUITable[inParameterID];
                slider->GetName(name);
                str = CFStringCreateWithCString(kCFAllocatorDefault, name, 0);//CFSTR("Unicode"));//GetApplicationTextEncoding());
                
                AUBase::FillInParameterName(outParameterInfo, str, false);
                outParameterInfo.unit =  kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = slider->fMin;//kMinCutoffHz;
                outParameterInfo.maxValue = slider->fMax;//GetSampleRate() * 0.5;
                outParameterInfo.defaultValue = slider->fInit;
            }
            
        }
    } else {
        result = kAudioUnitErr_InvalidParameter;
    }
    
    return result;
}


void	Faust::SetParameter(			AudioUnitParameterID			paramID,                                         AudioUnitParameterValue			value)
{
    AUEffectBase::SetParameter(paramID, value);
}


OSStatus 	Faust::SetParameter(AudioUnitParameterID			inID,
                                AudioUnitScope 				inScope,
                                AudioUnitElement 				inElement,
                                AudioUnitParameterValue		inValue,
                                UInt32							inBufferOffsetInFrames)
{
    if (inScope == kAudioUnitScope_Global) {
        
        
        if (dspUI) {
            if (dspUI->fUITable[inID] && dspUI->fUITable[inID]->fZone)
                *(dspUI->fUITable[inID]->fZone) = (FAUSTFLOAT)inValue;
            
        
            if (dynamic_cast<auButton*>(dspUI->fUITable[inID]))
                return AUEffectBase::SetParameter(inID, inScope, inElement, 0, inBufferOffsetInFrames);


        }
    }
    
    return AUEffectBase::SetParameter(inID, inScope, inElement, inValue, inBufferOffsetInFrames);
    

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Properties

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Faust::GetPropertyInfo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Faust::GetPropertyInfo (	AudioUnitPropertyID				inID,
                                            AudioUnitScope					inScope,
                                            AudioUnitElement				inElement,
                                            UInt32 &						outDataSize,
                                            Boolean &						outWritable)
{
    return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Faust::GetProperty
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Faust::GetProperty (	AudioUnitPropertyID 		inID,
                                        AudioUnitScope 				inScope,
                                        AudioUnitElement			inElement,
                                        void *						outData)
{
    // if we've gotten this far, handles the standard properties
    return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Presets

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Faust::GetPresets
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Faust::GetPresets (		CFArrayRef * 		outData) const
{
    
    return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Faust::NewFactoryPresetSet
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus	Faust::NewFactoryPresetSet (const AUPreset & inNewFactoryPreset)
{
    return noErr;
}


///////////////////////////////

OSStatus Faust::ProcessBufferLists	(AudioUnitRenderActionFlags&	iFlags,
                                     const 	AudioBufferList& 	inBufferList,
                                     AudioBufferList&	outBufferList,
                                     UInt32			iFrames)
{
    
    int inChannels = dsp->getNumInputs();
    int outChannels = dsp->getNumOutputs();
    
    // from Digenis BProc plugin (http://www.digenis.co.uk/plugins.html)
    // Array of pointers, as many as the number of input signals.
    float* audioData[inChannels];
    
    // Pointers point to incomming audio buffers.
    for(int i = 0; i < inChannels; i++)
    {
        audioData[i] = (float*) inBufferList.mBuffers[i].mData;
    }
    
    dsp->compute(iFrames, audioData, outBuffer);
    
    //   outBufferList.mBuffers[0].mData = 0;
    
    for(int i = 0; i < outChannels; i++)
    {
        // as a test, copy audio from the first channel straight to all outputs
        outBufferList.mBuffers[i].mData = outBuffer[i];
    }
    return noErr;
}

/*
 OSStatus Faust::ProcessBufferLists(AudioUnitRenderActionFlags &ioActionFlags,
 const AudioBufferList &inBuffer,
 AudioBufferList &outBuffer,
 UInt32 inFramesToProcess)
 {
 if (dsp) {
 int channel;
 //       float **faustInBuffer = (float**)new float[inBuffer.mNumberBuffers * inFramesToProcess];
 //     float **faustOutBuffer = (float**)new float[outBuffer.mNumberBuffers * inFramesToProcess];
 
 //   for (channel = 0; channel < inBuffer.mNumberBuffers; channel++)
 //      faustInBuffer[channel] = (float*)inBuffer.mBuffers[channel].mData;
 
 //dsp->compute(inFramesToProcess, faustInBuffer, faustOutBuffer);
 dsp->compute(inFramesToProcess, (float**)inBuffer.mBuffers[0].mData, (float**)outBuffer.mBuffers[0].mData);
 
 for (channel = 0; channel < outBuffer.mNumberBuffers; channel++)
 for(UInt32 frame = 0; frame < inFramesToProcess; ++frame) {
 ((float*)(outBuffer.mBuffers[channel].mData))[frame] = faustOutBuffer[channel][frame];
 }
 
 //  delete[] faustInBuffer;
 //delete[] faustOutBuffer;
 }
 
 return noErr;
 }*/

///////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____FaustKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FaustKernel::FaustKernel()
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
 FaustKernel::FaustKernel(AUEffectBase *inAudioUnit )
 : AUKernelBase(inAudioUnit)
 {
 //TODO
 au = (Faust*)inAudioUnit;
 
 Reset();
 }
 
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //	FaustKernel::~FaustKernel()
 //
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 FaustKernel::~FaustKernel( )
 {
 }
 
 
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //	FaustKernel::Reset()
 //
 //		It's very important to fully reset all state variables to their
 //		initial settings here.  For delay/reverb effects, the delay buffers must
 //		also be cleared here.
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 void		FaustKernel::Reset()
 {
 //TODO
 }
 
 
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //	FaustKernel::Process(int inFramesToProcess)
 //
 //		We process one non-interleaved stream at a time
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 void FaustKernel::Process(	const Float32 	*inSourceP,
 Float32 		*inDestP,
 UInt32 			inFramesToProcess,
 UInt32			inNumChannels,	// for version 2 AudioUnits inNumChannels is always 1
 bool &			ioSilence)
 {
 //TODO
 au->dsp->compute(inFramesToProcess, (float**)&inSourceP, &inDestP);
 
 }
 
 */



