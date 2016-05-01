
// LeapSign2Dlg.h : 头文件
//

#pragma once


// CLeapSign2Dlg 对话框
class CLeapSign2Dlg : public CDialogEx
{
// 构造
public:
	CLeapSign2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LEAPSIGN2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEnteringButton();
	afx_msg void OnBnClickedTranslatedButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
