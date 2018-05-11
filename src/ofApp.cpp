#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    

    timer = 0;
    wWidth  = 1024;
    wHeight = 768;
    
    soundplay1 = false;
    drawEnabled = true;
    
    wCam.initGrabber(wWidth, wHeight);
    
    origOutput.allocate(wWidth, wHeight);
    origOutputHSV.allocate(wWidth, wHeight);
    
    hueOutput.allocate(wWidth, wHeight);
    satOutput.allocate(wWidth, wHeight);
    briOutput.allocate(wWidth, wHeight);
    
    for(int i=0; i<5; i++){
        lockedOnPixels[i] = new unsigned char [wWidth * wHeight];
        lockedOnTexture[i].allocate(wWidth, wHeight, GL_LUMINANCE);
        box[i] = false;
        lockedOutput[i].allocate(wWidth, wHeight);
    }
    
    scribblePixels = new unsigned char[wWidth * wHeight];
    for (int i = 0; i < (wWidth * wHeight); i++) {
        scribblePixels[i] = 0;
    }
    scribbleOutput.allocate(wWidth, wHeight, GL_LUMINANCE);
    
    ofSetFrameRate(60);
    //ofBackground(63);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    particleImage.load("particle32.png");
    particleImage2.load("3.png");
    particleImage3.load("5.png");
    particleImage4.load("4.png");
    particleImage5.load("1.png");
    
    background.load("star.jpg");
    
    file1.load("1.wav");
    file2.load("2.wav");
    file3.load("3.wav");
    file4.load("4.wav");
    file5.load("5.wav");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    ofPushMatrix();

    ofPopMatrix();

    wCam.update();
    if (wCam.isFrameNew()) {
        origOutput.setFromPixels(wCam.getPixels());
        origOutput.resize(wWidth, wHeight);
        //ofImage myImage = ofImage(wCam.getPixels());
        origOutputHSV = origOutput;
        origOutputHSV.convertRgbToHsv();

        origOutputHSV.convertToGrayscalePlanarImages(hueOutput, satOutput, briOutput);

        hueOutput.flagImageChanged();
        satOutput.flagImageChanged();
        briOutput.flagImageChanged();

        unsigned char * huePixels = hueOutput.getPixels().getData();
        unsigned char * satPixels = satOutput.getPixels().getData();
        unsigned char * briPixels = briOutput.getPixels().getData();
        for(int j=0; j<5; j++){
            for (int i = 0; i < (wWidth * wHeight); i++) {

                if ((huePixels[i] >= (hue[j]-12)  && huePixels[i] <= (hue[j]+12)) &&
                    (satPixels[i] >= (sat[j]-24) && satPixels[i] <= (sat[j]+200))) {
                    lockedOnPixels[j][i] = 255;
                    box[j] = true;
                    
                } else {
                    lockedOnPixels[j][i] = 0;
                }
            }
        }
        for(int i=0; i<5; i++){
            lockedOutput[i].setFromPixels(lockedOnPixels[i], wWidth, wHeight);

            lockedContours[i].findContours(lockedOutput[i], 160, (wWidth * wHeight) / 3, 1, false, true);

            lockedOnTexture[i].loadData(lockedOnPixels[i], wWidth, wHeight, GL_LUMINANCE);

        if (lockedContours[i].blobs.size() > 0 && drawEnabled) {
            //count[i]++;
            int lineWidth = 3;
            /*Draw*/
            for (int a = -lineWidth; a <= lineWidth; a++) {
                for (int z = -lineWidth; z <= lineWidth; z++) {
                    //scribblePixels[((int)(lockedContours.blobs.at(0).centroid.y + i) * wWidth) + (int)((wWidth - lineWidth - 1) - lockedContours.blobs.at(0).centroid.x + z)] = 128;
                }
            }
            pos_L[i] = ofVec2f(lockedContours[i].blobs.at(0).centroid.x  - lockedContours[i].blobs.at(0).length/4  , (lockedContours[i].blobs.at(0).centroid.y));
            pos_R[i] = ofVec2f(lockedContours[i].blobs.at(0).centroid.x + lockedContours[i].blobs.at(0).length/4 , lockedContours[i].blobs.at(0).centroid.y );
            pos_U[i] = ofVec2f(lockedContours[i].blobs.at(0).centroid.x , lockedContours[i].blobs.at(0).centroid.y - lockedContours[i].blobs.at(0).length/4 );
            pos_D[i] = ofVec2f(lockedContours[i].blobs.at(0).centroid.x  , lockedContours[i].blobs.at(0).centroid.y +lockedContours[i].blobs.at(0).length/4);
            
            
            float length = ofRandom(1);
            float angle[4];
            int move = ofGetElapsedTimef()/30;
            angle[0] = ofRandom(PI*2);
            ofVec2f vel[4];
            vel[0] = ofVec2f(-move, sin(angle[0]));
            vel[1] = ofVec2f(move, sin(angle[0]));
            vel[2] = ofVec2f(sin(angle[0]), -move);
            vel[3] = ofVec2f(sin(angle[0]), move);
            
            
            Particle p[5][4];
            
            
            p[i][0].setup(pos_L[i], vel[0]);
            p[i][1].setup(pos_R[i], vel[1]);
            p[i][2].setup(pos_U[i], vel[2]);
            p[i][3].setup(pos_D[i], vel[3]);
            
            p[i][0].radius = 2;
            p[i][0].friction = 0.002;
            
            //cout << timeCounter << endl;
            //cout << ofGetElapsedTimef()<<endl;
            if(timer %30 == 0){

            particle_L[i].push_back(p[i][0]);
            particle_R[i].push_back(p[i][1]);
            particle_U[i].push_back(p[i][2]);
            particle_D[i].push_back(p[i][3]);
            }
//
//            timeCounter = ofGetElapsedTimef();
            //cout << timeCounter << endl;
            
            if(count[i] > 360){
                particle_L[i].clear();
                particle_R[i].clear();
                particle_U[i].clear();
                particle_D[i].clear();
                pos_L[i] = ofVec2f(NULL);
                pos_R[i] = ofVec2f(NULL);
                pos_U[i] = ofVec2f(NULL);
                pos_D[i] = ofVec2f(NULL);
                
                count[i] = 0;
            }
            
        }
            if(lockedContours[i].blobs.size() == 0)
            {
                box[i] = false;
            }
        
        scribbleOutput.loadData(scribblePixels, wWidth, wHeight, GL_LUMINANCE);
    

    for(int j = 0; j < particle_L[i].size(); j++){
        particle_L[i][j].resetForce();
        particle_L[i][j].updateForce();
        particle_L[i][j].updatePos();
        particle_R[i][j].resetForce();
        particle_R[i][j].updateForce();
        particle_R[i][j].updatePos();
        particle_U[i][j].resetForce();
        particle_U[i][j].updateForce();
        particle_U[i][j].updatePos();
        particle_D[i][j].resetForce();
        particle_D[i][j].updateForce();
        particle_D[i][j].updatePos();
    }
    }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    timer++;

    ofBackground(20);
    background.draw(0,0);
    
    ofPushMatrix();

    //ofSetColor(0xffffff);
    /*webcam image*/
    //wCam.draw(0,0,wWidth,wHeight);

    for(int i=0; i<5; i++){
        lockedContours[i].draw(0, 0);
    }

    /**/
    //origOutputHSV.draw(360, 0);
    //lockedContours.draw();

    /**/
    //lockedOnTexture.draw(720, 0);
    //ofDrawBitmapString("red", 720, 20);

    /*draw*/
    //scribbleOutput.draw(480, 0);
    scribbleOutput.draw(0, 0);

    //cout << box[1] << endl;

    char tmpStr[255];
    //sprintf(tmpStr, "h: %i\ns: %i\nv: %i", hue, sat, val);
    ofDrawBitmapString(tmpStr, 20, 260);
    for(int i=0; i<5; i++){
        if(lockedContours[i].blobs.size() > 0 && drawEnabled){
             // ofDrawTriangle(lockedContours.blobs.at(0).centroid.x + lockedContours.blobs.at(0).length, lockedContours.blobs.at(0).centroid.y + lockedContours.blobs.at(0).length);
            pattern(lockedContours[i].blobs.at(0).centroid.x, lockedContours[i].blobs.at(0).centroid.y , lockedContours[i].blobs.at(0).length);
        //ofDrawCircle(lockedContours[i].blobs.at(0).centroid.x, lockedContours[i].blobs.at(0).centroid.y, 10);
        }
    if(box[i] == true)
    {
        count[i]++;
    }
        
    //ofSetHexColor(0xffffff);
    if(timer %30 > 0 && timer%30 <15)
    {
        if(box[0] == true)
        {
            for(int i = 0; i < particle_L[0].size(); i++){
                //particle[i].draw();
                //ofPoint pos = ofPoint(lockedContours[i].blobs.at(0).centroid.x, lockedContours[i].blobs.at(0).centroid.y);
                pos_L[0] = ofPoint(particle_L[0][i].position.x, particle_L[0][i].position.y);
                particleImage.draw(pos_L[0].x-16, pos_L[0].y-16, 32, 32);
                pos_R[0] = ofPoint(particle_R[0][i].position.x, particle_R[0][i].position.y);
                particleImage.draw(pos_R[0].x-16, pos_R[0].y-16, 32, 32);
                pos_U[0] = ofPoint(particle_U[0][i].position.x, particle_U[0][i].position.y);
                particleImage.draw(pos_U[0].x-16, pos_U[0].y-16, 32, 32);
                pos_D[0] = ofPoint(particle_D[0][i].position.x, particle_D[0][i].position.y);
                particleImage.draw(pos_D[0].x-16, pos_D[0].y-16, 32, 32);
                
                if(!file1.isPlaying())
                {
                    file1.play();
                }
                
                if(lockedContours[0].blobs.size() > 0){
                    cout<<"in the loop"<<endl;
                    lockedContours[0].blobs.at(0).draw();
                    ofFill();
                    ofSetColor('red');
                }
            }
        }
        if(box[1] == true)
        {
            for(int i = 0; i < particle_L[1].size(); i++){
                //particle[i].draw();
                //ofPoint pos = ofPoint(lockedContours[i].blobs.at(0).centroid.x, lockedContours[i].blobs.at(0).centroid.y);
                pos_L[1] = ofPoint(particle_L[1][i].position.x, particle_L[1][i].position.y);
                particleImage2.draw(pos_L[1].x-16, pos_L[1] .y-16, 32, 32);
                pos_R[1] = ofPoint(particle_R[1][i].position.x, particle_R[1][i].position.y);
                particleImage2.draw(pos_R[1].x-16, pos_R[1].y-16, 32, 32);
                pos_U[1] = ofPoint(particle_U[1][i].position.x, particle_U[1][i].position.y);
                particleImage2.draw(pos_U[1].x-16, pos_U[1].y-16, 32, 32);
                pos_D[1] = ofPoint(particle_D[1][i].position.x, particle_D[1][i].position.y);
                particleImage2.draw(pos_D[1].x-16, pos_D[1].y-16, 32, 32);

                if(!file2.isPlaying())
                {
                    file2.play();
                }
            }
        }

        if(box[2] == true)
        {
            for(int i = 0; i < particle_L[2].size(); i++){
                //particle[i].draw();
                //ofPoint pos = ofPoint(lockedContours[i].blobs.at(0).centroid.x, lockedContours[i].blobs.at(0).centroid.y);
                pos_L[2] = ofPoint(particle_L[2][i].position.x, particle_L[2][i].position.y);
                particleImage3.draw(pos_L[2].x-16, pos_L[2] .y-16, 32, 32);
                pos_R[2] = ofPoint(particle_R[2][i].position.x, particle_R[2][i].position.y);
                particleImage3.draw(pos_R[2].x-16, pos_R[2].y-16, 32, 32);
                pos_U[2] = ofPoint(particle_U[2][i].position.x, particle_U[2][i].position.y);
                particleImage3.draw(pos_U[2].x-16, pos_U[2].y-16, 32, 32);
                pos_D[2] = ofPoint(particle_D[2][i].position.x, particle_D[2][i].position.y);
                particleImage3.draw(pos_D[2].x-16, pos_D[2].y-16, 32, 32);

                if(!file3.isPlaying())
                {
                    file3.play();
                }
            }
        }

        if(box[3] == true)
        {
            for(int i = 0; i < particle_L[3].size(); i++){
                //particle[i].draw();
                //ofPoint pos = ofPoint(lockedContours[i].blobs.at(0).centroid.x, lockedContours[i].blobs.at(0).centroid.y);
                pos_L[3] = ofPoint(particle_L[3][i].position.x, particle_L[3][i].position.y);
                particleImage4.draw(pos_L[3].x-16, pos_L[3].y-16, 32, 32);
                pos_R[3] = ofPoint(particle_R[3][i].position.x, particle_R[3][i].position.y);
                particleImage4.draw(pos_R[3].x-16, pos_R[3].y-16, 32, 32);
                pos_U[3] = ofPoint(particle_U[3][i].position.x, particle_U[3][i].position.y);
                particleImage4.draw(pos_U[3].x-16, pos_U[3].y-16, 32, 32);
                pos_D[3] = ofPoint(particle_D[3][i].position.x, particle_D[3][i].position.y);
                particleImage4.draw(pos_D[3].x-16, pos_D[3].y-16, 32, 32);

                if(!file4.isPlaying())
                {
                    file4.play();
                }
            }
        }

        if(box[4] == true)
        {
            for(int i = 0; i < particle_L[4].size(); i++){
                //particle[i].draw();
                //ofPoint pos = ofPoint(lockedContours[i].blobs.at(0).centroid.x, lockedContours[i].blobs.at(0).centroid.y);
                pos_L[4] = ofPoint(particle_L[4][i].position.x, particle_L[4][i].position.y);
                particleImage5.draw(pos_L[4].x-16, pos_L[4].y-16, 32, 32);
                pos_R[4] = ofPoint(particle_R[4][i].position.x, particle_R[4][i].position.y);
                particleImage5.draw(pos_R[4].x-16, pos_R[4].y-16, 32, 32);
                pos_U[4] = ofPoint(particle_U[4][i].position.x, particle_U[4][i].position.y);
                particleImage5.draw(pos_U[4].x-16, pos_U[4].y-16, 32, 32);
                pos_D[4] = ofPoint(particle_D[4][i].position.x, particle_D[4][i].position.y);
                particleImage5.draw(pos_D[4].x-16, pos_D[4].y-16, 32, 32);

                if(!file5.isPlaying())
                {
                    file5.play();
                }
            }
        }

    }
        //ofDrawBitmapString("particle num = " + ofToString(particle.size()), 10, 15);
        ofDrawBitmapString("fps = " + ofToString(ofGetFrameRate()), 10, 30);
    }
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::pattern(int centerX, int centerY, int length){
    ofPushMatrix();
    ofNoFill();
    ofDrawCircle(centerX, centerY, length/4);
    ofDrawCircle(centerX, centerY, length/ofRandom(10));
    ofDrawCircle(centerX, centerY, length/ofRandom(10));
    ofDrawCircle(centerX, centerY, length/ofRandom(10));
    ofPopMatrix();

    ofFill();
    //ofDrawTriangle(centerX + length/4, centerY + length/4, centerX + length/4, centerY - length/4, centerX + length, centerY);



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == OF_KEY_BACKSPACE) {
        for (int i = 0; i < (wWidth * wHeight); i++) {
            scribblePixels[i] = 0;
        }
    }

    if (key == OF_KEY_RETURN) { drawEnabled = false; }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == OF_KEY_RETURN) { drawEnabled = true; }

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
