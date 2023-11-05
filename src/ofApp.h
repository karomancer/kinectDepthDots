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
    void listenForColorChange(ofColor & newColor);
        
    ofxKinectV2 kinect;
    ofxKinectV2::Settings settings;
    ofTexture depthTex;
    
    ofPixels depthPixels;    
    
    ofParameter<float> minDepth;
    ofParameter<float> maxDepth;
    ofParameter<short> xDensity;
    ofParameter<short> yDensity;
    ofParameter<float> anchorDepth;
    
    ofParameter<ofColor> colorPalette;
    ofParameter<bool> colorRaveParty;
    
    ofParameter<bool> addTheJitters;
    ofParameter<bool> showDepthMap;
    
    ofxPanel guiPanel;
    ofColor fillColor;
    ofColor maxFillColor;
};
