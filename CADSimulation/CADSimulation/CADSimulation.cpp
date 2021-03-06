
// CADSimulation.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CADSimulation.h"
#include "MainFrm.h"

#include "CADSimulationDoc.h"
#include "CADSimulationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCADSimulationApp

BEGIN_MESSAGE_MAP(CCADSimulationApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCADSimulationApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCADSimulationApp construction

CCADSimulationApp::CCADSimulationApp()
  : m_strLastShapeName(_T(""))
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CADSimulation.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
  m_nPenWidth = 1;
  m_nPenStyle = PS_SOLID;
  m_nPenColor = RGB(0, 0, 0);

  m_nBrushColor = RGB(255, 255, 255);

  memset(m_aryCustomizedShapePoints, 0, sizeof(m_aryCustomizedShapePoints));
  m_nCustomiezedShapePointCount = 0;

  m_pView = NULL;

}

// The one and only CCADSimulationApp object

CCADSimulationApp theApp;


// CCADSimulationApp initialization

#pragma comment(lib, "SkinPPWTL.lib")

BOOL CCADSimulationApp::InitInstance()
{
  skinppLoadSkin(_T("Devoir.ssk"));//blue.ssk为项目下的皮肤文件

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCADSimulationDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCADSimulationView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CCADSimulationApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CCADSimulationApp message handlers


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

// App command to run the dialog
void CCADSimulationApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCADSimulationApp message handlers

CString * CCADSimulationApp::GetLastShapeName()
{
  return &m_strLastShapeName;
}


int CCADSimulationApp::SetLastShapeName(CWnd * pShapeButton)
{
  pShapeButton->GetWindowText(m_strLastShapeName);

  return 0;
}


int CCADSimulationApp::SaveCustomizedShapeToFile(CString &strFileName)
{
  //   ::CreateFile(strFileName,//文件名
  //     GENERIC_READ | GENERIC_WRITE,
  //     FILE_SHARE_READ | FILE_SHARE_WRITE,
  //     NULL,
  //     CREATE_ALWAYS,
  //     FILE_ATTRIBUTE_NORMAL,
  //     NULL);
  // 
  //   
  // 
  //   std::ofstream save;
  // 
  //   save.open(strFileName, std::ios::out|| std::ios::binary);
  // 
  //   for (int i = 0; i < CUSTOMIZE_SHAPE_MAX_POINTS; i++)
  //   {
  //     save << theApp.m_aryCustomizedShapePoints[i].x;
  //     save << theApp.m_aryCustomizedShapePoints[i].y;
  //   }
  // 
  //   save << theApp.m_nCustomiezedShapePointCount;
  FILE *pFile = fopen((const char *)strFileName.GetBuffer(0), "w");

  fprintf(pFile, "%d\r\n", theApp.m_nCustomiezedShapePointCount);

  for (int i = 0; i < CUSTOMIZE_SHAPE_MAX_POINTS; i++)
  {
    fseek(pFile, 0, SEEK_END);
    fprintf(pFile, "%d %d\r\n", theApp.m_aryCustomizedShapePoints[i].x,
      theApp.m_aryCustomizedShapePoints[i].y);
  }

  fflush(pFile);

  fclose(pFile);

  return 0;
}

int CCADSimulationApp::LoadCustomizedShapeFromFile(CString &strFileName)
{
  FILE *pFile = fopen((const char *)strFileName.GetBuffer(0), "r");

  fscanf(pFile, "%d\r\n", &theApp.m_nCustomiezedShapePointCount);

  for (int i = 0; i < CUSTOMIZE_SHAPE_MAX_POINTS; i++)
  {
    fscanf(pFile, "%d %d\r\n", &(theApp.m_aryCustomizedShapePoints[i].x),
      &(theApp.m_aryCustomizedShapePoints[i].y));
  }

  fclose(pFile);

  return 0;
}