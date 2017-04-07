#include "ofApp.h"

void ofApp::streamConnect()
{
	if(streamConnected[mainpt]){
		for (int i = 0; i < camct; i++) { if (streamConnected[i]) streamGrabber[i]->disconnect(); streamConnected[i] = streamGrabber[i]->isConnected(); }
	}else{
		streamInit();
	}
}

void ofApp::tcpConnect()
{
	if (tcpConnected = tcpClient.isConnected()) {
		tcpClient.close();
	}
	else {
		tcpClient.setup(roverAddress, tcpPort);
		tcpClient.setMessageDelimiter("\n");
	}
}

void ofApp::commandSent()
{
	modClient.setup(roverAddress, modPort);
	modClient.setMessageDelimiter("\t\0");

	for (int i = 0; i < 4; i++) {
		log[i] = log[i + 1];
	}

	if (modClient.isConnected()) {
		modClient.send(prompt);
		log[4] = modClient.receive();
		int p;
		while((p=log[4].toString().find("\n")) != string::npos){
			for (int i = 0; i < 4; i++) {
				log[i] = log[i + 1];
			}log[4] = log[3].toString().substr(p+1);
			log[3] = log[3].toString().erase(p);
		}
		modClient.send("close");
		modClient.close();
	}else log[4] = "Error Connecting To Module Manager!";
	prompt = "";
}

void ofApp::camprev()
{
	mainpt = (mainpt + 3) % 4;
}
void ofApp::camnext()
{
	mainpt = (mainpt + 1) % 4;
}

void ofApp::guiLoop()
{
	tcp_connect->setName(tcpConnected ? "Disconnect" : "Connect");
	tcp_connect->setNeedsRedraw();
	strm_connect->setName(streamConnected[mainpt] ? "Disconnect" : "Connect");
	strm_connect->setNeedsRedraw();
	maincamhandle->setName("Camera " + to_string(mainpt+1));
	maincamhandle->setNeedsRedraw();
	strm->getControl("Stream 1")->getParameter().cast<string>().set(streamConnected[0] ? "connected" : "disconnected");
	strm->getControl("Stream 2")->getParameter().cast<string>().set(streamConnected[1] ? "connected" : "disconnected");
	strm->getControl("Stream 3")->getParameter().cast<string>().set(streamConnected[2] ? "connected" : "disconnected");
	strm->getControl("Stream 4")->getParameter().cast<string>().set(streamConnected[3] ? "connected" : "disconnected");
}

void ofApp::tcpLoop()
{
	static uint64_t dt;
	if (roh.fwd_x != 50.0) cout << roh.fwd_x << "\n";

	if (tcpConnected = tcpClient.isConnected()) {
		//tcpClient.send(request);
		//tcpClient.receive();
		//get rover state
		if(gpcurr.compare(gplast)){
			tcpClient.send(to_string((int)((gpcurr.lsticky+1)*(255)/(2))) + " " + to_string((int)((gpcurr.rsticky+1)*(255)/(2))));
			//send gamepad state
		}
	}
	else if ((ofGetElapsedTimeMillis() - dt) > 5000) {
		//tcpConnect();
		dt = ofGetElapsedTimeMillis();
	}
}

void ofApp::streamLoop()
{
	for (int i = 0; i < camct; i++) {
		if(!(streamConnected[i] = streamGrabber[i]->isConnected())) continue;
		streamGrabber[i]->update();
		stream[i] = *streamGrabber[i]->getFrame();
	}maincam = *(streamGrabber[mainpt]->getFrame());
}

void ofApp::gamepadLoop()
{
	static bool msgdisplayed;
	if(gamepadConnected = gamepad.Refresh()){
		if(msgdisplayed){
			std::cout << "Controller connected on port " << gamepad.GetPort() << std::endl;
			msgdisplayed = false;
		}
		gplast = gpcurr;
		gpcurr.setstate(gamepad);
		if(gpcurr.compare(gplast)){
			cout << gpcurr.display();
		}
	}else if(!msgdisplayed){
		std::cout << "Please connect Xbox 360 controller." << std::endl;
		msgdisplayed = true;
	}
}