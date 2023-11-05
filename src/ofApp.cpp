// ofApp.cpp
#include "ofApp.h"

void ofApp::setup()
{
    int width = ofGetScreenWidth();
    int height = ofGetScreenHeight();
    ofSetWindowShape(width, height);
    
    // Set up canvas
    ofBackground(255);
    
    // Set up GUI panel
    guiPanel.setup("DEPTH_DOTS", "settings.json");
    
    colorPalette.set("Color Palette", ofColor(0));
    fillColor = colorPalette;
    colorPalette.addListener(this, &ofApp::listenForColorChange);
    colorRaveParty.set("Color Rave Party", false);
    
    minDepth.set("Min depth", 0.5f, 0.5f, 8.f);
    maxDepth.set("Max depth", 5.f, 0.5f, 8.f);
    
    // Didn't end up using listeners, but leaving here anyway for posterity's sake
    // minDepth.addListener(this, &ofApp::listenForMinDepth);
    // maxDepth.addListener(this, &ofApp::listenForMaxDepth);
    
    xDensity.set("X Density", 2, 1, 10);
    yDensity.set("Y Density", 2, 1, 10);
    anchorDepth.set("Base pixel size", .75f, 0.f, 5.f);
    
    addTheJitters.set("Jitterbug", false);
    showDepthMap.set("Show Kinect Depth Map", false);
    
    guiPanel.add(showDepthMap);
    guiPanel.add(minDepth);
    guiPanel.add(maxDepth);
    
    guiPanel.add(xDensity);
    guiPanel.add(yDensity);
    guiPanel.add(anchorDepth);
    
    guiPanel.add(colorPalette);
    guiPanel.add(colorRaveParty);
    
    guiPanel.add(addTheJitters);
    
    // Set up Kinect
    settings.enableRGB = false;
    settings.enableIR = false;
    settings.enableRGBRegistration = false;
    
    settings.config.MinDepth = minDepth;
    settings.config.MaxDepth = maxDepth;
    
    kinect.open(0, settings);
}

// No longer using listeners...closing and reopening with new settings is too memory intensive
// DEPRECATED:
void ofApp::listenForMinDepth(float & min) {
    kinect.close();
    
    cout << "Changing min depth..." << endl;
    
    settings.config.MinDepth = min;
    kinect.open(0, settings);
}

// DEPRECATED:
void ofApp::listenForMaxDepth(float & max) {
    kinect.close();
    
    cout << "Changing max depth..." << endl;
    
    settings.config.MaxDepth = max;
    kinect.open(0, settings);
}

void ofApp::listenForColorChange(ofColor & newColor) {
    fillColor = newColor;    
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
        ofSetColor(255);
        ofFill();
        
        depthTex.draw(ofGetScreenWidth()/2 - depthPixels.getWidth()/2, ofGetScreenHeight()/2 - depthPixels.getHeight()/2);
        
        // Get the point distance using the SDK function (in meters).
        float distAtMouse = kinect.getDistanceAt(ofGetMouseX(), ofGetMouseY());
        ofDrawBitmapStringHighlight(ofToString(distAtMouse, 3), ofGetMouseX(), ofGetMouseY() - 10);
        
        // Get the point depth using the texture directly (in millimeters).
        const ofFloatPixels& rawDepthPix = kinect.getRawDepthPixels();
        int depthAtMouse = rawDepthPix.getColor(ofGetMouseX(), ofGetMouseY()).r;
        ofDrawBitmapStringHighlight(ofToString(depthAtMouse), ofGetMouseX() + 16, ofGetMouseY() + 10);
        
    }
    else {
        ofSetColor(fillColor);
        ofFill();
        
        float newHue = fillColor.getHue();
        float minHue = max(newHue - 40, 1.f);
        float maxHue = min(newHue + 40, 360.f);
        
        float maxNumber = xDensity > yDensity ? depthPixels.getWidth() : depthPixels.getHeight();
        float xMultiplier = (float) ofGetScreenWidth() / depthPixels.getWidth();
        float yMultiplier = (float) ofGetScreenHeight() / depthPixels.getHeight() + 0.05;
        
        if (xDensity == yDensity) {
            maxNumber = depthPixels.getWidth() + depthPixels.getHeight();
        }
        
        for (int y = 0; y < depthPixels.getHeight(); y += yDensity) {
            for (int x = 0; x < depthPixels.getWidth(); x += xDensity) {
                float dist = kinect.getDistanceAt(x, y);
                float randomX = addTheJitters ? ofRandom(xMultiplier) : 0;
                float randomY = addTheJitters ? ofRandom(yMultiplier) : 0;
                
                if (dist > minDepth && dist < maxDepth) {
                    float radius = ofMap(dist, minDepth, maxDepth, anchorDepth * (xDensity + yDensity)/2, 0);
                    ofDrawCircle(x * xMultiplier + anchorDepth + randomX, y * yMultiplier + anchorDepth + randomY, radius);
                    
                    if (colorRaveParty) {
                        int dominantIdx = xDensity > yDensity ? x : y;
                        
                        if (xDensity == yDensity) {
                            // Gradient is diagonal if the x and y density is equal
                            dominantIdx = x + y;
                        }
                        
                        newHue = ofMap(dominantIdx, 0, maxNumber, minHue, maxHue);
                        float newSaturation = ofMap(dist, minDepth, maxDepth, 255, 40);
                        ofSetColor(ofColor::fromHsb(newHue, newSaturation, newSaturation));
                        ofFill();
                    }
                }
            }
        }
        
        // TODO: Rotate hue for a nice little rave party
        if (colorRaveParty) {
            // newHue = fillColor.getHue() + 0.1f;
            // fillColor = ofColor::fro/mHsb(newHue, 255, 255);
        }
    }
    
    guiPanel.draw();
}
