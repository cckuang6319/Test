// SMMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMMonitor.h"
#include "SMMonitorDlg.h"
#include "DlgInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMMonitorDlg dialog

CSMMonitorDlg::CSMMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMMonitorDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMMonitorDlg)
	DDX_Control(pDX, IDC_EDIT2, m_Edit_SMName);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Device);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSMMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CSMMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMMonitorDlg message handlers

BOOL CSMMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	this->gAutoRefresh = false;

    this->gSMIni = TEXT("./CIM.ini");

	memset(this->gSMName,'\0', sizeof(char));
	GetPrivateProfileString("SM", "SMName", "NO_VALUE", 
		this->gSMName, sizeof(this->gSMName), this->gSMIni);
	this->gComm_SMName = (LPTSTR)this->gSMName;
	this->gComm_SMsize = (unsigned long)GetPrivateProfileInt("SM", "SMSize", 0, this->gSMIni);

	this->m_Edit_SMName.SetWindowText(this->gComm_SMName);

	this->m_Edit_Device.SetWindowText("0");
	int j;
	for(int i=0; i<10; i++)
	{
		j = (i%2);
		if(0==j)
			this->m_List.InsertColumn(i,"Device", LVCFMT_LEFT, 60);
		else
			this->m_List.InsertColumn(i,"Value", LVCFMT_LEFT, 60);
	}

	CString s;
	this->m_Edit_Device.GetWindowText(s);
	int k = atoi(s);
	for(int i2=0; i2<20; i2++)
	{
		s.Format("D%d",k+i2);
		j = this->m_List.InsertItem(i2,s);
		this->m_List.SetItemText(j, 1, "null");
		s.Format("D%d",(k+i2)+20);
		this->m_List.SetItemText(j, 2, s);
		this->m_List.SetItemText(j, 3, "null");
		s.Format("D%d",(k+i2)+40);
		this->m_List.SetItemText(j, 4, s);
		this->m_List.SetItemText(j, 5, "null");
		s.Format("D%d",(k+i2)+60);
		this->m_List.SetItemText(j, 6, s);
		this->m_List.SetItemText(j, 7, "null");
		s.Format("D%d",(k+i2)+80);
		this->m_List.SetItemText(j, 8, s);
		this->m_List.SetItemText(j, 9, "null");
	}
	this->m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSMMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSMMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSMMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CSMMonitorDlg::chkAddress(unsigned int address)
{
	if( (unsigned int)this->gComm_SMsize > address )
		return 0;

	return 1;
}

void CSMMonitorDlg::OnOK() 
{
	// TODO: Add extra validation here
	hMapFile = OpenFileMapping(
					FILE_MAP_ALL_ACCESS,			
					FALSE,							
					this->gComm_SMName);			

	
	if (hMapFile == NULL)
	{
		this->MessageBox(TEXT("OpenFileMapping Fail"), TEXT("CSMDevice::Open"), MB_OK);
		return ;
	}
	pBuf = (unsigned short*) MapViewOfFile(hMapFile,	// handle to map object
					FILE_MAP_ALL_ACCESS,				// read/write permission
					0,
					0,
					2*this->gComm_SMsize);

	if (pBuf == NULL)
	{
		this->MessageBox(TEXT("MapViewOfFile Fail"), TEXT("CSMDevice::Open"), MB_OK);
		CloseHandle(hMapFile);
		return ;
	}
}

void CSMMonitorDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	this->gAutoRefresh = !this->gAutoRefresh;
	CButton *select;
	select = (CButton *)GetDlgItem(IDC_BUTTON1);
	if(this->gAutoRefresh == true)
	{
		SetTimer(1,500,NULL);
	}
	else
	{
		KillTimer(1);
	}

}

void CSMMonitorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);

	CString s;
	CString value;
	this->m_Edit_Device.GetWindowText(s);
	int k = atoi(s);
	unsigned short *v;
	CButton *select;
	select = (CButton *)GetDlgItem(IDC_RADIO1);
	select->SetCheck(!select->GetCheck());

	for(int i2=0; i2<20; i2++)
	{
		s.Format("D%d",k+i2);
		v = (unsigned short *)(this->pBuf + (k+i2) );
		if(this->chkAddress((k+i2)) != 0)
			value.Format("Invalid");
		else
			value.Format("%.4X", *v);
		this->m_List.SetItemText(i2, 0, s);
		this->m_List.SetItemText(i2, 1, value);

		s.Format("D%d",(k+i2)+20);
		v = (unsigned short *)(this->pBuf + (k+i2+20) );
		if(this->chkAddress((k+i2+20)) != 0)
			value.Format("Invalid");
		else
			value.Format("%.4X", *v);
		this->m_List.SetItemText(i2, 2, s);
		this->m_List.SetItemText(i2, 3, value);

		s.Format("D%d",(k+i2)+40);
		v = (unsigned short *)(this->pBuf + (k+i2+40) );
		if(this->chkAddress((k+i2+40)) != 0)
			value.Format("Invalid");
		else
			value.Format("%.4X", *v);
		this->m_List.SetItemText(i2, 4, s);
		this->m_List.SetItemText(i2, 5, value);

		s.Format("D%d",(k+i2)+60);
		v = (unsigned short *)(this->pBuf + (k+i2+60) );
		if(this->chkAddress((k+i2+60)) != 0)
			value.Format("Invalid");
		else
			value.Format("%.4X", *v);
		this->m_List.SetItemText(i2, 6, s);
		this->m_List.SetItemText(i2, 7, value);

		s.Format("D%d",(k+i2)+80);
		v = (unsigned short *)(this->pBuf + (k+i2+80) );
		if(this->chkAddress((k+i2+80)) != 0)
			value.Format("Invalid");
		else
			value.Format("%.4X", *v);
		this->m_List.SetItemText(i2, 8, s);
		this->m_List.SetItemText(i2, 9, value);
	}
}

void CSMMonitorDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CSMMonitorDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CSMMonitorDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	UNREFERENCED_PARAMETER(pResult);
	LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pNMHDR;

	CPoint   pos; 
	CRect   rcCtrl; 
	GetCursorPos(&pos);       
	ScreenToClient(&pos);
	LVHITTESTINFO   lvhti; 
	lvhti.pt   =   pia->ptAction;//pos; 
	this->m_List.SubItemHitTest(&lvhti); 	 
	this->m_List.GetSubItemRect(lvhti.iItem,lvhti.iSubItem,LVIR_LABEL,rcCtrl); 
	CString   strItem   =   this->m_List.GetItemText(lvhti.iItem,lvhti.iSubItem); 

	CDlgInput input;
	strItem.TrimLeft("D");
	input.device = strItem;
	input.pBuf = this->pBuf;
	input.DoModal();

	*pResult = 0;

}
