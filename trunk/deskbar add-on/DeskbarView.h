/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _DESKBAR_VIEW_H
#define _DESKBAR_VIEW_H

#include <View.h>

#define APP_SIGNATURE	"application/x-vnd.pecora-minimizeall_deskbar"
#define VIEW_NAME		"MinimizeAll Deskbar View"
#define VIEW_RECT		BRect(0, 0, 15, 15)

class _EXPORT DeskbarView;

class BBitmap;
class BPopUpMenu;
class DeskbarView : public BView
{

public:

				DeskbarView();
				DeskbarView(BMessage *archive);
				~DeskbarView();
				
void			Init();
		
void			AttachedToWindow(void);
void			Draw(BRect rect);
void			MessageReceived( BMessage *msg );

void			MouseDown(BPoint where);
void			MouseUp(BPoint where);
void			MouseMoved(	BPoint where,
							uint32 code,
							const BMessage *a_message);

static void		AddToDeskbar();
		
static			BArchivable *Instantiate(BMessage *data);
status_t		Archive(BMessage *data, bool deep = true) const;

private:

bool			fMouseDown;

BBitmap			*fIcon, *fMinimizedIcon;
BPopUpMenu		*fPopUpMenu;
		
};

#endif
