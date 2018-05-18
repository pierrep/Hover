#include "ofApp.h"

string ofApp::sampleToLoad = "test";

ofApp::~ofApp()
{
    cout << "exiting." << endl;
    soundStream.stop();
    cout << "soundstream stopped." << endl;
    soundStream.close();
    cout << "soundstream closed." << endl;

    while(!pads.empty()) {
        delete pads.back(), pads.pop_back();
    }
    pads.clear();

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetWindowTitle("Hover");
    loadLayoutXML();

    const int padlength = 150;
    marginSpacer = 10;
    sliderArea = ofRectangle(marginSpacer +  (padlength+marginSpacer)*4, marginSpacer, 270,  ofGetHeight()-(marginSpacer*2));

    for(unsigned int i =0; i < pads.size(); i++)
    {
        pads[i]->set(marginSpacer + (i%4)*(padlength+marginSpacer), (padlength+marginSpacer)*3 + marginSpacer - (i/4)*(padlength+marginSpacer), padlength, padlength);
        pads[i]->setup();
    }
    ofAddListener(PadEventArgs::padEvent, this, &ofApp::onPadEvent);

    setupAudio();
    setupMidi();

    bDoLeap = false;    
    bLeapInited = false;
    #ifdef USE_LEAP
    bLeapInited = leap.open();
    #endif

}

