// LeapSignEntering.cpp : 实现文件
//

#include "stdafx.h"
#include "LeapSign2.h"
#include "LeapSignEntering.h"
#include "afxdialogex.h"
#include "LeapListener.h"
#include <fstream>
#include <iostream>
#include <string>
#include "LeapSign2Dlg.h"


using namespace std;
extern LeapListener listener;
extern Controller controller;

//该全局变量用于标记录入数据时发现已经存在
uint16_t dataExist = 0;
//该全局变量用于标记是否已经录入，如录入置为1，否则为0，便于录入后含义编辑框清空
uint16_t entered = 0;
// LeapSignEntering 对话框

IMPLEMENT_DYNAMIC(LeapSignEntering, CDialogEx)

LeapSignEntering::LeapSignEntering(CWnd* pParent /*=NULL*/)
	: CDialogEx(LeapSignEntering::IDD, pParent)
	, m_editLeftFingers(_T(""))
	, m_editLeftHand(0)
	, m_editRightHand(0)
	, m_editRightFingers(_T(""))
	, m_editMean(_T(""))
{

}

LeapSignEntering::~LeapSignEntering()
{
}

void LeapSignEntering::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LEFT_FINGERS_EDIT, m_editLeftFingers);
	DDX_Text(pDX, IDC_LEFT_HAND_EDIT, m_editLeftHand);
	DDX_Text(pDX, IDC_RIGHT_HAND_EDIT, m_editRightHand);
	DDX_Text(pDX, IDC_RIGHT_FINGERS_EDIT, m_editRightFingers);
	DDX_Text(pDX, IDC_MEAN_EDIT, m_editMean);
}


BEGIN_MESSAGE_MAP(LeapSignEntering, CDialogEx)
	ON_BN_CLICKED(IDC_ENTERING_BUTTON, &LeapSignEntering::OnBnClickedEnteringButton)
	ON_BN_CLICKED(IDC_RETURN_BUTTON, &LeapSignEntering::OnBnClickedReturnButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// LeapSignEntering 消息处理程序
BOOL LeapSignEntering::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
		
	controller.addListener(listener);

	// 让程序一直运行，直到输入回车键才结束
	cout << "Press Enter to quit..." << endl;
	std::cin.get();

	//设置定时器1
	if (SetTimer(1, 500, NULL) == 0) {
		AfxMessageBox(_T("定时器设置未成功！"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


//调用该函数用于获取当前规范化采集数据，保持数据的格式一致性。eg:左手一个，右手一个，左手伸食指：1,0,00010,00000, （16个数据）,<-180.9999, -180.9999, -180.9999>(最大限度33个字符)
CString LeapSignEntering::strOrder()
{
	CString strOrder, strTemp;
	strOrder.Format("%d,%d,",listener.left_hand_number, listener.right_hand_number);
	if (0 == listener.left_hand_number){
		strTemp.Format(_T("00000,"));
		strOrder += strTemp;
	}
	for (int i = 0; i < listener.left_hand_number; i++){
		strTemp.Format("%d,", listener.left_right_fingersItem[0][i]);
		strTemp.Delete(0, 1);
		strOrder += strTemp;
	}
	if (0 == listener.right_hand_number){
		strTemp.Format(_T("00000,"));
		strOrder += strTemp;
	}
	for (int i = 0; i < listener.right_hand_number; i++){
		strTemp.Format("%d,", listener.left_right_fingersItem[1][i]);
		strTemp.Delete(0, 1);
		strOrder += strTemp;
	}
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 5; j++){
			strTemp.Format("%c",listener.fingers_position[i][j].toString());
			strOrder += strTemp;
		}
	}
	//strOrder.Delete(strOrder.GetLength()-1, 1);		//注释后：保留最后的逗号，用于在文本中起到隔断作用，对以后数据转移有利
	return strOrder;
}

//录入按钮：试验阶段录入数据，存入文本数据库
void LeapSignEntering::OnBnClickedEnteringButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	dataExist = 0;				//该全局变量用于标记录入数据时发现已经存在
	entered = 1;				//全局变量用于标记录入后含义编辑框是否清空
	char chTemp[64];
	CString strGetMean;
	CStdioFile txtSqlFile;
	CFileException* pError = NULL;
	ifstream txtSql("txtsql.txt", std::ios::in);		//如果不加using namespace std;此句将出错
	while (txtSql.getline(chTemp, sizeof(chTemp))) {
		if (memcmp(strOrder(), chTemp, 49) == 0) {      //由于手指指向与前16个数据不同，是模糊数据，无法采用直接比较，需修改2015.12.31
			dataExist = 1;
			break;
		}
	}
	txtSql.close();
	if (txtSqlFile.Open(_T("txtsql.txt"), CFile::typeText|CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite, pError)){
		if (dataExist == 0) {
			GetDlgItemText(IDC_MEAN_EDIT, strGetMean);
			if (strGetMean != "") {
				txtSqlFile.SeekToEnd();
				strGetMean = strOrder() + strGetMean + "\n";
				txtSqlFile.WriteString(strGetMean);		//调用规范字符串函数，写入文件尾部
				m_editMean.Format(_T("^_^成功录入！"));
			}
			else {
				m_editMean.Format(_T("您未输入手势含义！"));
				int sleepTime = 1000;
				while(sleepTime--);						//延长显示一段时间
			}
		}
		else {
			m_editMean.Format(_T("抱歉，该手势已存在！"));
			int sleepTime = 1000;
			while(sleepTime--);							//延长显示一段时间
		}
	}
	txtSqlFile.Close();
	UpdateData(FALSE);
}

//返回按钮
void LeapSignEntering::OnBnClickedReturnButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	OnOK();
	CLeapSign2Dlg leapSign2Dlg;
	leapSign2Dlg.DoModal();
}

void LeapSignEntering::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTemp;
	if (nIDEvent == 1) {
		//将Leap采集到的数据保存至相应的变量
		UpdateData(TRUE);
		m_editLeftHand = listener.left_hand_number;
		m_editRightHand = listener.right_hand_number;
		m_editLeftFingers.Empty();
		m_editRightFingers.Empty();
		//判断是否已经录入，录入则清空，否则不清空
		if (entered == 1) {
			entered = 0;
			m_editMean.Empty();
		}
		//左手手指事件显示
		strTemp.Format(_T("%d\r\n"), listener.left_finger_number);
		m_editLeftFingers = strTemp;
		if (0 == listener.left_hand_number){
			m_editLeftFingers += strTemp;
		}
		else {
			for (int i = 0; i < listener.left_hand_number; i++){
				strTemp.Format(_T("%d\r\n"), listener.left_right_fingersItem[0][i]);	//保存的数值6位数是避免高位为零不显示，实际只用到低5位
				strTemp.Delete(0, 1);
				m_editLeftFingers += strTemp;
			}
		}
		strTemp.Empty();		//strTemp置空
		//右手手指事件显示
		strTemp.Format(_T("%d\r\n"), listener.right_finger_number);
		m_editRightFingers = strTemp;
		if (0 == listener.right_hand_number){
			m_editRightFingers += strTemp;
		}
		else {
			for (int i = 0; i < listener.right_hand_number; i++){
				strTemp.Format(_T("%d\r\n"), listener.left_right_fingersItem[1][i]);
				strTemp.Delete(0, 1);
				m_editRightFingers += strTemp;
			}
		}

		UpdateData(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}
