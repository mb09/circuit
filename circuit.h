//
//  circuit.h
//  cyber_1
//
//  Created by mb9 on 23/11/2020.
//

#ifndef circuit_h
#define circuit_h

class Segment{
public:
    float progress;
    float length;
    float speed;
    ofColor color;
    ofVec3f offset;
    
    Segment(float offsetScale){
        progress = ofRandom(0, 1);
        length = ofRandom(0.03, 0.2);
        speed = ofRandom(0.001, 0.005);
        if(ofRandom(1)< 0.5) speed *= -1;
        
        
        offset = ofVec3f(ofRandom(-offsetScale,offsetScale), ofRandom(-offsetScale,offsetScale));
        
    }
    
    void update(){
        progress += speed;
        if(progress > 1) progress -= 1;
        else if(progress < 0) progress += 1;
    }
};

class Circuit{
public:
    vector<ofVec3f> pts;
    int direction;
    float speed;
    int id;
    bool isDead;
    ofColor color;
    float turnChance;
    int numTurns;
    int lineWidth;
    float offsetScale;
    
    float sumLength;
    vector<float>checkPts;
    
    vector<Segment> segments;
    
    Circuit(ofVec3f _head, int _direction, ofColor _color, float _turnChance, int _numTurns, float _offsetScale){
        color = _color;
        turnChance = _turnChance;
        numTurns = _numTurns;
        direction = _direction;
        pts.push_back(ofVec3f(_head));
        pts.push_back(ofVec3f(_head));
        speed = ofRandom(10, 25);
        isDead = false;
        lineWidth = pow(1.3,ofRandom(1,4) * ofRandom(1,5));
        
        offsetScale = _offsetScale;
        
        sumLength = 0;
        
        for(int i=0;i<ofRandom(5,30);i++)
        {
            Segment s(offsetScale);
            s.color = _color;
            s.color.setBrightness(ofRandom(120,180));
            s.color.setHue(s.color.getHue() + ofRandom(-15,15));
            segments.push_back(s);
            
        }
    }
    
    void update(vector<Circuit> *circuits){
        for(int i=0;i<segments.size();i++)
        {
            segments[i].update();
        }
        if(!isDead)
        {
            //calculate the next position
            ofVec3f move = ofVec3f(cos(direction * TWO_PI/numTurns),sin(direction * TWO_PI/numTurns)) * speed;
            ofVec3f newHead = pts[pts.size()-1] + move;
            
            //look for collision with all other circuits
            for(int i=0;i<circuits->size();i++)
            {
                Circuit c = circuits->at(i);
                if(c.id == id)
                {
                    continue;
                }
                else
                {
                    for(int j=1;j<c.pts.size();j++)
                    {
                        //dead if collide with other circuits
                        if(doIntersect(pts[pts.size()-1], newHead, c.pts[j-1], c.pts[j]) && ofRandom(1) < 0.8)
                        {
                            isDead = true;
                            break;
                        }
                    }
                }
            }
            if(!isDead)
            {
                //dead if out of bound
                if(newHead.x < 0 || newHead.y < 0 || newHead.x > ofGetWidth() || newHead.y > ofGetHeight())
                {
                    isDead = true;
                }
                
                //move the head and make some random turns
                pts[pts.size()-1] = newHead;
                if(ofRandom(1) > (1-turnChance))
                {
                    turn();
                }
                
                sumLength = 0;
                for(int i=1;i<pts.size();i++)
                {
                    sumLength += (pts[i+1] - pts[i]).length();
                }
                
                checkPts.clear();
                checkPts.push_back(0);  //first pt
                float sumLength2 = 0;
                for(int i=1;i<pts.size();i++)
                {
                    sumLength2 += (pts[i+1] - pts[i]).length();
                    checkPts.push_back((float)sumLength2 / sumLength);
                }
            }
        }
    }
    
