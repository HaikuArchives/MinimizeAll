/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "DeskbarView.h"

#include <AppFileInfo.h>
#include <Application.h>
#include <Bitmap.h>
#include <Deskbar.h>
#include <Entry.h>
#include <File.h>
#include <MenuItem.h>
#include <Message.h>
#include <PopUpMenu.h>
#include <Resources.h>
#include <Roster.h>
#include <stdio.h>
#include <Window.h>

#include "../minimize.h"

extern "C" _EXPORT BView *instantiate_deskbar_item();

BView *instantiate_deskbar_item()
{
	return new DeskbarView();
}

DeskbarView::DeskbarView()
	: BView( VIEW_RECT, VIEW_NAME, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW)
{
	Init();
}

DeskbarView::DeskbarView(BMessage *archive)
	: BView(archive)
{
	Init();
}

DeskbarView::~DeskbarView() {

	delete fIcon;
	delete fMinimizedIcon;
	delete fPopUpMenu;
	
}

void DeskbarView::Init() {

	// Icon
	entry_ref ref;
	be_roster->FindApp( APP_SIGNATURE, &ref);

	BFile			file(&ref, B_READ_ONLY);
	BAppFileInfo	appFileInfo(&file);
	
	fIcon = new BBitmap(BRect(0,0,15,15), B_CMAP8 );
	appFileInfo.GetIcon(fIcon, B_MINI_ICON);

	// Icon minimized
	if (be_roster->FindApp(APP_SIGNATURE, &ref)!=B_OK) return;
	
	file.SetTo( &ref, B_READ_ONLY );
	BResources	resourcen( &file );

	size_t		groesse;
	BMessage	msg;
	char		*buf = (char *)resourcen.LoadResource('BBMP', "minimized", &groesse);
	fMinimizedIcon = 0;
	if (buf) {
		msg.Unflatten(buf);
		fMinimizedIcon = new BBitmap( &msg );
	}
	else {
		printf("*** Resource not found\n");
	}

	fPopUpMenu = NULL;
	fMouseDown = false;
}

void DeskbarView::AttachedToWindow(void) 
{
	SetViewColor(Parent()->ViewColor());
	SetDrawingMode( B_OP_ALPHA );

	BView::AttachedToWindow();
	
}

void DeskbarView::Draw(BRect rect)
{
	DrawBitmap(fIcon, BPoint(0.0, 0.0));
}

void DeskbarView::MouseDown(BPoint where) {

	int32 buttons = Window()->CurrentMessage()->FindInt32("buttons");

	switch (buttons) {

		case B_PRIMARY_MOUSE_BUTTON:

			fMouseDown = true;
			SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS|B_SUSPEND_VIEW_FOCUS);
			DrawBitmap( fMinimizedIcon, BPoint(0.0, 0.0) );
			break;
			
		case B_SECONDARY_MOUSE_BUTTON:

			ConvertToScreen(&where);
			BRect r(-5, -5, 5, 5);
			r = ConvertToScreen(r);
			r.OffsetTo(where);
		
			if (!fPopUpMenu) {
		
				fPopUpMenu = new BPopUpMenu("", false, false);
				fPopUpMenu->SetFontSize( 10.0 );
				
				BMessenger messenger(this);
				
				BMenuItem	*item;
			
				item = new BMenuItem("Remove me!", new BMessage('Remv'));
				item->SetTarget( messenger );
				fPopUpMenu->AddItem( item );
	
			}
			fPopUpMenu->Go(where, true, true, r, true);
			
			break;
	}

}

void DeskbarView::MouseUp(BPoint where) {

	if (fMouseDown && Bounds().Contains(where)) {
		minimize(min_minimize_all);
	}

	fMouseDown = false;

	DrawBitmap( fIcon );

}

void DeskbarView::MouseMoved( BPoint where, uint32 code, const BMessage *a_message) {
	
	if (fMouseDown) {
		DrawBitmap( Bounds().Contains(where) ? fMinimizedIcon : fIcon );
	}
	
}

void DeskbarView::MessageReceived( BMessage *msg ) {

	if (msg->what=='Remv') {
		BDeskbar deskbar;
		deskbar.RemoveItem(VIEW_NAME);
	} else
		BView::MessageReceived(msg);

}

void DeskbarView::AddToDeskbar()
{
#if defined(__HAIKU__)
	BApplication app(APP_SIGNATURE);
	BDeskbar *deskbar = new BDeskbar();
	app_info info;
	app.GetAppInfo(&info);
	printf("%d: ", deskbar->AddItem(&info.ref) == B_OK);
	delete deskbar;
#elif B_BEOS_VERSION >= B_BEOS_VERSION_5
	BDeskbar *deskbar = new BDeskbar();
	entry_ref ref;
	be_roster->FindApp(APP_SIGNATURE, &ref);
	deskbar->AddItem(&ref);
	delete deskbar;
#else
	BDeskbar *deskbar = new BDeskbar();
	DeskbarView *replicant = new DeskbarView();
	
	status_t err = deskbar->AddItem(replicant);
	delete replicant;
	delete deskbar;
#endif
}

BArchivable *DeskbarView::Instantiate(BMessage *data)
{
	if(!validate_instantiation(data, VIEW_NAME))
		return NULL;
	return new DeskbarView(data);
}

status_t DeskbarView::Archive(BMessage *data, bool deep) const
{
	BView::Archive(data, deep);
	data->AddString("add_on", APP_SIGNATURE);
	data->AddString("class", VIEW_NAME);
	return B_OK;
}

int main()
{
	DeskbarView::AddToDeskbar();
	return 0;
}
