#pragma once


// CConfigDlg 对话框

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(double allTime, double step, int NO, double start, double end, CWnd* pParent = NULL);  
	virtual ~CConfigDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

public:
	double allTime;
	double step;
	int NO;
	double start;
	double end;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
