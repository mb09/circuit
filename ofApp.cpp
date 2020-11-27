#include "ofApp.h"

vector<Circuit> circuits;
int _c = 0;
float toneHue = ofRandom(255) + 255;
float toneRange = ofRandom(80);
float turnChance = ofRandom(0.01, 0.2);
int numTurns = (int)ofRandom(2,5) * 2;
int numIterations = 0;
float offsetScale = 0;

//--------------------------------------------------------------
void ofApp::setup(){
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0;i<circuits.size();i++)
    {
        circuits[i].update(&circuits);
    }
    
    if(numIterations < 50)
    {
        //add new circuits
        keyPressed(' ');
        numIterations ++;
    }
    
//    //clear canvas
    if(ofGetFrameNum() % 100 == 0)
    {
        keyPressed('1');
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for(int i=0;i<circuits.size();i++)
    {
        circuits[i].draw();
    }
    
    ofImage cap;
    cap.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    cap.save("output/"+ofToString(ofGetFrameNum()) + ".png");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //add a group of circuit line with similar origin and color
    if(key == ' ')
    {
        for(int j=0;j<ofRandom(5,30);j++)
        {
            ofVec3f origin = ofVec3f(ofRandom(1) * ofGetWidth(), ofRandom(1) * ofGetHeight(), 0);
            int num = ofRandom(1, 5);
            float radius = ofRandom(10, 20);
            int direction = ofRandom(numTurns);
            ofColor color;
            float hue = ofRandom(-toneRange,toneRange) + toneHue;
            if(hue > 255) hue -= 255;
            if(hue < 0) hue += 255;
            color.setHsb(hue, 255,ofRandom(80,130));
            
            for(int i=0;i<num;i++)
            {
                float angle = (i / (float)num * radius);
                ofVec3f offset = ofVec3f(cos(angle), sin(angle)) * radius;
                Circuit c = Circuit(origin + offset, direction, color, turnChance, numTurns, offsetScale);
                c.id = ++_c;
                circuits.push_back(c);
            }
        }
    }
    
    //clear canvas, generate new color palette and turning parameters
    if(key == '1')
    {
        numIterations = 0;
        circuits.clear();
        toneHue = ofRandom(255);
        toneRange = ofRandom(80);
        turnChance = ofRandom(0.01, 0.2);
        numTurns = (int)ofRandom(2,5) * 2;
//        turnChance = 0.25;
//        numTurns = 10;
        offsetScale = pow(2,ofRandom(3));
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
