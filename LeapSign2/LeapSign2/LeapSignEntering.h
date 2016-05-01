#pragma once


// LeapSignEntering �Ի���

class LeapSignEntering : public CDialogEx
{
	DECLARE_DYNAMIC(LeapSignEntering)

public:
	LeapSignEntering(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LeapSignEntering();

// �Ի�������
	enum { IDD = IDD_LEAPSIGNENTERING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
public:
	double m_editLeftHand;
	double m_editRightHand;
	CString m_editLeftFingers;
	CString m_editRightFingers;
	CString m_editMean;


	afx_msg CString strOrder();
	afx_msg void OnBnClickedEnteringButton();
	afx_msg void OnBnClickedReturnButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
