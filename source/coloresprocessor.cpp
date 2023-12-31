//------------------------------------------------------------------------
// Copyright(c) 2023 Dsus.
//------------------------------------------------------------------------

#include "coloresprocessor.h"
#include "colorescids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace Dsus {
//------------------------------------------------------------------------
// ColoresProcessor
//------------------------------------------------------------------------
ColoresProcessor::ColoresProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kColoresControllerUID);
}

//------------------------------------------------------------------------
ColoresProcessor::~ColoresProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------
    
    if (data.inputParameterChanges) {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
        for (int32 index = 0; index < numParamsChanged; index++) {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData(index)) {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount();
                switch (paramQueue->getParameterId ()) {
                    // handle different param change by tag defined in cid.h
                    case ColoresParams::kDryWetId:
                        if (paramQueue->getPoint(numPoints-1, sampleOffset, value) == kResultTrue) { // get the last point in the queue (there can be multiple change points in the same audio processing block)
                            mDryWet = value;
                        }
                        break;
                }
            }
        }
    }
	
	//--- Here you have to implement your processing
    if (data.numSamples > 0) {
        // TODO: a simple notch filter
    }
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ColoresProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Dsus
