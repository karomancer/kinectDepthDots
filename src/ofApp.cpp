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
    offGridMultiplier.allocate(ofGetScreenWidth(), ofGetScreenHeight(), OF_IMAGE_GRAYSCALE);
    
    // Set up GUI panel
    guiPanel.setup("DEPTH_DOTS", "settings.json");
    
    minDepth.set("Min depth", 0.5f, 0.5f, 8.f);
    //    minDepth.addListener(this, &ofApp::listenForMinDepth);
    
    maxDepth.set("Max depth", 5.f, 0.5f, 8.f);
    //    maxDepth.addListener(this, &ofApp::listenForMaxDepth);
    
    xDensity.set("X Density", 2, 1, 10);
    yDensity.set("Y Density", 2, 1, 10);
    anchorDepth.set("Anchor Depth Offset", .75f, 0.f, 5.f);
    
    showAsGrid.set("Show as grid", true);
    showDepthMap.set("Show Kinect Depth Map", false);
    
    
    guiPanel.add(showDepthMap);
    guiPanel.add(minDepth);
    guiPanel.add(maxDepth);
    
    guiPanel.add(xDensity);
    guiPanel.add(yDensity);
    guiPanel.add(anchorDepth);
    guiPanel.add(showAsGrid);
    
    // Set up Kinect
    settings.enableRGB = false;
    settings.enableIR = false;
    settings.enableRGBRegistration = false;
    
    settings.config.MinDepth = minDepth;
    settings.config.MaxDepth = maxDepth;
    
    kinect.open(0, settings);
}

// No longer using listeners...closing and reopening with new settings is too expensive
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
        
        depthTex.draw(0, 0);
        
        // Get the point distance using the SDK function (in meters).
        float distAtMouse = kinect.getDistanceAt(ofGetMouseX(), ofGetMouseY());
        ofDrawBitmapStringHighlight(ofToString(distAtMouse, 3), ofGetMouseX(), ofGetMouseY() - 10);
        
        // Get the point depth using the texture directly (in millimeters).
        const ofFloatPixels& rawDepthPix = kinect.getRawDepthPixels();
        int depthAtMouse = rawDepthPix.getColor(ofGetMouseX(), ofGetMouseY()).r;
        ofDrawBitmapStringHighlight(ofToString(depthAtMouse), ofGetMouseX() + 16, ofGetMouseY() + 10);
        
    }
    else {
        ofSetColor(0);
        ofFill();
        
        float xMultiplier = (float) ofGetScreenWidth() / depthPixels.getWidth();
        float yMultiplier = (float) ofGetScreenHeight() / depthPixels.getHeight() + 0.05;
        
        cout << "xMultiplier: " << xMultiplier << endl;
        cout << "yMultiplier: " << yMultiplier << endl;
        
        for (int y = 0; y < depthPixels.getHeight(); y += yDensity) {
            for (int x = 0; x < depthPixels.getWidth(); x += xDensity) {
                float dist = kinect.getDistanceAt(x, y);
                float randomX = showAsGrid ? 0 : ofRandom(xMultiplier);
                float randomY = showAsGrid ? 0 : ofRandom(xMultiplier);
                if (dist > minDepth && dist < maxDepth) {
                    float radius = ofMap(dist, minDepth, maxDepth, anchorDepth * (xDensity + yDensity)/2, 0);
                    ofDrawCircle(x * xMultiplier + anchorDepth + randomX, y * yMultiplier + anchorDepth + randomY, radius);
                }
            }
        }
    }
    
    guiPanel.draw();
}
