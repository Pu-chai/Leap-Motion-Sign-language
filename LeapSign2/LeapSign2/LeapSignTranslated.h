#pragma once


// LeapSignTranslated �Ի���

class LeapSignTranslated : public CDialogEx
{
	DECLARE_DYNAMIC(LeapSignTranslated)

public:
	LeapSignTranslated(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LeapSignTranslated();

// �Ի�������
	enum { IDD = IDD_LEAPSIGNTRANSLATED_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	

	DECLARE_MESSAGE_MAP()
public:
	CString m_editTranslated;
	
	afx_msg CString strOrder();
	afx_msg void OnBnClickedGoEnteringButton();
	afx_msg void OnBnClickedReturnButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
