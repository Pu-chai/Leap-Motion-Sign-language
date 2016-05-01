
// LeapSign2Dlg.cpp : 实现文件
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
// CLeapSign2Dlg 对话框



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


// CLeapSign2Dlg 消息处理程序

BOOL CLeapSign2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//启动leap motion
	controller.addListener(listener);
	//设置定时器0
	if (SetTimer(0, 500, NULL) == 0) {
		AfxMessageBox(_T("定时器设置未成功！"));
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLeapSign2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLeapSign2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLeapSign2Dlg::OnBnClickedEnteringButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(0);
	OnOK();
	LeapSignEntering leapSignEnteringDlg;
	leapSignEnteringDlg.DoModal();
}


void CLeapSign2Dlg::OnBnClickedTranslatedButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(0);
	OnOK();
	LeapSignTranslated leapSignTranslatedDlg;
	leapSignTranslatedDlg.DoModal();
}


void CLeapSign2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
