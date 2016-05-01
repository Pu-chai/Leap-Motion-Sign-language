
#include "stdafx.h"
#include "LeapListener.h"
#include "math.h"
LeapListener::LeapListener(void)
{
}


LeapListener::~LeapListener(void)
{
}

void LeapListener::onInit(const Controller& controller) {
	cout << "Initialized" << endl;
	//写入对leap motion的初始化，以便在之后将数据显示在对话框中
}

//打开对所有动作识别
void LeapListener::onConnect(const Controller& controller) {
	cout << "Connected! Hello, Leap user!" << endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapListener::onDisconnect(const Controller& controller) {
	cout << "Disconnected" << endl;
}

void LeapListener::onExit(const Controller& controller) {
	cout << "Exited" << endl;
}

void LeapListener::onFrame(const Controller& controller) {
	//每一帧都对记录变量初始化初值为零
	left_hand_number = 0;
	right_hand_number = 0;
	left_finger_number = 0;
	right_finger_number = 0;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			left_right_fingersItem[i][j] = 100000;
		}
	}
	Vector fingers_position_temp;
	//获取最新一帧数据，并且返回一些基本信息
	const Frame frame = controller.frame();
	cout << "Frame id: " << frame.id()
		<<", timestamp: "<< frame.timestamp()
		<<", hands "     << frame.hands().count()
		<<", fingers "    << frame.fingers().count()
		<<", tools "     << frame.tools().count() << endl;

	//非空，检测到手
	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl){
		//Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		cout << std::string(2, ' ') << handType << ", id: " << hand.id() << endl;

		//判断左手还是右手，对其进行计数，保证左右手分别不超过2个
		if (hand.isLeft())	left_hand_number++;
		else				right_hand_number++;

		//Get fingers 总是显示5个手指
		const FingerList fingers = hand.fingers();
		//根据手指的指向判断伸出了那些手指，显示其代表的含义
		if (!fingers.isEmpty()){
			cout << endl;
			int fingerNumber = 0;
			//显示每个指尖尖端指向,以0~1之间的小数代表角度
			for (int i = 0; i < fingers.count(); ++i) {
				const Finger finger = fingers[i];

				//使用官方API中给出的isExtended()函数识别所伸出的手指，感谢交流群的大神
				if (finger.isExtended())
				{
					cout << "The finger is " << fingerNames[finger.type()] << endl;
					//记录左手或右手手指的伸出事件，未识别到手指为初值零，否则对应手指按个十百千万计数，如只伸出食指则写入100000+00010
					if(hand.isLeft()){
						left_right_fingersItem[0][left_hand_number - 1] += (int) pow(10.0, finger.type() + 0.0);
						//记录左手每个手指指向
						fingers_position[left_hand_number - 1][finger.type()] = finger.tipPosition();
						left_finger_number++;
					}
					else {
						left_right_fingersItem[1][right_hand_number - 1] += (int) pow(10.0, -finger.type() + 4.0);	//手指对应数值取负数，再加4，便得到01000（对称），用户界面更友好
						//记录左手每个手指指向
						fingers_position[right_hand_number - 1][finger.type()] = finger.tipPosition();
						right_finger_number++;
					}
				}
			}
			//将局部变量赋给类变量，用于传递到空间变量，在对话框中显示
			frame_id = frame.id();
			cout << endl << "the number is: " <<fingerNum[fingerNumber - 1] << "." << endl;
		}
	}

	// 获取手势
	const GestureList gestures = frame.gestures();
	for (int g = 0; g < gestures.count(); ++g) {
		Gesture gesture = gestures[g];
		switch (gesture.type()) {
			case Gesture::TYPE_CIRCLE:
			{
				CircleGesture circle = gesture;
				std::string clockwiseness;
 
				if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
					clockwiseness = "clockwise";
				}
				else {
					clockwiseness = "counterclockwise";
				}
 
				// 计算和上一帧经过的角度
				float sweptAngle = 0;
				if (circle.state() != Gesture::STATE_START) {
					CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
					//progress()表示手指旋转了几圈
					sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
				}
				std::cout << "Circle id: " << gesture.id()
					<< ", state: " << gesture.state()
					<< ", progress: " << circle.progress()
					<< ", radius: " << circle.radius()
					<< ", angle " << sweptAngle * RAD_TO_DEG
					<<  ", " << clockwiseness << std::endl;
				break;
			}
		case Gesture::TYPE_SWIPE:
			{
				SwipeGesture swipe = gesture;
				std::cout << "Swipe id: " << gesture.id()
					<< ", state: " << gesture.state()
					<< ", direction: " << swipe.direction()
					<< ", speed: " << swipe.speed() << std::endl;
				//判断手指滑动方向
				Vector swipeDirection = swipe.direction();
				if (swipeDirection.x < -0.5) {
					left_swipe_direction = 1;			//向左滑动置标志位	
				}
				if (swipeDirection.x  > 0.5) {
					right_swipe_direction = 1;			//向右滑动置标志位
				}
				break;
			}
		case Gesture::TYPE_KEY_TAP:
			{
				KeyTapGesture tap = gesture;
				std::cout << "Key Tap id: " << gesture.id()
					<< ", state: " << gesture.state()
					<< ", position: " << tap.position()
					<< ", direction: " << tap.direction()<< std::endl;
				break;
			}
		case Gesture::TYPE_SCREEN_TAP:
			{
				ScreenTapGesture screentap = gesture;
				std::cout << "Screen Tap id: " << gesture.id()
					<< ", state: " << gesture.state()
					<< ", position: " << screentap.position()
					<< ", direction: " << screentap.direction()<< std::endl;
				break;
			}
		default:
			std::cout << "Unknown gesture type." << std::endl;
			break;
		}
	}

	if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
		std::cout << std::endl;
	}

	//Get tools
	const ToolList tools = frame.tools();
	for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl){
		const Tool tool = *tl;
		cout << std::string(2, ' ') << "Tool, id: " << tool.id() << endl;
	}

	if (!frame.hands().isEmpty()) {
    std::cout << std::endl;
  }
}

void LeapListener::onFocusGained(const Controller& controller) {
	cout << "Focus Gained" << endl;
}

void LeapListener::onFocusLost(const Controller& controller) {
	cout << "Focus Lost" << endl;
}

void LeapListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void LeapListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void LeapListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}
