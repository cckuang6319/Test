#if !defined(AFX_DLGINPUT_H__A63F9B9E_EA16_48D1_B646_F77318D8D460__INCLUDED_)
#define AFX_DLGINPUT_H__A63F9B9E_EA16_48D1_B646_F77318D8D460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInput dialog

class CDlgInput : public CDialog
{
// Construction
public:
	CDlgInput(CWnd* pParent = NULL);   // standard constructor
	CString device;
	unsigned short *pBuf;

// Dialog Data
	//{{AFX_DATA(CDlgInput)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_Value;
	CEdit	m_Spec;
	CEdit	m_Device;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInput)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUT_H__A63F9B9E_EA16_48D1_B646_F77318D8D460__INCLUDED_)
