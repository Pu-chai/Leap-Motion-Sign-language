
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
	//д���leap motion�ĳ�ʼ�����Ա���֮��������ʾ�ڶԻ�����
}

//�򿪶����ж���ʶ��
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
	//ÿһ֡���Լ�¼������ʼ����ֵΪ��
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
	//��ȡ����һ֡���ݣ����ҷ���һЩ������Ϣ
	const Frame frame = controller.frame();
	cout << "Frame id: " << frame.id()
		<<", timestamp: "<< frame.timestamp()
		<<", hands "     << frame.hands().count()
		<<", fingers "    << frame.fingers().count()
		<<", tools "     << frame.tools().count() << endl;

	//�ǿգ���⵽��
	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl){
		//Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		cout << std::string(2, ' ') << handType << ", id: " << hand.id() << endl;

		//�ж����ֻ������֣�������м�������֤�����ֱַ𲻳���2��
		if (hand.isLeft())	left_hand_number++;
		else				right_hand_number++;

		//Get fingers ������ʾ5����ָ
		const FingerList fingers = hand.fingers();
		//������ָ��ָ���ж��������Щ��ָ����ʾ�����ĺ���
		if (!fingers.isEmpty()){
			cout << endl;
			int fingerNumber = 0;
			//��ʾÿ��ָ����ָ��,��0~1֮���С������Ƕ�
			for (int i = 0; i < fingers.count(); ++i) {
				const Finger finger = fingers[i];

				//ʹ�ùٷ�API�и�����isExtended()����ʶ�����������ָ����л����Ⱥ�Ĵ���
				if (finger.isExtended())
				{
					cout << "The finger is " << fingerNames[finger.type()] << endl;
					//��¼���ֻ�������ָ������¼���δʶ����ָΪ��ֵ�㣬�����Ӧ��ָ����ʮ��ǧ���������ֻ���ʳָ��д��100000+00010
					if(hand.isLeft()){
						left_right_fingersItem[0][left_hand_number - 1] += (int) pow(10.0, finger.type() + 0.0);
						//��¼����ÿ����ָָ��
						fingers_position[left_hand_number - 1][finger.type()] = finger.tipPosition();
						left_finger_number++;
					}
					else {
						left_right_fingersItem[1][right_hand_number - 1] += (int) pow(10.0, -finger.type() + 4.0);	//��ָ��Ӧ��ֵȡ�������ټ�4����õ�01000���Գƣ����û�������Ѻ�
						//��¼����ÿ����ָָ��
						fingers_position[right_hand_number - 1][finger.type()] = finger.tipPosition();
						right_finger_number++;
					}
				}
			}
			//���ֲ�������������������ڴ��ݵ��ռ�������ڶԻ�������ʾ
			frame_id = frame.id();
			cout << endl << "the number is: " <<fingerNum[fingerNumber - 1] << "." << endl;
		}
	}

	// ��ȡ����
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
 
				// �������һ֡�����ĽǶ�
				float sweptAngle = 0;
				if (circle.state() != Gesture::STATE_START) {
					CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
					//progress()��ʾ��ָ��ת�˼�Ȧ
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
				//�ж���ָ��������
				Vector swipeDirection = swipe.direction();
				if (swipeDirection.x < -0.5) {
					left_swipe_direction = 1;			//���󻬶��ñ�־λ	
				}
				if (swipeDirection.x  > 0.5) {
					right_swipe_direction = 1;			//���һ����ñ�־λ
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
