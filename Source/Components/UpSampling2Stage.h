//
//  UpSampling2Stage.h
//
//
//  Created by Eric Tarr on 4/19/19.
//
//

#include "AntiAliasFilter.h"

#pragma once

// Class definition
class UpSampling2Stage
{

public:
    UpSampling2Stage (){
        upSamplingFactor = 0;
		quality = 0;
        tempBuffer[0] = 0.0f;
        tempBuffer[1] = 0.0f;
        tempBuffer2[0] = 0.0f;
        tempBuffer2[1] = 0.0f;
        tempBuffer2[2] = 0.0f;
        tempBuffer2[3] = 0.0f;
    };
    ~UpSampling2Stage (){};
	
    // Process an entire buffer of samples
    void process(const float* input, float* output, int c, int iNumSamples)
    {
        // Note: c = channel. Because of filtering, it is necessary to keep track of
        // previous samples on both the left and the right.
        
        float x; // Holding variable to keep track of a sample method
        
        float * temp1;
        float * temp2;
        temp1 = tempBuffer;
        temp2 = tempBuffer2;
        
        for (int s = 0; s < iNumSamples; ++s)
        {
            if (upSamplingFactor == 1){
                // If upSamplingFactor == 1, there is no need to filter
                //*output = *(input++);
                //++output;
                return;
            }
            else if(upSamplingFactor == 2){
                // When upSamplingFactor == 2, then it is necessary to filter
                x = *(input++);
                for (unsigned int i = 0; i < 2 ; ++i){
                    *output = antiAliasFilter1.process(x,c);
                    ++output;
                }
            }
            else if(upSamplingFactor == 4) { // upsampling by 4x == upsampling twice by a factor of 2
                x = *(input++);
				for (int i = 0; i < 2 ; ++i){
                    //tempBuffer[i] = antiAliasFilter1.process(x,c);
                    *temp1 = antiAliasFilter1.process(x,c);
                    ++temp1;
                }
                temp1 = tempBuffer;
                for (int i = 0; i < 2 ; ++i){
                    //x = tempBuffer[i];
                    x = *(temp1++);
                    for (unsigned int j = 0; j < 2 ; ++j){
                        *output = antiAliasFilter2.process(x,c);
                        ++output;
                    }
                }
                temp1 = tempBuffer;
            }
            else{ // upsampling by 8x == upsampling three times by a factor of 2
                x = *(input++);
                for (int i = 0; i < 2 ; ++i){
                    //tempBuffer[i] = antiAliasFilter1.process(x,c);
                    *temp1 = antiAliasFilter1.process(x,c);
                    ++temp1;
                }
                temp1 = tempBuffer;
				for (int i = 0; i < 2 ; ++i){
                    //x = tempBuffer[i];
                    x = *(temp1++);
                    for (int j = 0; j < 2 ; ++j){
                        //tempBuffer2[2*i + j] = antiAliasFilter2.process(x,c);
                        *temp2 = antiAliasFilter2.process(x,c);
                        ++temp2;
                    }
                }
                temp1 = tempBuffer;
                temp2 = tempBuffer2;
                for (int i = 0; i < 2 ; ++i){
					for (int j = 0; j < 2 ; ++j){
                        //x = tempBuffer2[2*i + j];
                        x = *(temp2++);
						for (int k = 0; k < 2 ; ++k){
                            *output = antiAliasFilter3.process(x,c);
                            ++output;
                        }
                    }
                }
                temp2 = tempBuffer2;
            }
        }
    }
    
	void prepare(int newUpSamplingFactor, int newQuality) {

		if (upSamplingFactor != newUpSamplingFactor) {
			upSamplingFactor = newUpSamplingFactor; // 1,2,4,or 8

			antiAliasFilter1.clearDelayBuffers();
			antiAliasFilter2.clearDelayBuffers();
			antiAliasFilter3.clearDelayBuffers();
		}

		if (quality != newQuality) {
			quality = newQuality;
			// Different levels of CPU load
			antiAliasFilter1.setQuality(quality);
			antiAliasFilter2.setQuality(quality);
			antiAliasFilter3.setQuality(quality);

		}
	}
    
private:
    
    int upSamplingFactor; // Must be initialized to "1" to pass AU Plug-in Validation
	int quality;

    AntiAliasFilter antiAliasFilter1;
    AntiAliasFilter antiAliasFilter2;
    AntiAliasFilter antiAliasFilter3;
    
    float tempBuffer[2];
    float tempBuffer2[4];
};

