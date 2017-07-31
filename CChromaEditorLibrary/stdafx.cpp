// stdafx.cpp : source file that includes just the standard includes
// CChromaEditorLibrary.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "CChromaEditorLibrary.h"
#include "ChromaThread.h"
#include <thread>

using namespace ChromaSDK;
using namespace std;

ChromaThread _gChromaThread;
bool _gDialogIsOpen = false;
string _gPath = "";

void SetupChromaThread()
{
	_gChromaThread.Start();
}

void ThreadOpenEditorDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here

	//fprintf(stdout, "CChromaEditorLibrary::ThreadOpenEditorDialog %s\r\n", _gPath.c_str());

	// dialog instance
	CMainViewDlg mainViewDlg;

	mainViewDlg.OpenOrCreateAnimation(_gPath);

	// keep dialog focused
	mainViewDlg.DoModal();

	// dialog is closed
	_gDialogIsOpen = false;
}

extern "C"
{
	EXPORT_API double PluginIsDialogOpen()
	{
		// Chroma thread plays animations
		SetupChromaThread();

		if (_gDialogIsOpen)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	EXPORT_API double PluginOpenEditorDialog(char* path)
	{
		// Chroma thread plays animations
		SetupChromaThread();

		//fprintf(stdout, "CChromaEditorLibrary::PluginOpenEditorDialog %s\r\n", path);

		if (_gDialogIsOpen)
		{
			return -1;
		}

		_gDialogIsOpen = true;
		_gPath = path;
		thread newThread(ThreadOpenEditorDialog);
		newThread.detach();

		return 0;
	}
}
