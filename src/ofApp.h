#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "Particle.h"

#define NUM 5
class Color {
public:
    float hue, sat, bri;
    ofxVec2f pos;
};

#define _USE_LIVE_VIDEO
 //uncomment this to use a live camera
 //otherwise, we'll use a movie file

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void pattern(int centerX, int centerY, int length);
    
    int wWidth;
    int wHeight;
    //B G Y O R
    int hue[5] = {104, 70, 26, 174, 179};
    int sat[5] = {123, 142, 155, 80, 180};
    int val[5] = {189, 129, 214, 204, 149};
    bool drawEnabled;
    int count[5];
    int timer;
    ofPoint pos_L[5], pos_R[5], pos_U[5], pos_D[5];
    
    ofVideoGrabber        wCam;
    ofxCvColorImage        origOutput;
    ofxCvColorImage        origOutputHSV;
    
    ofxCvGrayscaleImage hueOutput;
    ofxCvGrayscaleImage satOutput;
    ofxCvGrayscaleImage briOutput;
    
    unsigned char *        lockedOnPixels[5];
    ofTexture            lockedOnTexture[5];
    
    ofxCvGrayscaleImage    lockedOutput[5];
    ofxCvContourFinder    lockedContours[5];
    
    unsigned char *        scribblePixels;
    ofTexture            scribbleOutput;
    
    vector<Particle> particle_L[5];
    vector<Particle> particle_R[5];
    vector<Particle> particle_U[5];
    vector<Particle> particle_D[5];
    ofImage particleImage;
    ofImage particleImage2, particleImage3, particleImage4, particleImage5, background;
    bool box[5];
    
    float timeCounter = 0;
     bool soundplay1;
    
    ofSoundPlayer file1, file2, file3, file4, file5;
    
//    static const int NUM = 5;
//    

};


