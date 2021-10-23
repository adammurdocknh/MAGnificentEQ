//
//  AntiAliasFilter.h
//
//
//  Created by Eric Tarr on 4/19/19.
//
//

#pragma once

// Class definition
class AntiAliasFilter
{

public:
    AntiAliasFilter (){  // Constructor
        quality = 3;
        setFilterCoefficients();
        clearDelayBuffers();
    };
    
    ~AntiAliasFilter (){}; // Destructor
	
    // Process a single sample
    float process(float x,int c)
    {
        // Note: c = channel. Because of delay in filtering, it is necessary to keep track of
        // previous samples on both the left and the right.
    
        if (quality == 1){
            y = x*b0 + x1[c]*b1+ x2[c]*b2 + x3[c]*b3 + x4[c]*b4 - y1[c]*a1 - y2[c]*a2 - y3[c]*a3 - y4[c]*a4;
            
            // Update delay variables
            x4[c]=x3[c]; x3[c]=x2[c]; x2[c]=x1[c]; x1[c]=x;
            y4[c]=y3[c]; y3[c]=y2[c]; y2[c]=y1[c]; y1[c]=y;
        }
        else if(quality == 2){

            y = x*b0 + x1[c]*b1+ x2[c]*b2 + x3[c]*b3 + x4[c]*b4 + x5[c]*b5 +x6[c]*b6 - y1[c]*a1 - y2[c]*a2 - y3[c]*a3 - y4[c]*a4 - y5[c]*a5 - y6[c]*a6;
            
            // Update delay variables
            x6[c]=x5[c]; x5[c]=x4[c]; x4[c]=x3[c];
            x3[c]=x2[c]; x2[c]=x1[c]; x1[c]=x;
            
            y6[c]=y5[c]; y5[c]=y4[c]; y4[c]=y3[c];
            y3[c]=y2[c]; y2[c]=y1[c]; y1[c]=y;
        }
        else{

            y = x*b0 + x1[c]*b1+ x2[c]*b2 + x3[c]*b3 + x4[c]*b4 + x5[c]*b5 +x6[c]*b6 + x7[c]*b7 + x8[c]*b8 - y1[c]*a1 - y2[c]*a2 - y3[c]*a3 - y4[c]*a4 - y5[c]*a5 - y6[c]*a6 - y7[c]*a7 - y8[c]*a8;
            
            // Update delay variables
            x8[c]=x7[c]; x7[c]=x6[c]; x6[c]=x5[c]; x5[c]=x4[c];
            x4[c]=x3[c]; x3[c]=x2[c]; x2[c]=x1[c]; x1[c]=x;
            
            y8[c]=y7[c]; y7[c]=y6[c]; y6[c]=y5[c]; y5[c]=y4[c];
            y4[c]=y3[c]; y3[c]=y2[c]; y2[c]=y1[c]; y1[c]=y;
            
        }
        
        return y;
        
    }
    
    void clearDelayBuffers(){
        for (int i=0; i<2; ++i){
            x1[i]=0.0f;x2[i]=0.0f;x3[i]=0.0f;x4[i]=0.0f;x5[i]=0.0f;x6[i]=0.0f;x7[i]=0.0f;x8[i]=0.0f;
            y1[i]=0.0f;y2[i]=0.0f;y3[i]=0.0f;y4[i]=0.0f;y5[i]=0.0f;y6[i]=0.0f;y7[i]=0.0f;y8[i]=0.0f;
        }
    }
    
    void setQuality(int newQuality){
        quality = newQuality;
        setFilterCoefficients();
    }
    
    
    
private:
    
    float y; // Holding variable to keep track of a sample during "process" method
    
    // Initialize Delay Samples
    float x1[2], x2[2], x3[2], x4[2];
    float x5[2], x6[2], x7[2], x8[2];// Feed-forward
    float y1[2], y2[2], y3[2], y4[2];
    float y5[2], y6[2], y7[2], y8[2]; // Feed-back
    
    int quality;// = 1; // 1 = Low CPU, 2 = Medium, 3 = High (Best quality, most aliasing rejection)
    
    // Initialize Filter Coefficients
    // [b,a] = ellip(8,.1,75,0.44);
    float b0; // = 0.009176194979831f;
    float b1; // = 0.032293605291588f;
    float b2; // = 0.072321474176209f;
    float b3; // = 0.111071974378409f;
    float b4; // = 0.128028090063212f;
    float b5; // = 0.111071974378409f;
    float b6; // = 0.072321474176209f;
    float b7; // = 0.032293605291588f;
    float b8; // = 0.009176194979831f;
    
    float a0; // = 1.f;
    float a1; // = -2.393266775477970f;
    float a2; // = 4.504354760061331f;
    float a3; // = -5.395896763059963f;
    float a4; // = 5.044451609399958f;
    float a5; // = -3.420032037299648f;
    float a6; // = 1.710984420398763f;
    float a7; // = -0.567182397048739f;
    float a8; // = 0.101031853563449f;
    
    void setFilterCoefficients(){
        if (quality == 1){
                
            b0 = 0.071242096945448f;
            b1 = 0.268611817577063f;
            b2 = 0.395227505580429f;
            b3 = 0.268611817577063f;
            b4 = 0.071242096945448f;
            
            a0 = 1.f;
            a1 = -0.530422312749419f;
            a2 = 0.821030572817477f;
            a3 = -0.298180000938177f;
            a4 = 0.094954240035216f;
                
        }
        else if (quality == 2){
            b0 = 0.018234095485930f;
            b1 = 0.075590761797346f;
            b2 = 0.155037586536106f;
            b3 = 0.193859352283532f;
            b4 = 0.155037586536106f;
            b5 = 0.075590761797346f;
            b6 = 0.018234095485930f;
            
            a0 = 1.f;
            a1 = -1.535263128954549f;
            a2 = 2.285382076829951f;
            a3 = -1.875764904940776f;
            a4 = 1.190865622517742f;
            a5 = -0.463731350831024f;
            a6 = 0.098104093374003f;
        }
        else{
            b0 = 0.009176194979831f;
            b1 = 0.032293605291588f;
            b2 = 0.072321474176209f;
            b3 = 0.111071974378409f;
            b4 = 0.128028090063212f;
            b5 = 0.111071974378409f;
            b6 = 0.072321474176209f;
            b7 = 0.032293605291588f;
            b8 = 0.009176194979831f;
            
            a0 = 1.f;
            a1 = -2.393266775477970f;
            a2 = 4.504354760061331f;
            a3 = -5.395896763059963f;
            a4 = 5.044451609399958f;
            a5 = -3.420032037299648f;
            a6 = 1.710984420398763f;
            a7 = -0.567182397048739f;
            a8 = 0.101031853563449f;
        }
    }
};


