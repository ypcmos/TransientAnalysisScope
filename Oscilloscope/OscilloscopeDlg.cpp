
// OscilloscopeDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// COscilloscopeDlg 对话框




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


// COscilloscopeDlg 消息处理程序

BOOL COscilloscopeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	sp = new CScope(GetDlgItem(IDC_PIC1));
	SetTimer(1, 100, NULL);
	x = -2;
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COscilloscopeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COscilloscopeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COscilloscopeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	GetDlgItemText(IDC_COMBO1, text);
	if (text.Find(_T("一号发电机功角")) != -1)
	{
		which = 0;
	} else if (text.Find(_T("二号发电机功角")) != -1)
	{
		which = 1;
	}else if (text.Find(_T("三号发电机功角")) != -1)
	{
		which = 2;
	}else if (text.Find(_T("四号发电机功角")) != -1)
	{
		which = 3;
	} else if (text.Find(_T("一二")) != -1)
	{
		which = 4;
	}else if (text.Find(_T("二三")) != -1)
	{
		which = 5;
	}else if (text.Find(_T("三四")) != -1)
	{
		which = 6;
	}else if (text.Find(_T("一号发电机电压")) != -1)
	{
		which = 7;
	}else if (text.Find(_T("二号发电机电压")) != -1)
	{
		which = 8;
	}else if (text.Find(_T("三号发电机电压")) != -1)
	{
		which = 9;
	}else if (text.Find(_T("四号发电机电压")) != -1)
	{
		which = 10;
	}else {
		AfxMessageBox(_T("请选择需要监测的发电机"));
		return;
	}
	sp->clear();
	x = 0;
	
	am = new AnalysisModule(step, 0);
	if (!am->readDataFile("4G2A.dat"))
	{
		AfxMessageBox(_T("不存在数据文件【O(∩_∩)O请确保文件4G2A.dat位于当前路径下】"));
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	//KillTimer(0);
	TerminateThread(threadSim->m_hThread, -1);
	am->endSimulate(sn);
	GetDlgItem(IDOK)->EnableWindow();
	GetDlgItem(IDC_BUTTON1)->EnableWindow();
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
}
