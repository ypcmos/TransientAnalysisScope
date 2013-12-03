
// OscilloscopeDlg.h : ͷ�ļ�
//

#pragma once
#include "Scope.h"
#include "AnalysisModule.h"

// COscilloscopeDlg �Ի���
class COscilloscopeDlg : public CDialogEx
{
// ����
public:
	COscilloscopeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~COscilloscopeDlg();

// �Ի�������
	enum { IDD = IDD_OSCILLOSCOPE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
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
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
