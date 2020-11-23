#include "ofApp.h"

vector<Circuit> circuits;
int _c = 0;
float toneHue = ofRandom(255) + 255;
float toneRange = ofRandom(80);
float turnChance = ofRandom(0.01, 0.2);
int numTurns = (int)ofRandom(2,5) * 2;

//--------------------------------------------------------------
void ofApp::setup(){
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0;i<circuits.size();i++)
    {
        circuits[i].update(&circuits);
    }
    //add new circuits
    keyPressed(' ');
    
    //clear canvas
    if(ofGetFrameNum() % 200 == 0)
    {
        keyPressed('1');
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    for(int i=0;i<circuits.size();i++)
    {
        circuits[i].draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //add a group of circuit line with similar origin and color
    if(key == ' ')
    {
        for(int j=0;j<5;j++)
        {
            ofVec3f origin = ofVec3f(ofRandom(1) * ofGetWidth(), ofRandom(1) * ofGetHeight(), 0);
            int num = ofRandom(1, 5);
            float radius = ofRandom(10, 20);
            int direction = ofRandom(numTurns);
            ofColor color;
            float hue = ofRandom(-toneRange,toneRange) + toneHue;
            if(hue > 255) hue -= 255;
            if(hue < 0) hue += 255;
            color.setHsb(hue, 255,ofRandom(100,255));
            
            for(int i=0;i<num;i++)
            {
                float angle = (i / (float)num * radius);
                ofVec3f offset = ofVec3f(cos(angle), sin(angle)) * radius;
                Circuit c = Circuit(origin + offset, direction, color, turnChance, numTurns);
                c.id = ++_c;
                circuits.push_back(c);
            }
        }
    }
    
    //clear canvas, generate new color palette and turning parameters
    if(key == '1')
    {
        circuits.clear();
        toneHue = ofRandom(255);
        toneRange = ofRandom(80);
        turnChance = ofRandom(0.01, 0.2);
        numTurns = (int)ofRandom(2,5) * 2;
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
