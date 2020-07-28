
// UIToPyConverterDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "UIToPyConverter.h"
#include "UIToPyConverterDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIToPyConverterDlg dialog



CUIToPyConverterDlg::CUIToPyConverterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UITOPYCONVERTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUIToPyConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUIToPyConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUIToPyConverterDlg::OnClickedConvert)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT1, &CUIToPyConverterDlg::OnChangeFromFilePath)
	ON_BN_CLICKED(IDC_BUTTON2, &CUIToPyConverterDlg::BrowseFromFile)
	ON_BN_CLICKED(IDC_BUTTON3, &CUIToPyConverterDlg::BrowseToFilePath)
END_MESSAGE_MAP()


// CUIToPyConverterDlg message handlers

BOOL CUIToPyConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	Init();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUIToPyConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUIToPyConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUIToPyConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CUIToPyConverterDlg* DlgPtr;

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#define FILE_NOT_FOUND "?Error?"
#define CHECK_FILE_MODIFIED 200


char UIFromFilePath[500] = "";//This is for performance improvement IsFileModified() function that gets called every 500 miliseconds

//This functions sees if the selected file is changed and if so, keeping the py file up to date
void IsFileModified()
{
	//Initializing
	static CTime m_prevTime;

	//If no file is selected, returning
	if (UIFromFilePath[0] == NULL)
		return;

	//Opening file
	CFile cfile;
	cfile.Open(UIFromFilePath, CFile::modeRead);
	
	//Getting File info
	CFileStatus status;
	if (cfile.GetStatus(status))
	{
		//If Last Modified time is changed, saving the file
		if (m_prevTime < status.m_mtime)
		{
			cfile.Close();
			DlgPtr->OnClickedConvert();
		}
		m_prevTime = status.m_mtime;
		return;
	}
	cfile.Close();
}


//Returns path of first file found with a ui extension in the same folder
std::string& GetFirstFileWithExt(std::string& DirectoryPath, std::string& Extension)
{
	static std::string FilePath = "";

	for (const auto& entry : fs::directory_iterator(DirectoryPath))
	{
		if (entry.path().extension() == Extension)
		{
			FilePath = entry.path().string();
			return FilePath;
		}
	}
	FilePath = FILE_NOT_FOUND;
	return FilePath;
}


//Removes Name from path
char* RemoveNameFromPath(const char* Path)
{
	static char PathWithoutName[500];

	int j = 0;
	for (int i = 0; Path[i] != NULL; i++)
	{
		if (Path[i] == '\\')
			j = i;
	}

	for (int i = 0; i < j; i++)
	{
		PathWithoutName[i] = Path[i];
	}

	return PathWithoutName;
}


//Change extension
void ChangeExtension(char* fname, char* extension)
{
	int j = 0;
	for (int i = 0; fname[i] != NULL; i++)
	{
		if (fname[i] == '.')
		{
			j = i;
		}
	}
	fname[j] = NULL;
	strcat(fname, ".");
	strcat(fname, extension);
}


//Gets called in OnInitDlg()
void CUIToPyConverterDlg::Init()
{
	//Setting Dlg Ptr = this
	DlgPtr = this;

	//Setting timer
	SetTimer(CHECK_FILE_MODIFIED, 500, NULL);

	//Getting Program dir Path
	char ProgramDirName[_MAX_PATH];
	GetModuleFileName(NULL, ProgramDirName, _MAX_PATH);
	strcpy(ProgramDirName, RemoveNameFromPath(ProgramDirName));

	//Getting file path of first UI File found
	std::string UIFilePath = GetFirstFileWithExt(std::string(ProgramDirName), std::string(".ui"));

	//If UI File found in the folder, Setting UI file path to EDIT1 and setting same path with .py ext to EDIT2
	if (UIFilePath != FILE_NOT_FOUND)
	{
		SetDlgItemText(IDC_EDIT1, UIFilePath.c_str());

		//Getting ToFilePath, which is the same as UI File Path just changed ext to .py
		char PyFilePath[500];
		strcpy(PyFilePath, UIFilePath.c_str());
		ChangeExtension(PyFilePath, "py");

		SetDlgItemText(IDC_EDIT2, PyFilePath);
	}
}


//Gets called on clicked button "Convert"
void CUIToPyConverterDlg::OnClickedConvert()
{
	//Initialization
	char FromFilePath[500], ToFilePath[500], Command[1000];
	GetDlgItemText(IDC_EDIT1, FromFilePath, 499);
	GetDlgItemText(IDC_EDIT2, ToFilePath, 499);

	//Trimming whitespaces before checking if any of the two arrays are empty, if so, returning
	if ((CString(FromFilePath).Trim()).IsEmpty() || (CString(ToFilePath).Trim()).IsEmpty())
		return;
	
	//Setting up Command to execute
	sprintf(Command, "pyuic5 -x %s -o %s", FromFilePath, ToFilePath);

	//Executing command
	system(Command);
}


//Timer
void CUIToPyConverterDlg::OnTimer(UINT_PTR nIDEvent)
{
	//Checking if selected UI file has changed
	if(nIDEvent == CHECK_FILE_MODIFIED)
		IsFileModified();

	CDialogEx::OnTimer(nIDEvent);
}


//Gets called on change in IDC_EDIT1
void CUIToPyConverterDlg::OnChangeFromFilePath()
{
	//On Change in edit box, saving UI FilePath for function called in a timer
	GetDlgItemText(IDC_EDIT1, UIFromFilePath, 499);
}


//This function displays a dlg for browsing and returns file path
CString& Browse()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);

	int val = (int)dlg.DoModal();
	if (val == IDCANCEL)
		return CString("?Error?");

	static CString Path;
	Path = dlg.GetPathName();
	return Path;
}

//Gets called while browsing UI File
void CUIToPyConverterDlg::BrowseFromFile()
{
	CString Path = Browse();
	SetDlgItemText(IDC_EDIT1, Path);
}


//Gets called while browsing Py File
void CUIToPyConverterDlg::BrowseToFilePath()
{
	CString Path = Browse();
	SetDlgItemText(IDC_EDIT2, Path);
}
