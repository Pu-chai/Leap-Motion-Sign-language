#pragma once
#include "Leap.h"

using namespace Leap;
using std::cout;
using std::endl;

class LeapListener : public Listener
{
public:
	int64_t frame_id;
	int left_hand_number;
	int right_hand_number;
	int left_finger_number;				//左手指伸出数量
	int right_finger_number;			//右手指伸出数量
	int left_right_fingersItem[2][2];
	int left_swipe_direction;			//向左滑动
	int right_swipe_direction;			//向左滑动
	int up_swipe_direction;
	int down_swipe_direction;
	Vector fingers_position[4][5];         //20个手指的指向,0左一，1右一，2左二，3右二

	LeapListener(void);
	~LeapListener(void);
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);
};

const std::string fingerNum[] = {"one", "two", "three", "four", "five"};
const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
//const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
//const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};
