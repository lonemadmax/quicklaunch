/*
 * Copyright 2010-2017. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *	Humdinger, humdingerb@gmail.com
 */

#include "QuickLaunch.h"
#include "SetupListView.h"
#include "SetupWindow.h"

#include <Catalog.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SetupWindow"

SetupListView::SetupListView()
	:
	BListView("IgnoreList", B_MULTIPLE_SELECTION_LIST),
	fShowingPopUpMenu(false)
{
}


SetupListView::~SetupListView()
{
}


#pragma mark -- BListView Overrides --


void
SetupListView::Draw(BRect rect)
{
	SetHighColor(ui_color(B_LIST_BACKGROUND_COLOR));
	BRect bounds(Bounds());
	BRect itemFrame = ItemFrame(CountItems() - 1);
	bounds.top = itemFrame.bottom;
	FillRect(bounds);

	BListView::Draw(rect);
}


void
SetupListView::FrameResized(float w, float h)
{
	BListView::FrameResized(w, h);

	for (int32 i = 0; i < CountItems(); i++) {
		BListItem* item = ItemAt(i);
		item->Update(this, be_plain_font);
	}
	Invalidate();
}


void
SetupListView::KeyDown(const char* bytes, int32 numBytes)
{
	switch (bytes[0]) {
		case B_DELETE:
		{
			if (!IsEmpty()) {
				Looper()->PostMessage(REM_BUT, this);
			}
			break;
		}
		default:
		{
			BListView::KeyDown(bytes, numBytes);
			break;
		}
	}
}


void
SetupListView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case POPCLOSE:
		{
			fShowingPopUpMenu = false;
			break;
		}
		default:
		{
			BListView::MessageReceived(message);
			break;
		}
	}
}


void
SetupListView::MouseDown(BPoint position)
{
	if (!IsEmpty()) {
		bool onSelection = false;
		BListItem* item = ItemAt(IndexOf(position));

		if (item != NULL && item->IsSelected())
			onSelection = true;

		uint32 buttons = 0;
		if (Window() != NULL && Window()->CurrentMessage() != NULL)
			buttons = Window()->CurrentMessage()->FindInt32("buttons");

		if ((buttons & B_SECONDARY_MOUSE_BUTTON) != 0) {

			if (CurrentSelection() < 0 || !onSelection)
				Select(IndexOf(position));

			if (CurrentSelection() >= 0)
				_ShowPopUpMenu(ConvertToScreen(position));
			return;
		}
	}

	BListView::MouseDown(position);
}


void
SetupListView::SelectionChanged()
{
	SetupWindow* window = dynamic_cast<SetupWindow *> (Window());
	if (CurrentSelection() < 0)
		window->fButRem->SetEnabled(false);
	else
		window->fButRem->SetEnabled(true);
}


#pragma mark -- Public Methods --


void
SetupListView::_ShowPopUpMenu(BPoint screen)
{
	if (fShowingPopUpMenu)
		return;

	ContextPopUp* menu = new ContextPopUp("PopUpMenu", this);
	BMessage* msg = NULL;

	msg = new BMessage(REM_BUT);
	BMenuItem* item = new BMenuItem(B_TRANSLATE("Remove"), msg);
	menu->AddItem(item);

	menu->SetTargetForItems(Looper());
	menu->Go(screen, true, true, true);
	fShowingPopUpMenu = true;
}


// #pragma mark - Context menu


ContextPopUp::ContextPopUp(const char* name, BMessenger target)
	:
	BPopUpMenu(name, false, false),
	fTarget(target)
{
	SetAsyncAutoDestruct(true);
}


ContextPopUp::~ContextPopUp()
{
	fTarget.SendMessage(POPCLOSE);
}
