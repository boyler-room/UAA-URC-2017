#include "ofApp.h"

void ofApp::setup_arm_panel()
{
	arm_FK = panels["Arm Controls"]->addGroup("Forward Kinematics");
		arm_FK->add(roh.fwd_x.set("X-Axis", 50, 0, 360), ofJson({ { "precision",2 } }));
		arm_FK->add(roh.fwd_y.set("Y-Axis", 50, 0, 360), ofJson({ { "precision",2 } }));
		arm_FK->add(roh.fwd_z.set("Z-Axis", 50, 0, 360), ofJson({ { "precision",2 } }));

	arm_IK = panels["Arm Controls"]->addGroup("Inverse Kinematics");
		arm_IK->add(roh.inv_x.set("X-Axis", 50, 0, 360), ofJson({ { "precision",2 } }));
		arm_IK->add(roh.inv_y.set("Y-Axis", 50, 0, 360), ofJson({ { "precision",2 } }));
		arm_IK->add(roh.inv_z.set("Z-Axis", 50, 0, 360), ofJson({ { "precision",2 } }));

	panels["Arm Controls"]->addFpsPlotter();
}
void ofApp::setup_cam_panel()
{
	main_Camera = panels["Cameras"]->addContainer("Main Camera", ofJson({ { "direction","horizontal" } }));
		cam_prev = main_Camera->add<ofxGuiButton>("<", ofJson({ { "type","fullsize" },{ "width",20 },{ "text-align","center" } }));
		cam_prev->addListener(this, &ofApp::camprev);
		maincamhandle = main_Camera->add<ofxGuiGraphics>("Main Camera", &maincam.getTexture());
		cam_next = main_Camera->add<ofxGuiButton>(">", ofJson({ { "type","fullsize" },{ "width",20 },{ "text-align","center" } }));
		cam_next->addListener(this, &ofApp::camnext);

	other_Cameras = panels["Cameras"]->addContainer("Other Cameras", ofJson({ { "direction","horizontal" } }));
		other_Cameras->add<ofxGuiGraphics>("Camera 1", &stream[0].getTexture());
		other_Cameras->add<ofxGuiGraphics>("Camera 2", &stream[1].getTexture());
		other_Cameras->add<ofxGuiGraphics>("Camera 3", &stream[2].getTexture());
		other_Cameras->add<ofxGuiGraphics>("Camera 4", &stream[3].getTexture());
}
void ofApp::setup_drive_panel()
{
	motor_Container = panels["Motor Feedback"]->addContainer("Motors", ofJson({ { "direction","horizontal" } }));
		motor_Container->add(roh.motor1.set("Left Front", ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 },{ "type", "circular" },{ "height",100 } }));
		motor_Container->add(roh.motor2.set("Right Front", ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 },{ "type", "circular" },{ "height",100 } }));
		motor_Container->add(roh.motor3.set("Left Rear", ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 },{ "type", "circular" },{ "height",100 } }));
		motor_Container->add(roh.motor4.set("Right Rear", ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 },{ "type", "circular" },{ "height",100 } }));
		motor_Container->add(roh.battery.set("Battery Level", 80, 0, 100), ofJson({ { "precision",2 },{ "width",40 } }));
}
void ofApp::setup_hand_panel()
{
	hand_image = panels["Hand Controls"]->addGroup("3D Model");
		hand.load("zahando.jpg");
		haspectrat = hand.getHeight() / hand.getWidth();
		hand_image->add<ofxGuiGraphics>("Hand Model", &hand.getTexture());

	hand_control = panels["Hand Controls"]->addGroup("Hand Controls");
		hand_control->add(roh.rotation.set("Rotation", ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 },{ "type", "circular" },{ "width",140 },{ "height",100 } }));
		hand_control->add(roh.clamp.set("Clamp", 80, 0, 100), ofJson({ { "precision",2 },{ "width",200 } }));
}
void ofApp::setup_comm_panel()
{
	ip = panels["Rover Connection"]->addContainer("Rover IP");
		ofParameter<string> rip;
		ip->add(rip.set("Rover IP: " + roverAddress));

	tcp = panels["Rover Connection"]->addGroup("Rover");
		ofParameter<string> rover, stream1, stream2, stream3, stream4;
		tcp_connect = tcp->add<ofxGuiButton>("", ofJson({ { "type","fullsize" },{ "text-align","center" } }));
		tcp_connect->addListener(this, &ofApp::tcpConnect);

	strm = panels["Rover Connection"]->addGroup("Stream");
		strm_connect = strm->add<ofxGuiButton>("", ofJson({ { "type","fullsize" },{ "text-align","center" } }));
		strm_connect->addListener(this, &ofApp::streamConnect);
		strm->add(stream1.set("Stream 1", ""));
		strm->add(stream2.set("Stream 2", ""));
		strm->add(stream3.set("Stream 3", ""));
		strm->add(stream4.set("Stream 4", ""));

	modprompt = panels["Rover Connection"]->addGroup("Module Manager");
		for(int i=0;i<5;i++){
			modprompt->add<ofxGuiLabel>(log[i].set(""));
		}
		ofxGuiContainer* f = modprompt->addContainer("bpan", ofJson({ { "direction","horizontal" } }));
		f->add<ofxGuiTextField>(prompt.set(">", ""), ofJson({ {"text-align", "left"} }));
		sendcmd = f->add<ofxGuiButton>("Send", ofJson({ { "type","fullsize" },{ "text-align","center" } }));
		sendcmd->addListener(this, &ofApp::commandSent);

	//ofxGuiContainer *buttons = group->addContainer("", ofJson({ { "direction","horizontal" } }));
	//buttons->add<ofxGuiButton>(state ? "Disconnect" : "Connect", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
}

