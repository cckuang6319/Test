// DlgInput.cpp : implementation file
//

#include "stdafx.h"
#include "SMMonitor.h"
#include "DlgInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInput dialog


CDlgInput::CDlgInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInput)
	DDX_Control(pDX, IDC_EDIT3, m_Value);
	DDX_Control(pDX, IDC_EDIT2, m_Spec);
	DDX_Control(pDX, IDC_EDIT1, m_Device);
	//}}AFX_DATA_MAP
	this->m_Device.SetWindowText(this->device);
	CButton *select;
	select = (CButton *)GetDlgItem(IDC_RADIO1);
	select->SetCheck(BST_CHECKED);
}


BEGIN_MESSAGE_MAP(CDlgInput, CDialog)
	//{{AFX_MSG_MAP(CDlgInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInput message handlers

void CDlgInput::OnOK() 
{
	// TODO: Add extra validation here
	try
	{
		CButton *select;
		CString address, value, spec;
		int iAddress;
		int iValue; 
		int iSpec;
		double dValue;
		this->m_Device.GetWindowText(address);
		this->m_Value.GetWindowText(value);
		this->m_Spec.GetWindowText(spec);
		iAddress = atoi(address);

		select = (CButton *)GetDlgItem(IDC_RADIO1);
		if( select->GetCheck() )
		{
			iValue = atoi(value);
			memcpy((void *)(this->pBuf+iAddress), &iValue, sizeof(short));
		}
		select = (CButton *)GetDlgItem(IDC_RADIO2);
		if( select->GetCheck() )
		{
			iValue = atoi(value);
			memcpy((void *)(this->pBuf+iAddress), &iValue, sizeof(int));
		}
		select = (CButton *)GetDlgItem(IDC_RADIO3);
		if( select->GetCheck() )
		{
			dValue = atof(value);
			memcpy((void *)(this->pBuf+iAddress), &iValue, sizeof(double));
		}
		select = (CButton *)GetDlgItem(IDC_RADIO4);
		if( select->GetCheck() )
		{
			this->m_Spec.GetWindowText(spec);
			iSpec = atoi(spec);
			memset((void *)(this->pBuf+iAddress), '\0', iSpec);
			memcpy((void *)(this->pBuf+iAddress), value.GetBuffer(value.GetLength()), _tcslen(value.GetBuffer(value.GetLength())) * sizeof(TCHAR)>iSpec?iSpec: _tcslen(value.GetBuffer(value.GetLength())) * sizeof(TCHAR) );
		}
	}
	catch(...)
	{
		this->MessageBox("!","Err");
	}	
	CDialog::OnOK();
}
