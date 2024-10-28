/*
 * Copyright 2010-2019. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *	Humdinger, humdingerb@gmail.com
 */

#ifndef QL_WINDOW_H
#define QL_WINDOW_H

#include "AppListItem.h"
#include "MainListItem.h"
#include "MainListView.h"

#include <Alert.h>
#include <Application.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <ListView.h>
#include <Message.h>
#include <ObjectList.h>
#include <Path.h>
#include <Query.h>
#include <Roster.h>
#include <Screen.h>
#include <ScrollView.h>
#include <TextControl.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <Window.h>

#include <stdlib.h>
#include <strings.h>


#define SINGLE_CLICK	'1clk'
#define SETUP_BUTTON	'setb'
#define HELP_BUTTON		'help'

#define kMAX_DISPLAYED_ITEMS	10


class MainWindow : public BWindow {
public:
					MainWindow();
	virtual			~MainWindow();

	virtual	void	BuildUI();
	virtual	void	MoveContentTo(BPoint leftTop);
	virtual	void	ResizeContentTo(float width, float height);
	BRect			ContentFrame()
						{return fContentRect;};

	void			MessageReceived(BMessage* message);
	bool			QuitRequested();

	void			BuildAppList();
	void			FilterAppList();
	float			GetScrollPosition();
	void			SetScrollPosition(float position);
	int				GetStringLength()
						{return fSearchBox->TextView()->TextLength();};
	const char*		GetSearchString()
						{return fSearchBox->TextView()->Text();};
	void			ResizeWindow();

	MainListView*	fListView;

private:
	static status_t _AppListThread(void* self);
	void			_BuildAppList();

	void			_LaunchApp(MainListItem* item);
	void			_AddDroppedAsFav(BMessage* message);
	void			_ShowFavorites();

protected:
	BTextControl*	fSearchBox;
	BScrollView*	fScrollView;
	BRect			fContentRect;

private:
	thread_id		fThreadId;
	bool			fBusy;

	BObjectList<AppListItem> fAppList;
	int32			fIconHeight;
};


class FocusWindow : public MainWindow {
public:
					FocusWindow();
	virtual			~FocusWindow();

	virtual	void	BuildUI();
	virtual	void	MoveContentTo(BPoint leftTop);
	virtual	void	ResizeContentTo(float width, float height);

private:
	BView*			fContainer;

};

#endif // QL_WINDOW_H
