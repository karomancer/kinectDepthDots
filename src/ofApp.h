// ofApp.h
#pragma once

#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void listenForMinDepth(float & min);
    void listenForMaxDepth(float & max);
        
    ofxKinectV2 kinect;
    ofxKinectV2::Settings settings;
    ofTexture depthTex;
    
    ofImage renderImage;
    ofPixels depthPixels;    
    
    ofParameter<float> minDepth;
    ofParameter<float> maxDepth;
    ofParameter<bool> showDepthMap;
    
    ofxPanel guiPanel;
};
