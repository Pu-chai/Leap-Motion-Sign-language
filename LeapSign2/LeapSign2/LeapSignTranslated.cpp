// LeapSignTranslated.cpp : ʵ���ļ�
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

// LeapSignTranslated �Ի���

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


// LeapSignTranslated ��Ϣ�������
BOOL LeapSignTranslated::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
		
	controller.addListener(listener);

	// �ó���һֱ���У�ֱ������س����Ž���
	cout << "Press Enter to quit..." << endl;
	std::cin.get();

	//���ö�ʱ��2
	if (SetTimer(2, 500, NULL) == 0) {
		AfxMessageBox(_T("��ʱ������δ�ɹ���"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


//���øú������ڻ�ȡ��ǰ�淶���ɼ����ݣ��������ݵĸ�ʽһ���ԡ�eg:����һ��������һ����������ʳָ��1,0,00010,00000, ��16�����ݣ�
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
	//strOrder.Delete(strOrder.GetLength()-1, 1);		//ע�ͺ󣺱������Ķ��ţ��������ı����𵽸������ã����Ժ�����ת������
	return strOrder;
}

//ǰ��¼�밴ť
void LeapSignTranslated::OnBnClickedGoEnteringButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(2);
	OnOK();
	LeapSignEntering leapSignEnteringDlg;
	leapSignEnteringDlg.DoModal();
}


//���ذ�ť
void LeapSignTranslated::OnBnClickedReturnButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(2);  //�رձ����ڶ�ʱ��
	OnOK();  //�����ݴ����Ի����Ա�������رնԻ���
	CLeapSign2Dlg leapSign2Dlg;  //����һ��CLeapSign2Dlg����
	leapSign2Dlg.DoModal();  //ʵ��DoModal()�����������Ի���
}
//��ʱ���밴ť�����õ�ǰ�������ݱȶ����ݿ������ݣ��Ը����Ʒ���
void LeapSignTranslated::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 2) {
		UpdateData(TRUE);
		if (listener.left_hand_number + listener.right_hand_number == 0) {
			m_editTranslated.Format(_T("������������ҽ���^_^"));
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
					dataFind = 1;									//���ݶԱȳɹ����ҵ���Ӧ����
					strTemp = CA2W(chTemp + 16);
					m_editTranslated = strTemp;
					break;
				}
			}
			if (dataFind == 0) {
				m_editTranslated.Format(_T("��Ǹ��������δ¼�룡"));
			}
			txtSqlFile.close();
			SetDlgItemText(IDC_TRANSLATED_EDIT, m_editTranslated);
		}
		UpdateData(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}

