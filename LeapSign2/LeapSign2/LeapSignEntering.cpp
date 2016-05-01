// LeapSignEntering.cpp : ʵ���ļ�
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

//��ȫ�ֱ������ڱ��¼������ʱ�����Ѿ�����
uint16_t dataExist = 0;
//��ȫ�ֱ������ڱ���Ƿ��Ѿ�¼�룬��¼����Ϊ1������Ϊ0������¼�����༭�����
uint16_t entered = 0;
// LeapSignEntering �Ի���

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


// LeapSignEntering ��Ϣ�������
BOOL LeapSignEntering::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
		
	controller.addListener(listener);

	// �ó���һֱ���У�ֱ������س����Ž���
	cout << "Press Enter to quit..." << endl;
	std::cin.get();

	//���ö�ʱ��1
	if (SetTimer(1, 500, NULL) == 0) {
		AfxMessageBox(_T("��ʱ������δ�ɹ���"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


//���øú������ڻ�ȡ��ǰ�淶���ɼ����ݣ��������ݵĸ�ʽһ���ԡ�eg:����һ��������һ����������ʳָ��1,0,00010,00000, ��16�����ݣ�,<-180.9999, -180.9999, -180.9999>(����޶�33���ַ�)
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
	//strOrder.Delete(strOrder.GetLength()-1, 1);		//ע�ͺ󣺱������Ķ��ţ��������ı����𵽸������ã����Ժ�����ת������
	return strOrder;
}

//¼�밴ť������׶�¼�����ݣ������ı����ݿ�
void LeapSignEntering::OnBnClickedEnteringButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	dataExist = 0;				//��ȫ�ֱ������ڱ��¼������ʱ�����Ѿ�����
	entered = 1;				//ȫ�ֱ������ڱ��¼�����༭���Ƿ����
	char chTemp[64];
	CString strGetMean;
	CStdioFile txtSqlFile;
	CFileException* pError = NULL;
	ifstream txtSql("txtsql.txt", std::ios::in);		//�������using namespace std;�˾佫����
	while (txtSql.getline(chTemp, sizeof(chTemp))) {
		if (memcmp(strOrder(), chTemp, 49) == 0) {      //������ָָ����ǰ16�����ݲ�ͬ����ģ�����ݣ��޷�����ֱ�ӱȽϣ����޸�2015.12.31
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
				txtSqlFile.WriteString(strGetMean);		//���ù淶�ַ���������д���ļ�β��
				m_editMean.Format(_T("^_^�ɹ�¼�룡"));
			}
			else {
				m_editMean.Format(_T("��δ�������ƺ��壡"));
				int sleepTime = 1000;
				while(sleepTime--);						//�ӳ���ʾһ��ʱ��
			}
		}
		else {
			m_editMean.Format(_T("��Ǹ���������Ѵ��ڣ�"));
			int sleepTime = 1000;
			while(sleepTime--);							//�ӳ���ʾһ��ʱ��
		}
	}
	txtSqlFile.Close();
	UpdateData(FALSE);
}

//���ذ�ť
void LeapSignEntering::OnBnClickedReturnButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
	OnOK();
	CLeapSign2Dlg leapSign2Dlg;
	leapSign2Dlg.DoModal();
}

void LeapSignEntering::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strTemp;
	if (nIDEvent == 1) {
		//��Leap�ɼ��������ݱ�������Ӧ�ı���
		UpdateData(TRUE);
		m_editLeftHand = listener.left_hand_number;
		m_editRightHand = listener.right_hand_number;
		m_editLeftFingers.Empty();
		m_editRightFingers.Empty();
		//�ж��Ƿ��Ѿ�¼�룬¼������գ��������
		if (entered == 1) {
			entered = 0;
			m_editMean.Empty();
		}
		//������ָ�¼���ʾ
		strTemp.Format(_T("%d\r\n"), listener.left_finger_number);
		m_editLeftFingers = strTemp;
		if (0 == listener.left_hand_number){
			m_editLeftFingers += strTemp;
		}
		else {
			for (int i = 0; i < listener.left_hand_number; i++){
				strTemp.Format(_T("%d\r\n"), listener.left_right_fingersItem[0][i]);	//�������ֵ6λ���Ǳ����λΪ�㲻��ʾ��ʵ��ֻ�õ���5λ
				strTemp.Delete(0, 1);
				m_editLeftFingers += strTemp;
			}
		}
		strTemp.Empty();		//strTemp�ÿ�
		//������ָ�¼���ʾ
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
