#pragma once


// CConfigDlg �Ի���

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(double allTime, double step, int NO, double start, double end, CWnd* pParent = NULL);  
	virtual ~CConfigDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

public:
	double allTime;
	double step;
	int NO;
	double start;
	double end;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