void ofApp::positionPanels(int w, int h)
{
	panels["Hand Controls"]->setPosition(.05*w/4, 10);
	panels["Arm Controls"]->setPosition(panels["Hand Controls"]->getShape().getBottomLeft() + ofPoint(0, 10));
	panels["Cameras"]->setPosition(panels["Hand Controls"]->getShape().getTopRight() + ofPoint(.05*w / 4, 0));
	panels["Motor Feedback"]->setPosition(panels["Cameras"]->getShape().getBottomLeft() + ofPoint(0, 10));
	panels["Rover Connection"]->setPosition(panels["Cameras"]->getShape().getTopRight() + ofPoint(.05*w / 4, 0));
}

void ofApp::windowResized(int w, int h) {
	panels["Arm Controls"]->setWidth(w*panels_info["Arm Controls"].width);
	panels["Motor Feedback"]->setWidth(w*panels_info["Motor Feedback"].width);
	panels["Hand Controls"]->setWidth(w*panels_info["Hand Controls"].width);
	panels["Rover Connection"]->setWidth(w*panels_info["Rover Connection"].width);
	panels["Cameras"]->setWidth(w*panels_info["Cameras"].width);
	panels["Cameras"]->setHeight(h*panels_info["Cameras"].width*aspect_ratio);

	hand_image->getControl("Hand Model")->setConfig(ofJson({ { "height", (hand_image->getWidth()-8)*haspectrat } }));
	
	maincamhandle->setConfig(ofJson({ { "width", w*panels_info["Cameras"].width-72 },{ "height", h*(panels_info["Cameras"].width)*aspect_ratio } }));
	other_Cameras->getControl("Camera 1")->setConfig(ofJson({ { "width", (main_Camera->getWidth()-32) / camct },{ "height", h*(panels_info["Cameras"].width / camct)*aspect_ratio } }));
	other_Cameras->getControl("Camera 2")->setConfig(ofJson({ { "width", (main_Camera->getWidth() - 32) / camct },{ "height", h*(panels_info["Cameras"].width / camct)*aspect_ratio } }));
	other_Cameras->getControl("Camera 3")->setConfig(ofJson({ { "width", (main_Camera->getWidth() - 32) / camct },{ "height", h*(panels_info["Cameras"].width / camct)*aspect_ratio } }));
	other_Cameras->getControl("Camera 4")->setConfig(ofJson({ { "width", (main_Camera->getWidth() - 32) / camct },{ "height", h*(panels_info["Cameras"].width / camct)*aspect_ratio } }));

	motor_Container->setWidth(main_Camera->getWidth());
	motor_Container->getControl("Left Front")->setConfig(ofJson({{ "width",(main_Camera->getWidth()-40-40)/4 } }));
	motor_Container->getControl("Right Front")->setConfig(ofJson({{"width",(main_Camera->getWidth()-40-40)/4 } }));
	motor_Container->getControl("Left Rear")->setConfig(ofJson({ { "width",(main_Camera->getWidth()-40-40)/4 } }));
	motor_Container->getControl("Right Rear")->setConfig(ofJson({{ "width",(main_Camera->getWidth()-40-40)/4 } }));

	((ofxGuiContainer*)(modprompt->getControl("bpan")))->getControl(">")->setConfig(ofJson({ {"width",strm->getWidth()-24-40} }));

	positionPanels(w,h);
}

