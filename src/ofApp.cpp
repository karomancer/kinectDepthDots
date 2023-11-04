// ofApp.cpp
#include "ofApp.h"

void ofApp::setup()
{
    int width = ofGetScreenWidth();
    int height = ofGetScreenHeight();
    ofSetWindowShape(width, height);
    
    // Set up canvas
    ofBackground(255);
    
    // Set up rendered image
    renderImage.allocate(ofGetScreenWidth(), ofGetScreenHeight(), OF_IMAGE_GRAYSCALE);
    
    // Set up GUI panel
    guiPanel.setup("DEPTH_DOTS", "settings.json");
    
    minDepth.set("Min depth", 0.5f, 0.5f, 8.f);
    minDepth.addListener(this, &ofApp::listenForMinDepth);
    
    maxDepth.set("Max Area", 8.f, 0.5f, 8.f);
    minDepth.addListener(this, &ofApp::listenForMaxDepth);
    
    showDepthMap.set("Show Kinect Depth Map", true);
    
    
    guiPanel.add(minDepth);
    guiPanel.add(maxDepth);
    guiPanel.add(showDepthMap);
    
    // Set up Kinect    
    settings.enableRGB = true;
    settings.enableDepth = true;
    settings.config.MinDepth = minDepth;
    settings.config.MaxDepth = maxDepth;
    
    kinect.open(0, settings);
}

void ofApp::listenForMinDepth(float & min) {
    kinect.close();
    
    settings.config.MinDepth = min;
    kinect.open(0, settings);
}

void ofApp::listenForMaxDepth(float & max) {
    kinect.close();
    
    settings.config.MaxDepth = max;
    kinect.open(0, settings);
}

void ofApp::update()
{
    kinect.update();
    
    // Only load the data if there is a new frame to process.
    if (kinect.isFrameNew())
    {
        depthPixels = kinect.getDepthPixels();
        depthTex.loadData(depthPixels);
    }
}

void ofApp::draw()
{
    
    if (showDepthMap) {
        depthTex.draw(0, 0);
        
        // Get the point distance using the SDK function (in meters).
        float distAtMouse = kinect.getDistanceAt(ofGetMouseX(), ofGetMouseY());
        ofDrawBitmapStringHighlight(ofToString(distAtMouse, 3), ofGetMouseX(), ofGetMouseY() - 10);
        
        // Get the point depth using the texture directly (in millimeters).
        const ofFloatPixels& rawDepthPix = kinect.getRawDepthPixels();
        int depthAtMouse = rawDepthPix.getColor(ofGetMouseX(), ofGetMouseY()).r;
        ofDrawBitmapStringHighlight(ofToString(depthAtMouse), ofGetMouseX() + 16, ofGetMouseY() + 10);
        
    } else {
        ofSetColor(0);
        ofFill();
        
        for (int y = 0; y < depthPixels.getHeight(); y++) {
            for (int x = 0; x < depthPixels.getWidth(); x++) {
                float dist = kinect.getDistanceAt(x, y);
                if (dist > 0) {
                    ofDrawCircle(x * 3, y * 3, dist);
                }
    //            depthPixels[]

            }
        }
    }
    
    guiPanel.draw();
}
