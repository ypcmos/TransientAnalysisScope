
// OscilloscopeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Oscilloscope.h"
#include "OscilloscopeDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include "ConfigDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COscilloscopeDlg �Ի���




COscilloscopeDlg::COscilloscopeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COscilloscopeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	allTime = 5;
	step = 0.01;
	NO = 0;
	start = 2.0;
	end = 2.1;
}

void COscilloscopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COscilloscopeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &COscilloscopeDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &COscilloscopeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COscilloscopeDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// COscilloscopeDlg ��Ϣ�������

BOOL COscilloscopeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	sp = new CScope(GetDlgItem(IDC_PIC1));
	SetTimer(1, 100, NULL);
	x = -2;
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COscilloscopeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COscilloscopeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COscilloscopeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COscilloscopeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString text;
	GetDlgItemText(IDC_COMBO1, text);
	if (text.Find(_T("һ�ŷ��������")) != -1)
	{
		which = 0;
	} else if (text.Find(_T("���ŷ��������")) != -1)
	{
		which = 1;
	}else if (text.Find(_T("���ŷ��������")) != -1)
	{
		which = 2;
	}else if (text.Find(_T("�ĺŷ��������")) != -1)
	{
		which = 3;
	} else if (text.Find(_T("һ��")) != -1)
	{
		which = 4;
	}else if (text.Find(_T("����")) != -1)
	{
		which = 5;
	}else if (text.Find(_T("����")) != -1)
	{
		which = 6;
	}else if (text.Find(_T("һ�ŷ������ѹ")) != -1)
	{
		which = 7;
	}else if (text.Find(_T("���ŷ������ѹ")) != -1)
	{
		which = 8;
	}else if (text.Find(_T("���ŷ������ѹ")) != -1)
	{
		which = 9;
	}else if (text.Find(_T("�ĺŷ������ѹ")) != -1)
	{
		which = 10;
	}else {
		AfxMessageBox(_T("��ѡ����Ҫ���ķ����"));
		return;
	}
	sp->clear();
	x = 0;
	
	am = new AnalysisModule(step, 0);
	if (!am->readDataFile("4G2A.dat"))
	{
		AfxMessageBox(_T("�����������ļ���O(��_��)O��ȷ���ļ�4G2A.datλ�ڵ�ǰ·���¡�"));
		return;
	}
	
	sn = am->readySimulate();
	num = am->getGenNum();
	//SetTimer(0, int(step * 1000), NULL);
	threadSim = AfxBeginThread(simulate, this);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow();
}

COscilloscopeDlg::~COscilloscopeDlg()
{
	delete sp;
}


void COscilloscopeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*double y = sin(x);
	sp->drawCurrent(50 * x, y);*/
	double y;
	switch (nIDEvent)
	{
	case 0:
	{
		double *del, *v;
		del = new double[num];
		v = new double[num];
		am->simulateStep(sn, del, v);
		if (which <= 3)
		{
			y = del[which] / 3.14 * 180;
		} else if (which > 6)
		{
			y = v[which - 7] ;
		} else {
			y = (del[which - 4] - del[ which - 3]) / 3.14 * 180;
		}
		sp->drawCurrent(x, y);
		
		if (NO != 0)
		{
			if (abs(x - start) < 1e-5)
			{
				am->busShortCircuit(sn, NO - 1);
				am->simulateStep(sn, del, v);
				if (which <= 3)
				{
					y = del[which] / 3.14 * 180;
				} else if (which > 6)
				{
					y = v[which - 7] ;
				} else {
					y = (del[which - 4] - del[ which - 3]) / 3.14 * 180;
				}
				sp->drawCurrent(x, y);
			}

			if (abs(x - end) < 1e-5)
			{
				sn->calcY();
				am->simulateStep(sn, del, v);
				if (which <= 3)
				{
					y = del[which] / 3.14 * 180;
				} else if (which > 6)
				{
					y = v[which - 7] ;
				} else {
					y = (del[which - 4] - del[ which - 3]) / 3.14 * 180;
				}
				sp->drawCurrent(x, y);
			}
		}
		x += step;
		if (x > allTime)
		{
			KillTimer(0);
			am->endSimulate(sn);
			GetDlgItem(IDOK)->EnableWindow();
			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON1)->EnableWindow();
		}
		
		break;

	}
	case 1:
	{
		y = sqrt(1 - pow(abs(x) - 1, 2.0));
		sp->drawCurrent(x, y);
		x += 0.1;
		if (x > 2.0)
		{
			y = 0;
			sp->drawCurrent(x, y);
			KillTimer(1);
			SetTimer(2, 100, NULL);
			x = 2;
		}
		break;
	}
	case 2:
	{
		y = acos(1 - abs(x)) - 3.14;
		sp->drawCurrent(x, y);
		x -= 0.1;
		if (x < -2.0)
		{
			y = 0;
			sp->drawCurrent(x, y);
			KillTimer(2);
			GetDlgItem(IDOK)->EnableWindow();
			GetDlgItem(IDC_BUTTON1)->EnableWindow();
		}
		break;
	}

	}
	showMessage(x, y);
	/*delete [] del;
	delete [] v;*/
	CDialogEx::OnTimer(nIDEvent);
}

