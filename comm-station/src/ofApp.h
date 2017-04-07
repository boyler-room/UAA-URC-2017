#include "ofMain.h"
#include "ofxGamepad.h"
#include "ofxTCPClient.h"
#include "ofxGuiExtended.h"
#include "IPVideoGrabber.h"

struct panel_dimensions {
	panel_dimensions() {}
	panel_dimensions(const float & width) : width(width), height(0) {}
	panel_dimensions(const float & width, const float & height) : width(width), height(height) {}
	float width;
	float height; //unused
};

struct roverstate {
	//arm
	ofParameter<float> fwd_x, fwd_y, fwd_z;
	ofParameter<float> inv_x, inv_y, inv_z;
	//hand
	ofParameter<float> rotation, clamp;
	//drive
	ofParameter<float> motor1, motor2, motor3, motor4, battery;
};

struct gpadstate {
	float lstickx;
	float lsticky;
	float rstickx;
	float rsticky;
	float ltrigger;
	float rtrigger;
	struct buttons {
		bool dpadup;//		0x0001
		bool dpaddown;//	0x0002
		bool dpadleft;//	0x0004
		bool dpadright;//	0x0008
		bool start;//		0x0010
		bool back;//		0x0020
		bool lstick;//		0x0040
		bool rstick;//		0x0080
		bool lshoulder;//	0x0100
		bool rshoulder;//	0x0200
		bool a;//			0x1000
		bool b;//			0x2000
		bool x;//			0x4000
		bool y;//			0x8000
	} b;
	void setstate(Gamepad g);
	bool compare(gpadstate g);
	string display();
};

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw() {}

	void windowResized(int w, int h);
	void keyPressed(int key){}
	void keyReleased(int key);
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
	void mouseEntered(int x, int y){}
	void mouseExited(int x, int y){}
	void dragEvent(ofDragInfo dragInfo){}
	void gotMessage(ofMessage msg){}

	void tcpInit();
	void streamInit();
	void gamepadInit();
	void guiInit();
	void loadConfig();

	void tcpLoop();
	void streamLoop();
	void gamepadLoop();
	void guiLoop();

	//Rover state
	roverstate roh;

	//TCP
	bool tcpConnected = false;
	string roverAddress = "127.0.0.1";// "192.168.112.101";
	int tcpPort = 9999, modPort = 8888;
	ofxTCPClient tcpClient, modClient;
	ofParameter<string> prompt, log[5];
	void tcpConnect();
	void commandSent();

	//stream
	static const int camct = 4;
	bool streamConnected[camct] = { false };
	int streamPort = 8080;
	ofImage stream[camct], maincam;
	ofxGuiGraphics* maincamhandle;
	int mainpt = 0;
	ofx::Video::IPVideoGrabber* streamGrabber[camct];
	void streamConnect();
	void camprev();
	void camnext();

	//gamepad
	bool gamepadConnected = false;
	Gamepad gamepad;
	gpadstate gplast, gpcurr;

	//GUI
	float aspect_ratio, haspectrat;
	ofImage hand;
	map<string, ofxGuiPanel*> panels;
	map<string, panel_dimensions> panels_info;
	ofxGui gui;

	//gui groups
	ofxGuiGroup *arm_FK, *arm_IK;
	ofxGuiGroup *hand_control, *hand_image;
	ofxGuiGroup *strm, *tcp, *modprompt;
	ofxGuiContainer *ip;
	ofxGuiContainer *main_Camera, *other_Cameras;
	ofxGuiContainer *motor_Container;

	//gui setup
	void positionPanels(int w,int h);
	void setup_arm_panel();
	void setup_cam_panel();
	void setup_drive_panel();
	void setup_hand_panel();
	void setup_comm_panel();

	//buttons
	ofxGuiButton *tcp_connect, *strm_connect;
	ofxGuiButton *cam_prev, *cam_next;
	ofxGuiButton *sendcmd;
};