// ofApp.cpp
#include "ofApp.h"

void ofApp::setup()
{
    int width = 1024;
    int height = 720;
    ofSetWindowShape(width, height);
    
    // Set up Kinect
    ofxKinectV2::Settings settings;
    settings.enableRGB = true;
    settings.enableDepth = true;
    settings.config.MinDepth = 0.5;
    settings.config.MaxDepth = 8.0;
    
    kinect.open(0, settings);
    
    // Set up rendered image
    renderImage.allocate(width, height, OF_IMAGE_COLOR);
}

void ofApp::update()
{
    kinect.update();
    
    // Only load the data if there is a new frame to process.
    if (kinect.isFrameNew())
    {
        depthTex.loadData(kinect.getDepthPixels());
    }
}

void ofApp::draw()
{
    depthTex.draw(0, 0);
    
    // Get the point distance using the SDK function (in meters).
    float distAtMouse = kinect.getDistanceAt(ofGetMouseX(), ofGetMouseY());
    ofDrawBitmapStringHighlight(ofToString(distAtMouse, 3), ofGetMouseX(), ofGetMouseY() - 10);
    
    // Get the point depth using the texture directly (in millimeters).
    const ofFloatPixels& rawDepthPix = kinect.getRawDepthPixels();
    int depthAtMouse = rawDepthPix.getColor(ofGetMouseX(), ofGetMouseY()).r;
    ofDrawBitmapStringHighlight(ofToString(depthAtMouse), ofGetMouseX() + 16, ofGetMouseY() + 10);
}