void COscilloscopeDlg::showMessage(double x, double y)
{
	double xmin, xmax, ymin, ymax;
	xmin = sp->getXmin();
	xmax = sp->getXmax();
	ymin = sp->getYmin();
	ymax = sp->getYmax();
	CString text;
	text.Format(_T("%f"), xmin);
	SetDlgItemText(IDC_EDIT1, text);
	text.Format(_T("%f"), xmax);
	SetDlgItemText(IDC_EDIT2, text);
	text.Format(_T("%f"), ymin);
	SetDlgItemText(IDC_EDIT3, text);
	text.Format(_T("%f"), ymax);
	SetDlgItemText(IDC_EDIT4, text);
	text.Format(_T("%f"), x);
	SetDlgItemText(IDC_EDITX, text);
	text.Format(_T("%f"), y);
	SetDlgItemText(IDC_EDIT5, text);
}

UINT simulate(LPVOID param)
{
	COscilloscopeDlg *dlg = (COscilloscopeDlg *)param;
	int num = dlg->num;
	AnalysisModule * am = dlg->am;
	SimplePowerNet * sn = dlg->sn;
	int which = dlg->which;
	CScope * sp = dlg->sp;
	double y;
	double *del, *v;
	int NO = dlg->NO;
	double start = dlg->start, end = dlg->end, step = dlg->step, allTime = dlg->allTime, x = dlg->x;
	del = new double[num];
	v = new double[num];
	while (1)
	{
		am->simulateStep(sn, del, v);
		if (which <= 3)
		{
			y = del[which] / 3.14 * 180;
		} else if (which > 6)
		{
			y = v[which - 7] ;
		} else {
			y = (del[which - 4] - del[ which - 3]) / 3.14 * 180;
		}
		sp->drawCurrent(x, y);

		if (NO != 0)
		{
			if (abs(x - start) < 1e-5)
			{
				am->busShortCircuit(sn, NO - 1);
				am->simulateStep(sn, del, v);
				if (which <= 3)
				{
					y = del[which] / 3.14 * 180;
				} else if (which > 6)
				{
					y = v[which - 7] ;
				} else {
					y = (del[which - 4] - del[ which - 3]) / 3.14 * 180;
				}
				sp->drawCurrent(x, y);
			}

			if (abs(x - end) < 1e-5)
			{
				sn->calcY();
				am->simulateStep(sn, del, v);
				if (which <= 3)
				{
					y = del[which] / 3.14 * 180;
				} else if (which > 6)
				{
					y = v[which - 7] ;
				} else {
					y = (del[which - 4] - del[ which - 3]) / 3.14 * 180;
				}
				sp->drawCurrent(x, y);
			}
		}
		dlg->showMessage(x, y);
		x += step;
		if (x > allTime)
		{
			am->endSimulate(sn);
			dlg->GetDlgItem(IDOK)->EnableWindow();
			dlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow();
			delete [] del;
			delete [] v;
			return 0;
		}
	}
	delete [] del;
	delete [] v;
	return -1;
}

void COscilloscopeDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CConfigDlg dlg(allTime, step, NO, start, end);

	if (dlg.DoModal() == IDOK)
	{
		allTime = dlg.allTime;
		step = dlg.step;
		NO = dlg.NO;
		start = dlg.start;
		end = dlg.end;
	}
}


void COscilloscopeDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//KillTimer(0);
	TerminateThread(threadSim->m_hThread, -1);
	am->endSimulate(sn);
	GetDlgItem(IDOK)->EnableWindow();
	GetDlgItem(IDC_BUTTON1)->EnableWindow();
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
}
