// ConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Oscilloscope.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"


// CConfigDlg �Ի���

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(double allTime, double step, int NO, double start, double end, CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
{
	this->allTime = allTime;
	this->step = step;
	this->NO = NO;
	this->start = start;
	this->end = end;
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg ��Ϣ�������


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString text;
	text.Format(_T("%d"), int(allTime * 1000));
	SetDlgItemText(IDC_EDIT6, text);
	text.Format(_T("%d"), int(step * 1000));
	SetDlgItemText(IDC_EDIT7, text);
	text.Format(_T("%d"), NO);
	SetDlgItemText(IDC_COMBO3, text);
	text.Format(_T("%d"), int(start * 1000));
	SetDlgItemText(IDC_EDIT8, text);
	text.Format(_T("%d"), int(end * 1000));
	SetDlgItemText(IDC_EDIT9, text);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CConfigDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString text;
	GetDlgItemText(IDC_EDIT6, text);
	this->allTime = _ttoi(text) / 1000.0;
	GetDlgItemText(IDC_EDIT7, text);
	this->step = _ttoi(text) / 1000.0;
	GetDlgItemText(IDC_COMBO3, text);
	this->NO = _ttoi(text);
	GetDlgItemText(IDC_EDIT8, text);
	this->start = _ttoi(text) / 1000.0;
	GetDlgItemText(IDC_EDIT9, text);
	this->end = _ttoi(text) / 1000.0;
	CDialogEx::OnOK();
}