    void draw(){
        ofSetColor(color, 500.0 / lineWidth);
        ofSetLineWidth(lineWidth);
        glBegin(GL_LINE_STRIP);
        
        ofSetCircleResolution(numTurns);
        for(int i=0;i<pts.size();i++)
        {
            glVertex3f(pts[i].x , pts[i].y, pts[i].z);
        }
        glEnd();
        
        ofFill();
        ofDrawCircle(pts[0], 3);
        ofDrawCircle(pts[pts.size()-1], 3);
        
        for(int i=0;i<segments.size();i++)
        {
            float segBeginFactor = ofClamp(segments[i].progress, 0, 1);
            ofVec3f segBegin = pointOnCircuit(segBeginFactor);
            float segEndFactor = ofClamp(segments[i].progress + segments[i].length, 0, 1);
            ofVec3f segEnd = pointOnCircuit(segEndFactor);
            ofSetColor(segments[i].color, 1000 / lineWidth);
            
            //rejecting top left corner segments (from invalid pointOnCircuit returns)
            if(segBegin.length() > 1 && segEnd.length() > 1)
            {
                glBegin(GL_LINE_STRIP);
                
                glVertex3f(segBegin.x + segments[i].offset.x, segBegin.y + segments[i].offset.y, segBegin.z + segments[i].offset.z);
                
                if(checkPts.size() > 0)
                {
                    for(int j=1;j<checkPts.size()-1;j++)
                    {
                        if(checkPts[j] > segments[i].progress && checkPts[j] < segments[i].progress + segments[i].length)
                        {
                            glVertex3f(pts[j].x + segments[i].offset.x, pts[j].y + segments[i].offset.y, pts[j].z + segments[i].offset.z);
                        }
                    }
                }
                
                glVertex3f(segEnd.x + segments[i].offset.x, segEnd.y + segments[i].offset.y, segEnd.z + segments[i].offset.z);
                
                
                glEnd();
            }
            
            ofSetColor(segments[i].color, 100.0 / lineWidth);
            ofNoFill();
            
            //circles
            ofDrawCircle(segBegin.x + segments[i].offset.x, segBegin.y +   segments[i].offset.y, segBegin.z + segments[i].offset.z, lineWidth / 4);

            if(checkPts.size() > 0)
            {
                for(int j=1;j<checkPts.size()-1;j++)
                {
                    if(checkPts[j] > segments[i].progress && checkPts[j] <   segments[i].progress + segments[i].length)
                    {
                        ofDrawCircle(pts[j].x + segments[i].offset.x, pts[j].y + segments[i].offset.y, pts[j].z + segments[i].offset.z, lineWidth / 4);
                    }
                }
            }
            ofDrawCircle(segEnd.x + segments[i].offset.x, segEnd.y + segments[i].offset.y, segEnd.z + segments[i].offset.z, lineWidth / 4);
            
            
            //points
            ofSetColor(segments[i].color);
            glBegin(GL_POINTS);
            if(checkPts.size() > 0)
            {
                glVertex3f(segBegin.x + segments[i].offset.x, segBegin.y +   segments[i].offset.y, segBegin.z + segments[i].offset.z);
                for(int j=1;j<checkPts.size()-1;j++)
               {
                    if(checkPts[j] > segments[i].progress && checkPts[j] <   segments[i].progress + segments[i].length)
                    {
                        glVertex3f(pts[j].x + segments[i].offset.x, pts[j].y + segments[i].offset.y, pts[j].z + segments[i].offset.z);
                    }
                }
                glVertex3f(segEnd.x + segments[i].offset.x, segEnd.y + segments[i].offset.y, segEnd.z + segments[i].offset.z);
            }

            glEnd();
            
            
            
//            ofNoFill();
//            ofDrawCircle(pointOnCircuit(segments[i].progress) + segments[i].offset, 1.5);
        }
    }
    
    ofVec3f pointOnCircuit(float progress)
    {
        for(int i=0;i<checkPts.size();i++)
        {
            if(progress < checkPts[i])
            {
                float checkPtDiff = checkPts[i] - checkPts[i-1];
                float progressDiff = progress - checkPts[i-1];
                float progressDiffFactor = progressDiff / checkPtDiff;
                
                return progressDiffFactor * pts[i-1] + (1 - progressDiffFactor) * pts[i];
            }
        }
    }
    
    void turn(){
        pts.push_back(ofVec3f(pts[pts.size()-1]));
        if(ofRandom(1)>0.5)
        {
            direction += 1;
        }
        else
        {
            direction -= 1;
        }
    }
    
    
    //LINE INTERSECT DETECTION CODE COPIED FROM https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
    
    bool onSegment(ofVec3f p, ofVec3f q, ofVec3f r)
    {
        if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
           return true;
      
        return false;
    }
      
    // To find orientation of ordered triplet (p, q, r).
    // The function returns following values
    // 0 --> p, q and r are colinear
    // 1 --> Clockwise
    // 2 --> Counterclockwise
    int orientation(ofVec3f p, ofVec3f q, ofVec3f r)
    {
        // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
        // for details of below formula.
        int val = (q.y - p.y) * (r.x - q.x) -
                  (q.x - p.x) * (r.y - q.y);
      
        if (val == 0) return 0;  // colinear
      
        return (val > 0)? 1: 2; // clock or counterclock wise
    }
      
    // The main function that returns true if line segment 'p1q1'
    // and 'p2q2' intersect.
    bool doIntersect(ofVec3f p1, ofVec3f q1, ofVec3f p2, ofVec3f q2)
    {
        // Find the four orientations needed for general and
        // special cases
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);
      
        // General case
        if (o1 != o2 && o3 != o4)
            return true;
      
        // Special Cases
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1
        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
      
        // p1, q1 and q2 are colinear and q2 lies on segment p1q1
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
      
        // p2, q2 and p1 are colinear and p1 lies on segment p2q2
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
      
         // p2, q2 and q1 are colinear and q1 lies on segment p2q2
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;
      
        return false; // Doesn't fall in any of the above cases
    }
};


#endif /* circuit_h */
