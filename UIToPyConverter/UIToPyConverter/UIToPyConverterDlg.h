
// UIToPyConverterDlg.h : header file
//

#pragma once


// CUIToPyConverterDlg dialog
class CUIToPyConverterDlg : public CDialogEx
{
// Construction
public:
	CUIToPyConverterDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UITOPYCONVERTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void Init();
	void OnClickedConvert();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeFromFilePath();
	afx_msg void BrowseFromFile();
	afx_msg void BrowseToFilePath();
};
