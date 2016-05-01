#pragma once


// LeapSignTranslated 对话框

class LeapSignTranslated : public CDialogEx
{
	DECLARE_DYNAMIC(LeapSignTranslated)

public:
	LeapSignTranslated(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LeapSignTranslated();

// 对话框数据
	enum { IDD = IDD_LEAPSIGNTRANSLATED_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	

	DECLARE_MESSAGE_MAP()
public:
	CString m_editTranslated;
	
	afx_msg CString strOrder();
	afx_msg void OnBnClickedGoEnteringButton();
	afx_msg void OnBnClickedReturnButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
