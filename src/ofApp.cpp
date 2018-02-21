/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */

#include "ofApp.h"
//#include "maximilian.h"/* include the lib */
#include "time.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>


//--------------------------------------------------------------
void ofApp::setup(){

#ifdef      PI_VERSION
    gpio18.setup("18");
    gpio18.export_gpio();
    gpio18.setdir_gpio("out");
    gpio17.setup("17");
    gpio17.export_gpio();
    gpio17.setdir_gpio("in");
#endif
    
    ofEnableAlphaBlending();
    ofSetupScreen();
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    
    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofBackground(0,0,0);
    
    //set sound threshold
    soundThreshold = 0.2;
    holdTime = (sampleRate / bufferSize)*2.0;
    
    #ifdef PI_VERSION
    soundstream.printDeviceList();
    soundstream.setDeviceID(2);
    soundstream.setup(this, 2, 1, sampleRate, bufferSize, 4);
    #else
    soundstream.printDeviceList();
    soundstream.setDeviceID(4);
    soundstream.setup(this, 2, 2, sampleRate, bufferSize, 4);
    #endif
    
    


    recordingOn = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
#ifdef      PI_VERSION
    gpio17.getval_gpio(recButton);
    
    recButton  =  !recButton;
    if(recButton){
        setupRecording();
    } else {
        processSoundVec();
    }

    
#endif
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
}


void ofApp::processSoundVec(){
    
    maxiSample out;
    maxiRecorder rec;
    processing = true;
    double frequencyValues = 44100/bufferSize;
    float buffer1[(int)frequencyValues];
    double lengthSamp = recorder.size();
    double sampChunks = lengthSamp/frequencyValues;
    string name = "recording";
    name = name+ofToString(recCount);
    name = name+".wav";
    rec.setup(ofToDataPath(name));
    recCount++;
    rec.startRecording();
    for(double i = 0 ; i < sampChunks; i++){
        double pos = i*frequencyValues;
        for(double playing = 0; playing<frequencyValues; playing++){
            buffer1[(int)playing]=recorder[playing+pos];
        }
        rec.passData(buffer1,frequencyValues);
    }
    
    rec.stopRecording();
    rec.saveToWav();
    processing = false;
    
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    if(playBack){
        for (int i = 0; i < bufferSize; i++){
            double wave = playbackSound.play();
            output[i*nChannels] = wave;
            output[i*nChannels+1] = wave;
        }
    }
  
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    
    
    
    // samples are "interleaved"
    if(recordingOn){
        for(int i = 0; i < bufferSize; i++){
        /* you can also grab the data out of the arrays*/
            recorder.push_back(input[i*nChannels]);
        }
        currHold --;
        if(currHold == 0){
            recordingOn = false;
            processSoundVec();
        }
    
    
    } else {
        for(int i = 0; i < bufferSize; i++){
            if(fabs(input[i*nChannels]) > soundThreshold && recordingOn == false && processing == false){
                cout << "PLAY" << endl;
                setupRecording();
                currHold = holdTime;
            }
        }

    }
}

void ofApp::setupRecording(){
        recorder.resize(0);
    bufferCount = 0;
    if(recordingOn  == true){
        recordingOn = false;
    } else {
        recordingOn = true;
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key =='a'){
        if(recButton  == true){
            recButton = false;
        } else {
            recButton = true;
        }

        if(recButton){
            
            setupRecording();
        } else {
            processSoundVec();
        }
    }
    if(key =='p'){
        playbackSound.load(ofToDataPath("recording0.wav"));
        playBack =! playBack;
        }
   }

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}


void ofApp::exit(){
#ifdef PI_VERSION
    gpio17.unexport_gpio();
    gpio18.unexport_gpio();
#endif
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
