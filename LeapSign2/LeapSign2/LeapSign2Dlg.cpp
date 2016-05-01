
// LeapSign2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LeapSign2.h"
#include "LeapSign2Dlg.h"
#include "afxdialogex.h"
#include "LeapListener.h"

#include "LeapSignEntering.h"
#include "LeapSignTranslated.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
LeapListener listener;
Controller controller;
// CLeapSign2Dlg �Ի���



CLeapSign2Dlg::CLeapSign2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLeapSign2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLeapSign2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLeapSign2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ENTERING_BUTTON, &CLeapSign2Dlg::OnBnClickedEnteringButton)
	ON_BN_CLICKED(IDC_TRANSLATED_BUTTON, &CLeapSign2Dlg::OnBnClickedTranslatedButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLeapSign2Dlg ��Ϣ�������

BOOL CLeapSign2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����leap motion
	controller.addListener(listener);
	//���ö�ʱ��0
	if (SetTimer(0, 500, NULL) == 0) {
		AfxMessageBox(_T("��ʱ������δ�ɹ���"));
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLeapSign2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLeapSign2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLeapSign2Dlg::OnBnClickedEnteringButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(0);
	OnOK();
	LeapSignEntering leapSignEnteringDlg;
	leapSignEnteringDlg.DoModal();
}


void CLeapSign2Dlg::OnBnClickedTranslatedButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(0);
	OnOK();
	LeapSignTranslated leapSignTranslatedDlg;
	leapSignTranslatedDlg.DoModal();
}


void CLeapSign2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 0) {
		UpdateData(TRUE);
		if (listener.left_swipe_direction == 1) {
			listener.left_swipe_direction = 0;
			KillTimer(0);
			OnOK();
			LeapSignEntering leapSignEnteringDlg;
			leapSignEnteringDlg.DoModal();
		}
		if (listener.right_swipe_direction == 1) {
			listener.right_swipe_direction = 0;
			KillTimer(0);
			OnOK();
			LeapSignTranslated leapSignTranslatedDlg;
			leapSignTranslatedDlg.DoModal();
		}
		UpdateData(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}
