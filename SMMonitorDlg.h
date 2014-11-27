// SMMonitorDlg.h : header file
//

#if !defined(AFX_SMMONITORDLG_H__BD326A9A_C7EA_499F_AD5D_3DAD0F20B206__INCLUDED_)
#define AFX_SMMONITORDLG_H__BD326A9A_C7EA_499F_AD5D_3DAD0F20B206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSMMonitorDlg dialog

class CSMMonitorDlg : public CDialog
{
// Construction
public:
	CSMMonitorDlg(CWnd* pParent = NULL);	// standard constructor

		int chkAddress(unsigned int address);
		bool gAutoRefresh;
		//INI File Paramater
		char gSMName[100];
		LPTSTR gSMIni;
		LPTSTR gComm_SMName;
		unsigned long gComm_SMsize;
		unsigned int gAlarm_Address;
        unsigned int gAlarm_Length;
		unsigned int gWarring_Address;
        unsigned int gWarring_Length;
		HANDLE hMapFile;
		unsigned short *pBuf;	//for Device Address Design 16 bit/one Device.

// Dialog Data
	//{{AFX_DATA(CSMMonitorDlg)
	enum { IDD = IDD_SMMONITOR_DIALOG };
	CEdit	m_Edit_SMName;
	CEdit	m_Edit_Device;
	CListCtrl	m_List;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSMMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCancelMode();
	virtual void OnCancel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMMONITORDLG_H__BD326A9A_C7EA_499F_AD5D_3DAD0F20B206__INCLUDED_)
