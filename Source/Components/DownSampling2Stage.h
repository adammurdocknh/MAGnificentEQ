//
//  DownSampling2Stage.h
//
//
//  Created by Eric Tarr on 4/19/19.
//
//

#include "AntiAliasFilter.h"

#pragma once


// Class definition
class DownSampling2Stage
{

public:
    DownSampling2Stage (){
        downSamplingFactor = 0;
		quality = 0;
        tempBuffer[0] = 0.0f;
        tempBuffer[1] = 0.0f;
        tempBuffer2[0] = 0.0f;
        tempBuffer2[1] = 0.0f;
        tempBuffer2[2] = 0.0f;
        tempBuffer2[3] = 0.0f;
    };
    ~DownSampling2Stage (){};
	
    // Process an entire buffer of samples
    void process(const float* input, float* output, int c, int iNumSamples)
    {
        // Note: c = channel
        float x;
        
        float * temp1;
        float * temp2;
        temp1 = tempBuffer;
        temp2 = tempBuffer2;
        
        for (int s = 0; s < iNumSamples; ++s)
        {
            if (downSamplingFactor == 1){
                // If upSamplingFactor == 1, there is no need to filter
                //*output = *(input++);
                //++output;
                return;
            }
            else if (downSamplingFactor == 2){
				for (int i = 0; i < 2; ++i){
                    x = antiAliasFilter1.process(*(input++),c);
                    
                    // Only use 1 out of every 2 samples in the output signal
                    if (i == 0){
                        *output = x; // Decimation
                        ++output;
                    }
                }
            }
            else if (downSamplingFactor == 4){
				for (int i = 0; i < 4 ; ++i){
                    // Stage 1: 4x->2x
                    x = antiAliasFilter1.process(*(input++),c);
                    
                    // Only use every other sample in the tempBuffer (stage 1 of down-sampling)
                    if (i == 0){
                        //tempBuffer[0] = x; // Decimation
                        *temp1 = x;
                        ++temp1;
                    }
                    if (i == 2){
                        //tempBuffer[1] = x; // Decimation
                        *temp1 = x;
                    }
                }
                temp1 = tempBuffer;
                for (int j = 0; j < 2 ; ++j){
                    // Stage 2: 2x->1x
                    //x = antiAliasFilter2.process(tempBuffer[j],c);
                    x = antiAliasFilter2.process(*(temp1++),c);
                    
                    // Only use 1 out of every 2 samples in the output signal
                    if (j == 0){
                        *output = x; // Decimation
                        ++output;
                    }
                }
                temp1 = tempBuffer;
            }
            else{
                for (int i = 0; i < 8 ; ++i){
                    // Stage 1: 8x->4x
                    x = antiAliasFilter1.process(*(input++),c);
                    
                    // Only use every other sample in the tempBuffer (stage 1 of down-sampling)
                    if (i == 0){
                        //tempBuffer2[0] = x; // Decimation
                        *temp2 = x;
                        ++temp2;
                    }
                    if (i == 2){
                        //tempBuffer2[1] = x; // Decimation
                        *temp2 = x;
                        ++temp2;
                    }
                    if (i == 4){
                        //tempBuffer2[2] = x; // Decimation
                        *temp2 = x;
                        ++temp2;
                    }
                    if (i == 6){
                        //tempBuffer2[3] = x; // Decimation
                        *temp2 = x;
                    }
                }
                temp2 = tempBuffer2;
                for (int i = 0; i < 4 ; ++i){
                    // Stage 2: 4x->2x
                    //x = antiAliasFilter2.process(tempBuffer2[i],c);
                    x = antiAliasFilter2.process(*(temp2++),c);
                    
                    // Only use every other sample in the tempBuffer (stage 2 of down-sampling)
                    if (i == 0){
                        //tempBuffer[0] = x; // Decimation
                        *temp1 = x;
                        ++temp1;
                    }
                    if (i == 2){
                        //tempBuffer[1] = x; // Decimation
                        *temp1 = x;
                    }
                }
                temp1 = tempBuffer;
                temp2 = tempBuffer2;
                for (int j = 0; j < 2 ; ++j){
                    // Stage 3: 2x->1x
                    //x = antiAliasFilter3.process(tempBuffer[j],c);
                    x = antiAliasFilter3.process(*(temp1++),c);
                    
                    // Only use 1 out of every 2 samples in the output signal
                    if (j == 0){
                        *output = x; // Decimation
                        ++output;
                    }
                }
                temp1 = tempBuffer;
            }
        }
   }
    
	void prepare(int newDownSamplingFactor, int newQuality) {

		if (downSamplingFactor != newDownSamplingFactor) {
			downSamplingFactor = newDownSamplingFactor;

			antiAliasFilter1.clearDelayBuffers();
			antiAliasFilter2.clearDelayBuffers();
			antiAliasFilter3.clearDelayBuffers();
		}

		if (quality != newQuality) {
			quality = newQuality;
			antiAliasFilter1.setQuality(quality);
			antiAliasFilter2.setQuality(quality);
			antiAliasFilter3.setQuality(quality);
		}
    }
    
private:
    
    int downSamplingFactor;  // Must be initialized to "1" to pass AU Plug-in Validation
	int quality;
    
    AntiAliasFilter antiAliasFilter1;
    AntiAliasFilter antiAliasFilter2;
    AntiAliasFilter antiAliasFilter3;
    
    float tempBuffer[2];
    float tempBuffer2[4];
};

