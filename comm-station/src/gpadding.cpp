#include "ofApp.h"

void gpadstate::setstate(Gamepad g) {
	lstickx = g.leftStickX; lsticky = g.leftStickY;
	rstickx = g.rightStickX; rsticky = g.rightStickY;
	ltrigger = g.leftTrigger; rtrigger = g.rightTrigger;
	b.dpadup = (g.GetState()->wButtons) & 0x0001;
	b.dpaddown = (g.GetState()->wButtons) & 0x0002;
	b.dpadleft = (g.GetState()->wButtons) & 0x0004;
	b.dpadright = (g.GetState()->wButtons) & 0x0008;
	b.start = (g.GetState()->wButtons) & 0x0010;
	b.back = (g.GetState()->wButtons) & 0x0020;
	b.lstick = (g.GetState()->wButtons) & 0x0040;
	b.rstick = (g.GetState()->wButtons) & 0x0080;
	b.lshoulder = (g.GetState()->wButtons) & 0x0100;
	b.rshoulder = (g.GetState()->wButtons) & 0x0200;
	b.a = (g.GetState()->wButtons) & 0x1000;
	b.b = (g.GetState()->wButtons) & 0x2000;
	b.x = (g.GetState()->wButtons) & 0x4000;
	b.y = (g.GetState()->wButtons) & 0x8000;
}
bool gpadstate::compare(gpadstate g) {
	return (b.rstick != g.b.rstick) || (lstickx != g.lstickx) || (lsticky != g.lsticky) || (b.lstick != g.b.lstick) || (rstickx != g.rstickx) || (rsticky != g.rsticky)
		|| (ltrigger != g.ltrigger) || (rtrigger != g.rtrigger) || (b.lshoulder != g.b.lshoulder) || (b.rshoulder != g.b.rshoulder)
		|| (b.dpadup != g.b.dpadup) || (b.dpaddown != g.b.dpaddown) || (b.dpadleft != g.b.dpadleft) || (b.dpadright != g.b.dpadright)
		|| (b.a != g.b.a) || (b.b != g.b.b) || (b.x != g.b.x) || (b.y != g.b.y) || (b.start != g.b.start) || (b.back != g.b.back);
}
string gpadstate::display() {
	return "\nlstick:" + to_string(b.lstick) + ", lstickx:" + to_string(lstickx) + ", lsticky:" + to_string(lsticky) + "\n"
		+ "rstick:" + to_string(b.rstick) + ", rstickx:" + to_string(rstickx) + ", rsticky:" + to_string(rsticky) + "\n"
		+ "ltrigger:" + to_string(ltrigger) + ", rtrigger:" + to_string(rtrigger) + ", lshoulder:" + to_string(b.lshoulder) + ", rshoulder:" + to_string(b.rshoulder) + "\n"
		+ "dpadup:" + to_string(b.dpadup) + ", dpaddown:" + to_string(b.dpaddown) + ", dpadleft:" + to_string(b.dpadleft) + ", dpadright:" + to_string(b.dpadright) + "\n"
		+ "A:" + to_string(b.a) + ", B:" + to_string(b.b) + ", X:" + to_string(b.x) + ", Y:" + to_string(b.y) + ", start:" + to_string(b.start) + ", back:" + to_string(b.back) + "\n";
}