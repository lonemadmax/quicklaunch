/*
 * Copyright 2010. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *	Humdinger, humdingerb@gmail.com
 *
 * A graphical launch panel finding an app via a query.
 */


#include "QuickLaunch.h"


extern const char *kApplicationSignature = "application/x-vnd.QuickLaunch";


QLApp::QLApp()
	:
	BApplication(kApplicationSignature)
{	
	fSettings = new QLSettings();

	BRect aFrame;
	aFrame.Set(0.0, 0.0, 340.0, 250.0);
	fSetupWindow = new SetupWindow(aFrame);
	aFrame.Set(0.0, 0.0, 340.0, 90.0);
	fMainWindow = new MainWindow(aFrame);
}


void
QLApp::ReadyToRun()
{
	fMainWindow->SetSizeLimits(190.0, 800.0, 90.0, 1000.0);
	BRect frame = fSettings->GetMainWindowFrame();
	fMainWindow->MoveTo(frame.LeftTop());
	fMainWindow->ResizeTo(frame.right - frame.left, 90.0);
	fMainWindow->Show();
	
	frame.OffsetBy(70.0, 120.0);
	fSetupWindow->MoveTo(frame.LeftTop());
	BRect bounds = fSettings->GetSetupWindowBounds();
	fSetupWindow->ResizeTo(bounds.Width(), bounds.Height());
	fSetupWindow->SetSizeLimits(340.0, 800.0, 160.0, 1000.0);
	
	fSettings->InitIgnoreList();
	
}


QLApp::~QLApp()
{
	delete fSettings;
}


void
QLApp::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case SETUP_BUTTON:
		{
			if (fSetupWindow->IsHidden()) {
				BRect frame = fMainWindow->Frame();
				frame.OffsetBy(70.0, 120.0);
				fSetupWindow->MoveTo(frame.LeftTop());
				fSetupWindow->Show();
			}
			else {
				fSetupWindow->Hide();
			}
			break;
		}		
		case VERSION_CHK:
		{
			int32 value;
			message->FindInt32("be:value", &value);
			fSettings->SetShowVersion(value);
			if (!fMainWindow->fListView->IsEmpty()) {
				fMainWindow->LockLooper();
				fMainWindow->fListView->Invalidate();
				fMainWindow->UnlockLooper();
			}
			break;
		}
		case PATH_CHK:
		{
			int32 value;
			message->FindInt32("be:value", &value);
			fSettings->SetShowPath(value);
			if (!fMainWindow->fListView->IsEmpty()) {
				fMainWindow->LockLooper();
				fMainWindow->fListView->Invalidate();
				fMainWindow->UnlockLooper();
			}
			break;
		}
		case IGNORE_CHK:
		{
			if (fSetupWindow->fIgnoreList->IsEmpty()) {
				fSetupWindow->LockLooper();
				fSetupWindow->fChkIgnore->SetValue(false);
				fSettings->SetShowIgnore(false);
				fSetupWindow->UnlockLooper();
				break;
			} 
			int32 value;
			message->FindInt32("be:value", &value);
			fSettings->SetShowIgnore(value);
			if (!fSetupWindow->fIgnoreList->IsEmpty()) {
 				fSetupWindow->fChkIgnore->SetValue(value);
				if (!fMainWindow->fListView->IsEmpty()) {
	 				fMainWindow->fListView->LockLooper();
	
	 				const char *searchString = fMainWindow->GetSearchString();
	
					fMainWindow->BuildList(searchString);
					fMainWindow->fListView->UnlockLooper();
				}
			}
			break;
		}
		case FILEPANEL:
		{
			if (!fSetupWindow->fIgnoreList->IsEmpty()) {
				fSetupWindow->LockLooper();
				fSetupWindow->fChkIgnore->SetValue(true);
				fSettings->SetShowIgnore(true);
				fSetupWindow->UnlockLooper();
			} else {
				fSetupWindow->LockLooper();
				fSetupWindow->fChkIgnore->SetValue(false);
				fSettings->SetShowIgnore(false);
				fSetupWindow->UnlockLooper();
			}
			if (fMainWindow->GetStringLength() > 0) {
				fMainWindow->fListView->LockLooper();
				const char *searchString = fMainWindow->GetSearchString();
				fMainWindow->BuildList(searchString);
				fMainWindow->fListView->UnlockLooper();
				fSettings->SetItemCount(fSetupWindow->fIgnoreList->CountItems());
			}
			break;
		}
		default:
		{
			BApplication::MessageReceived(message);
			break;
		}
	}
}


bool
QLApp::QuitRequested()
{
	return true;
}


void
QLApp::AboutRequested()
{
	BAlert *alert = new BAlert("about", "QuickLaunch   v0.9.5\n"
		"\twritten by Humdinger\n"
		"\tCopyright 2010\n\n"
		"QuickLaunch quickly starts any installed application. "
		"Just enter at least three letters of its name and choose "
		"from a list of all found programs.\n", "Thank you");
	BTextView *view = alert->TextView();
	BFont font;

	view->SetStylable(true);
	view->GetFont(&font);
	font.SetSize(font.Size()+4);
	font.SetFace(B_BOLD_FACE);
	view->SetFontAndColor(0, 11, &font);
	alert->Go();
}

		
int
main()
{
	QLApp app;
	app.Run();
	return 0;
}
