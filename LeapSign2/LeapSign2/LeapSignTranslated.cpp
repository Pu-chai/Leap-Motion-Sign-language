// LeapSignTranslated.cpp : 实现文件
//

#include "stdafx.h"
#include "LeapSign2.h"
#include "LeapSignTranslated.h"
#include "afxdialogex.h"
#include "LeapListener.h"
#include <fstream>
#include <iostream>
#include <string>
#include "LeapSign2Dlg.h"
#include "LeapSignEntering.h"

using namespace std;
extern LeapListener listener;
extern Controller controller;

// LeapSignTranslated 对话框

IMPLEMENT_DYNAMIC(LeapSignTranslated, CDialogEx)

LeapSignTranslated::LeapSignTranslated(CWnd* pParent /*=NULL*/)
	: CDialogEx(LeapSignTranslated::IDD, pParent)
	, m_editTranslated(_T(""))
{

}

LeapSignTranslated::~LeapSignTranslated()
{
}

void LeapSignTranslated::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRANSLATED_EDIT, m_editTranslated);
}


BEGIN_MESSAGE_MAP(LeapSignTranslated, CDialogEx)
	ON_BN_CLICKED(IDC_GO_ENTERING_BUTTON, &LeapSignTranslated::OnBnClickedGoEnteringButton)
	ON_BN_CLICKED(IDC_RETURN_BUTTON, &LeapSignTranslated::OnBnClickedReturnButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// LeapSignTranslated 消息处理程序
BOOL LeapSignTranslated::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
		
	controller.addListener(listener);

	// 让程序一直运行，直到输入回车键才结束
	cout << "Press Enter to quit..." << endl;
	std::cin.get();

	//设置定时器2
	if (SetTimer(2, 500, NULL) == 0) {
		AfxMessageBox(_T("定时器设置未成功！"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


//调用该函数用于获取当前规范化采集数据，保持数据的格式一致性。eg:左手一个，右手一个，左手伸食指：1,0,00010,00000, （16个数据）
CString LeapSignTranslated::strOrder()
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
	//strOrder.Delete(strOrder.GetLength()-1, 1);		//注释后：保留最后的逗号，用于在文本中起到隔断作用，对以后数据转移有利
	return strOrder;
}

//前往录入按钮
void LeapSignTranslated::OnBnClickedGoEnteringButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(2);
	OnOK();
	LeapSignEntering leapSignEnteringDlg;
	leapSignEnteringDlg.DoModal();
}


//返回按钮
void LeapSignTranslated::OnBnClickedReturnButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(2);  //关闭本窗口定时器
	OnOK();  //将数据传给对话框成员变量，关闭对话框
	CLeapSign2Dlg leapSign2Dlg;  //声明一个CLeapSign2Dlg对象
	leapSign2Dlg.DoModal();  //实现DoModal()方法，产生对话框
}
//定时翻译按钮：利用当前捕获数据比对数据库中数据，对该手势翻译
void LeapSignTranslated::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 2) {
		UpdateData(TRUE);
		if (listener.left_hand_number + listener.right_hand_number == 0) {
			m_editTranslated.Format(_T("请您用手语和我交流^_^"));
		}
		else {
			m_editTranslated.Empty();
			int16_t dataFind = 0;
			ifstream txtSqlFile("txtsql.txt", std::ios::in);
			char chTrans[32], chTemp[64];
			CString strTemp, strCurr;
	
			memset(chTrans, 0, sizeof(bool)*16);
			strCurr = strOrder();
			strncpy_s(chTrans, _countof(chTrans), strCurr, 16);
			while (txtSqlFile.getline(chTemp, sizeof(chTemp))){
				if (memcmp(chTrans, chTemp, 16) == 0) {
					dataFind = 1;									//数据对比成功，找到对应含义
					strTemp = CA2W(chTemp + 16);
					m_editTranslated = strTemp;
					break;
				}
			}
			if (dataFind == 0) {
				m_editTranslated.Format(_T("抱歉，该手势未录入！"));
			}
			txtSqlFile.close();
			SetDlgItemText(IDC_TRANSLATED_EDIT, m_editTranslated);
		}
		UpdateData(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}

