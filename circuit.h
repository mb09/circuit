//
//  circuit.h
//  cyber_1
//
//  Created by mb9 on 23/11/2020.
//

#ifndef circuit_h
#define circuit_h

class Circuit{
public:
    vector<ofVec3f> pts;
    int direction;
//    ofVec3f head;
    float speed;
    int id;
    bool isDead;
    ofColor color;
    float turnChance;
    int numTurns;
    int lineWidth;
    
    Circuit(ofVec3f _head, int _direction, ofColor _color, float _turnChance, int _numTurns){
        color = _color;
        turnChance = _turnChance;
        numTurns = _numTurns;
        direction = _direction;
        pts.push_back(ofVec3f(_head));
        pts.push_back(ofVec3f(_head));
        speed = ofRandom(10, 25);
        isDead = false;
        lineWidth = ofRandom(1,4);
    }
    
    void update(vector<Circuit> *circuits){
        if(!isDead)
        {
            ofVec3f move = ofVec3f(cos(direction * TWO_PI/numTurns),sin(direction * TWO_PI/numTurns)) * speed;
            ofVec3f newHead = pts[pts.size()-1] + move;
            
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
                        if(doIntersect(pts[pts.size()-1], newHead, c.pts[j-1], c.pts[j]))
                        {
                            isDead = true;
                        }
                    }
                }
            }
            if(!isDead)
            {
                if(newHead.x < 0 || newHead.y < 0 || newHead.x > ofGetWidth() || newHead.y > ofGetHeight())
                {
                    isDead = true;
                }
                pts[pts.size()-1] = newHead;
                if(ofRandom(1) > (1-turnChance))
                {
                    turn();
                }
                
            }
        }
    }
    
    void draw(){
        ofSetColor(color);
        ofSetLineWidth(lineWidth);
        glBegin(GL_LINE_STRIP);
        for(int i=0;i<pts.size();i++)
        {
            glVertex3f(pts[i].x , pts[i].y, pts[i].z);
        }
        glEnd();
        
//        ofSetColor(0);
        ofFill();
//        ofDrawCircle(pts[0], 5);
//        ofDrawCircle(pts[pts.size()-1], 5);
//        ofSetColor(color);
//        ofNoFill();
        ofDrawCircle(pts[0], 3);
        ofDrawCircle(pts[pts.size()-1], 3);
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