//--------------------------------------------------------------
void ofApp::update(){
    if((bDoLeap) && (bLeapInited)) {
		updateLeap();
    }


}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor::lightGray,ofColor::darkGray);

    // create some sliders to control those parameters
    float sliderHeight = sliderArea.getHeight() / sliders[selectedPad].size();

    ///Background
    ofSetColor(150,150,150);
    ofDrawRectangle(sliderArea.x,sliderArea.y,sliderArea.width, sliderArea.height);

    for(int i = 0; i < sliders[selectedPad].size(); i++){
        ofPoint sliderStart = ofPoint(sliderArea.getX(), sliderArea.getY() + i * sliderHeight);

        ParameterSlider& slider = sliders[selectedPad].at(i);
        float sliderPosition = (slider.param.getValue() - slider.param.getMin()) / (slider.param.getMax() - slider.param.getMin());

        if(slider.param.getName() == "volume") {
            sliderPosition = pads[selectedPad]->volume;
        }
        ///slider background
        ofSetColor(120, 120, 120);
        ofDrawRectangle(sliderStart.x, sliderStart.y, sliderArea.getWidth(), sliderHeight - 3);
        ///slider
        ofSetColor(50, 50, 50);
        ofDrawRectangle(sliderStart.x, sliderStart.y, sliderArea.getWidth() * sliderPosition, sliderHeight - 3);
        ofSetColor(255, 255, 255);
        stringstream sliderLabel;
        if(slider.param.getName() == "volume") {
            sliderLabel << slider.param.getDisplayName()
              << ": "
              << pads[selectedPad]->volume;

        } else {
            sliderLabel << slider.param.getDisplayName()
              << ": "
              << slider.param.getValue();
        }
        ofDrawBitmapString(sliderLabel.str(), sliderStart.x + 5, sliderStart.y + 20);
    }

    ///Master vol background
    ofSetColor(120, 120, 120);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer, sliderArea.getWidth(), 60 - 3);
    ///Master vol slider
    ofSetColor(50, 50, 50);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer, sliderArea.getWidth() * masterVolume.getDeclickedValue(), 60 - 3);
    ofSetColor(255,255,255);
    ofDrawBitmapString("Master vol: "+ofToString(masterVolume.getDeclickedValue()),sliderArea.getX()+5+ sliderArea.getWidth()+marginSpacer,marginSpacer + 20);

    ///FX1 send slider background
    ofSetColor(120, 120, 120);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer +60, sliderArea.getWidth(), 60 - 3);
    ///FX slider
    ofSetColor(50, 50, 50);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer +60, sliderArea.getWidth() * engine.getFX1Send(selectedPad).getValue(), 60 - 3);
    ofSetColor(255,255,255);
    ofDrawBitmapString("FX1 send:: "+ofToString(engine.getFX1Send(selectedPad).getValue()),sliderArea.getX()+5+ sliderArea.getWidth()+marginSpacer,marginSpacer +60 + 20);

    ///FX slider background
    ofSetColor(120, 120, 120);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer +60*2, sliderArea.getWidth(), 60 - 3);
    ///FX slider
    ofSetColor(50, 50, 50);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer +60*2, sliderArea.getWidth() * crushFXvol, 60 - 3);
    ofSetColor(255,255,255);
    ofDrawBitmapString("Crush vol: "+ofToString(crushFXvol),sliderArea.getX()+5+ sliderArea.getWidth()+marginSpacer,marginSpacer +60*2 + 20);

    ///FX slider background
    ofSetColor(120, 120, 120);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer +60*3, sliderArea.getWidth(), 60 - 3);
    ///FX slider
    ofSetColor(50, 50, 50);
    ofDrawRectangle(sliderArea.getX() + sliderArea.getWidth()+marginSpacer, marginSpacer +60*3, sliderArea.getWidth() * crushFXamount, 60 - 3);
    ofSetColor(255,255,255);
    ofDrawBitmapString("Crush amt: "+ofToString(crushFXamount),sliderArea.getX()+5+ sliderArea.getWidth()+marginSpacer,marginSpacer +60*3 + 20);
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{

	float leftPan = 1 - masterPan;
	float rightPan = masterPan;
	float synthbuf[bufferSize*nChannels];

    masterVolume.rebuildRampIfNecessary();

	engine.fillBufferOfFloats(synthbuf,bufferSize,nChannels);

    for (int i = 0; i < bufferSize; i++) {
        outLeft = 0.0f;
        outRight = 0.0f;


        outLeft     += synthbuf[i*nChannels    ];
        outRight    += synthbuf[i*nChannels + 1];

        crushFX.setValues(8,crushFXamount);
        outLeft = outLeft + crushFX.decimate(outLeft) * crushFXvol;
        outRight = outRight + crushFX.decimate(outRight) * crushFXvol;

        output[i*nChannels    ] = outLeft * masterVolume.getDeclickedValue() * leftPan;
        output[i*nChannels + 1] = outRight * masterVolume.getDeclickedValue() * rightPan;

        masterVolume.frameTick();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_ESC) {
        masterVolume = 0.0f;
    }
    if(key == 's') saveSet();

    if(key == ' ') {
        //bDoLeap = !bDoLeap;
        //pads[selectedPad]->bSelected = !(pads[selectedPad]->bSelected);
        if(pads[selectedPad]->bSelected) {
            pads[selectedPad]->onPress(0,0,0);
        }
        else {
            pads[selectedPad]->onRelease(0,0,0);
        }
    }

    if(key == OF_KEY_LEFT)
    {
        pads[selectedPad]->bSelected = false;
        selectedPad -= 1;
        if(selectedPad < 0) selectedPad = pads.size() - 1;
        pads[selectedPad]->bSelected = true;
    }
    if(key == OF_KEY_DOWN)
    {
        pads[selectedPad]->bSelected = false;
        selectedPad -= 4;
        if(selectedPad < 0) selectedPad += pads.size();
        pads[selectedPad]->bSelected = true;
    }
    if(key == OF_KEY_RIGHT)
    {
        pads[selectedPad]->bSelected = false;
        selectedPad += 1;
        if(selectedPad > pads.size()-1) selectedPad = 0;
        pads[selectedPad]->bSelected = true;
    }
    if(key == OF_KEY_UP)
    {
        pads[selectedPad]->bSelected = false;
        selectedPad += 4;
        if(selectedPad > pads.size()-1) selectedPad -= pads.size();
        pads[selectedPad]->bSelected = true;
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

    if (sliderArea.inside(x, y)) {
        float amountDown = (y - sliderArea.getY()) / sliderArea.getHeight();
        float amountAcross = (x - sliderArea.getX()) / sliderArea.getWidth();
        int sliderIndex = sliders[selectedPad].size() * amountDown;
        if(pads[selectedPad]->audioType == InteractivePad::SYNTH) {
            if(sliders[selectedPad].size() > 0) {
                if(sliderIndex == 0) {
                    if(sliders[selectedPad].at(0).param.getName() == "volume") {
                        if(pads[selectedPad]->bIsOn) {
                            sliders[selectedPad].at(0).param.setNormalizedValue(amountAcross);
                        }
                        pads[selectedPad]->volume = amountAcross;

                    } else {
                        ofLogError() << "mouseDragged() - Can't set volume, parameter not found";
                    }

                }
                else if(sliders[selectedPad].at(sliderIndex).param.getParameterType() == Tonic::ControlParameterTypeToggle)
                {
                    amountAcross = floor(amountAcross+0.5f);
                    sliders[selectedPad].at(sliderIndex).param.setNormalizedValue(amountAcross);
                }
                else {
                    sliders[selectedPad].at(sliderIndex).param.setNormalizedValue(amountAcross);
                }
            }
        }
    }

    ofRectangle volArea = sliderArea;
    volArea.height = 60-3;

    ///Master vol
    volArea.x += volArea.width + marginSpacer;
    if(volArea.inside(x,y)) {
        float amt = (x - volArea.getX()) / volArea.getWidth();
        masterVolume = amt;
    }

    ///FX send
    volArea.y += 60;
    if(volArea.inside(x,y)) {
        float amt = (x - volArea.getX()) / volArea.getWidth();
        engine.getFX1Send(selectedPad).value(amt);
    }

    ///FX vol
    volArea.y += 60;
    if(volArea.inside(x,y)) {
        float amt = (x - volArea.getX()) / volArea.getWidth();
        crushFXvol = amt;
    }

    volArea.y += 60;
    if(volArea.inside(x,y)) {
        float amt = (x - volArea.getX()) / volArea.getWidth();
        crushFXamount = amt;
    }
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

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg)
{

    int padId = ofMap(msg.pitch,36,51,0,15,true);
    if(ofxMidiMessage::getStatusString(msg.status) == "Note On") {
        if(padId < numPads) {
            pads[padId]->onPress(0,0,0);
        }
    }
    if(ofxMidiMessage::getStatusString(msg.status) == "Note Off") {
        if(padId < numPads) {
            pads[padId]->onRelease(0,0,0);
        }
    }
     if(ofxMidiMessage::getStatusString(msg.status) == "Control Change") {
        if(msg.control == 1)
        {
            if(pads[selectedPad]->bIsOn) {
                if(sliders[selectedPad].at(0).param.getName() == "volume") {
                    sliders[selectedPad].at(0).param.setNormalizedValue((float)msg.value/127.0f);
                } else {
                    ofLogError() << "newMidiMessage - Can't set volume, parameter not found";
                }
            }
            pads[selectedPad]->volume = (float)msg.value/127.0f;
        }
        if(msg.control == 115)
        {
            pads[selectedPad]->bSelected = false;
            selectedPad -= 1;
            if(selectedPad < 0) selectedPad = pads.size() - 1;
            pads[selectedPad]->bSelected = true;
        }
        if(msg.control == 116)
        {
            pads[selectedPad]->bSelected = false;
            selectedPad += 1;
            if(selectedPad > pads.size()-1) selectedPad = 0;
            pads[selectedPad]->bSelected = true;
        }
     }
    //cout << "padId: " << padId << endl;
	//cout << "Received: " << ofxMidiMessage::getStatusString(msg.status);
    //cout << " value: " << msg.value << " pitch: " << msg.pitch  <<  " velocity: " << msg.velocity << "control: " << msg.control << endl;
}

//--------------------------------------------------------------
void ofApp::onPadEvent(PadEventArgs &e)
{
    pads[selectedPad]->bSelected = false;
    currentPad = e.index;
    selectedPad = currentPad;
    if(e.state) {
        //cout << "Pad ON:" << e.index << endl;
        float vol = pads[e.index]->volume;
        if(sliders[selectedPad].at(0).param.getName() == "volume") {
            sliders[selectedPad].at(0).param.setNormalizedValue(vol);
        } else {
            ofLogError() << "onPadEvent - Can't set volume, parameter not found";
        }
        pads[e.index]->bSelected = true;
    }
    else {
        //cout << "Pad OFF:" << e.index << endl;
        if(sliders[selectedPad].at(0).param.getName() == "volume") {
            pads[e.index]->volume = sliders[selectedPad].at(0).param.getNormalizedValue();
            sliders[selectedPad].at(0).param.setNormalizedValue(0.0f);
        } else {
            ofLogError() << "onPadEvent - Can't set volume, parameter not found";
        }
        pads[e.index]->bSelected = true;
    }

}

//--------------------------------------------------------------
void ofApp::loadLayoutXML(){

    numPads = numSynths = numSamplers = numGrainpitchers = 0;
	currentPad = 0;
	selectedPad = 0;
    int instrumentId = 0;

	if( xml.load("layout.xml") ){
        cout << "Loading layout.xml" << endl;
        xml.setTo("layout");


        if(xml.exists("pad"))
        {
            xml.setTo("pad[0]");
            do {
                // get each individual pad
                string type = xml.getValue<string>("type");
                string padtype = xml.getValue<string>("padtype");
                string name = xml.getValue<string>("name");
                string filename = xml.getValue<string>("file");
                string samplename = xml.getValue<string>("sample");
                bool bIsOn = xml.getValue<bool>("state");
                float volume = xml.getValue<float>("volume");
                float fx1send = xml.getValue<float>("FX1send");

                InteractivePad::PadType padType;
                if(padtype == "toggle") {
                    padType = InteractivePad::TOGGLE;
                } else {
                    padType = InteractivePad::BUTTON;
                }

                InteractivePad::AudioType audioType;
                cout << "*---------------------------------------------------------------" << endl;

                cout << "type: " << type << "\t\tname: " << name << "\t\tfile: " <<  filename << endl;

                if(type == "synth")
                {
                    audioType = InteractivePad::SYNTH;

                    if(!samplename.empty() ) {
                        string name = "samples/"+samplename;
                        name = ofToDataPath(name,true);
                        ofApp::sampleToLoad = name;
                    }

                    int index = engine.createSynth(filename);

                    // build a slider for each parameter
                    vector<ControlParameter> synthParameters = engine.getSynth(index).getParameters();

                    bool bDoPop = false;
                    xml.setTo("parameters[0]");

                    vector<ParameterSlider> v;
                    for(int i = 0; i < synthParameters.size(); i++){
                        float p = xml.getValue<float>("param"+ofToString(i));
                        synthParameters.at(i).value(p);
                        v.push_back(ParameterSlider(synthParameters.at(i)));
                    }
                    sliders.push_back(v);

                    xml.setToParent(1);

                    instrumentId = numSynths;
                    numSynths++;

                }
                if(type != "empty") {
                InteractivePad *p = new InteractivePad(name,numPads,instrumentId,padType,audioType,filename,samplename,volume);
                p->bIsOn = bIsOn;
                pads.push_back(p);
                engine.getFX1Send(numPads).value(fx1send);
                numPads++;
                }


            }
            while( xml.setToSibling() ); // go to the next pad

            pads[selectedPad]->bSelected = true;
        }
        cout << "*---------------------------------------------------------------" << endl;
	}else{
		cout << "Failed to load layout.xml" << endl;
	    std::exit(-1);
	}
}

//--------------------------------------------------------------
void ofApp::setupAudio()
{
	int bufferSize  = 128;
	int sampleRate  = 44100;
	masterPan = 0.5f;
	masterVolume = 0.0f;

	speed = -1;
	rate = 0.001;
	overlaps = 3;
	grainLength = 0.05;

    ///aux sends
    crushFX.setValues(8,0.5f);
    auxsend1.resize(pads.size());
	//soundStream.listDevices();
	//soundStream.setDeviceID(1);
    bool bInited = soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    if(!bInited) {
        ofLogError() << "Failed to open audio. Is Jack running?";
        soundStream.printDeviceList();
    }

    ofSleepMillis(200);
	masterVolume = 0.5f;
}

//--------------------------------------------------------------
void ofApp::setupMidi()
{
    bMidiExists = false;
	midiIn.listPorts();
	vector<string> ports =  midiIn.getPortList();

	for(int i=0; i < ports.size();i++)
    {
        if(strncmp(ports.at(i).c_str(),"Akai MPD24 24",14) == 0)
        {
            //midiIn.openPort(1);
            bMidiExists = midiIn.openPort(ports.at(i));	// by name
        }
    }

	if(bMidiExists) {
        midiIn.ignoreTypes(false, false, false);
        midiIn.addListener(this);
	}
}

//--------------------------------------------------------------
void ofApp::saveSet()
{
    xml.clear();

    xml.addChild("layout");
    xml.setTo("layout");
    for(int i=0; i < pads.size();i++)
    {

        ofXml pad;
        pad.addChild("pad");
        pad.setTo("pad");
        string audiotype;
        if(pads[i]->audioType == InteractivePad::SAMPLER) audiotype = "sampler";
        if(pads[i]->audioType == InteractivePad::GRAINPITCHER) audiotype = "grainpitcher";
        if(pads[i]->audioType == InteractivePad::SYNTH) audiotype = "synth";
        string padtype;
        if(pads[i]->padType == InteractivePad::TOGGLE) padtype = "toggle";
        if(pads[i]->padType == InteractivePad::BUTTON) padtype = "button";
        pad.addValue("type", audiotype);
        pad.addValue("padtype", padtype);
        pad.addValue("name", pads[i]->name);
        pad.addValue("file", pads[i]->filename);
        pad.addValue("sample", pads[i]->samplename);
        pad.addValue("state", pads[i]->bIsOn);
        pad.addValue("volume", pads[i]->volume);
        pad.addValue("FX1send", engine.getFX1Send(i).getValue());


        if(pads[i]->audioType == InteractivePad::SYNTH) {
            //vector<ControlParameter> synthParameters = synths[pads[i]->instrumentId]->getParameters();
            vector<ControlParameter> synthParameters = engine.getSynth(pads[i]->instrumentId).getParameters();

            ofXml params;
            params.addChild("parameters");
            params.setTo("parameters");
            if(synthParameters.size() > 0) {
                for(int j = 0; j < synthParameters.size(); j++){
                    params.addValue("param"+ofToString(j), synthParameters.at(j).getValue());
                }
            }
            pad.addXml(params);
        }

        xml.addXml(pad);

    }

    xml.save("layout.xml");
}

void ofApp::updateLeap()
{
#ifdef USE_LEAP
	fingersFound.clear();
	simpleHands = leap.getSimpleHands();
	if( leap.isFrameNew() && simpleHands.size() ){

		leap.setMappingX(-230, 230, 0, ofGetWidth());
		leap.setMappingY(90, 490, 0, ofGetHeight());
		leap.setMappingZ(-150, 150, -200, 200);
	}
	leap.markFrameAsOld();

	for(int i = 0; i < simpleHands.size(); i++){
	   // simpleHands[i].debugDraw();
		for(int j = 0; j < simpleHands[i].fingers.size(); j++){
				  // ofCircle(simpleHands[i].fingers[j].pos,20);
		}

		if(simpleHands[i].fingers.size() >= 1) {
			leapx = simpleHands[i].fingers[0].pos.x ;
			leapy = simpleHands[i].fingers[0].pos.y ;
			leapx = ofClamp(leapx,0,ofGetWidth());
			leapx = ofClamp(leapy,0,ofGetHeight());

	float x = leapx;
	float y = leapy;

	//cout << "leapx = " << leapx << " leapy" << leapy << endl;

	speed = ((double ) x / ofGetWidth() * 4.0) - 2.0;
	speed = ofClamp(speed,-2,2);
	grainLength = ((double) y / ofGetHeight() * 0.5) + 0.0001;
	grainLength = ofClamp(grainLength,0.0001,0.5);
	rate = ((double) x / ofGetWidth() * 2.0);
	rate = ofClamp(rate,0,2);

		}
	}
#endif	
}

//--------------------------------------------------------------
void ofApp::exit(){
    masterVolume = 0.0f;
    saveSet();
    ofSleepMillis(500);
}
