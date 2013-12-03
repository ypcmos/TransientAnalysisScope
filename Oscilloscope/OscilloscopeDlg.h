
// OscilloscopeDlg.h : 头文件
//

#pragma once
#include "Scope.h"
#include "AnalysisModule.h"

// COscilloscopeDlg 对话框
class COscilloscopeDlg : public CDialogEx
{
// 构造
public:
	COscilloscopeDlg(CWnd* pParent = NULL);	// 标准构造函数
	~COscilloscopeDlg();

// 对话框数据
	enum { IDD = IDD_OSCILLOSCOPE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	friend UINT simulate(LPVOID param);
	void showMessage(double x, double y);
private:
	CScope *sp;
	double x;
	AnalysisModule *am;
	SimplePowerNet *sn;
	int num;
	int which;
	double allTime;
	double step;
	int NO;
	double start;
	double end;
	CWinThread * threadSim;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

};