std::pair<string, panel_dimensions> panel_pairer(const string & name, const float & width, const float & height = 0) {
	return std::pair<string, panel_dimensions>(name, panel_dimensions(width, height));
}

ofxGuiPanel * gui_panel(ofxGui & gui, const string & name, const float & width) {
	return gui.addPanel(name, ofJson({ { "width",ofGetWidth()*width } }));
}

void ofApp::guiInit()
{
	ofSetFrameRate(60);

	panels_info = map<string, panel_dimensions>{
		panel_pairer("Arm Controls", .2),
		panel_pairer("Hand Controls", .2),
		panel_pairer("Cameras", .55, .5*aspect_ratio),
		panel_pairer("Motor Feedback", .55),
		panel_pairer("Rover Connection", .2),
	};
	for (auto panel : panels_info) { //mapping the panels info to the panels in the class, ofxGuiPanels
		panels[panel.first] = gui_panel(gui, panel.first, panel.second.width);
	}

	//Panel Initialization
	setup_arm_panel();
	setup_cam_panel();
	setup_drive_panel();
	setup_hand_panel();
	setup_comm_panel();
}

void ofApp::tcpInit()
{
	tcpClient.setup(roverAddress, tcpPort);
	tcpClient.setMessageDelimiter("\n");
	if (tcpConnected = tcpClient.isConnected()) {
		//get initial rover state
	}
}

void ofApp::streamInit()
{
	for (int i = 0; i < camct; i++) {
		delete streamGrabber[i];
		streamGrabber[i]=new ofx::Video::IPVideoGrabber();
		streamGrabber[i]->setURI("http://"+roverAddress+":"+to_string(streamPort+i)+"/?action=stream");
		streamGrabber[i]->connect();

		streamGrabber[i]->update();
		stream[i] = *(streamGrabber[i]->getFrame());
		streamConnected[i] = streamGrabber[0]->isConnected();
	}maincam = *(streamGrabber[mainpt]->getFrame());
	aspect_ratio = stream[0].getHeight() / stream[0].getWidth();
}

void ofApp::gamepadInit()
{
	if(gamepadConnected = gamepad.Refresh()){
		std::cout << "Controller connected on port " << gamepad.GetPort() << std::endl;
		gpcurr.setstate(gamepad);
	}
}

void ofApp::loadConfig()
{
	fstream fcon;
	fcon.open("ip.cfg", ios::in);
	if(fcon.is_open()){
		fcon >> roverAddress >> tcpPort >> streamPort >> modPort;
	}else{
		cout << "Error loading config\n";
		fcon.open("ip.cfg", ios::out);
		if(fcon.is_open()) fcon << roverAddress <<'\n'<< tcpPort <<'\n'<< streamPort << '\n' << modPort;
	}fcon.close();
}