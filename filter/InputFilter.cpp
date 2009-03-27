/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "InputFilter.h"

#include <InterfaceDefs.h>
#include <Message.h>
#include <OS.h>
#include <stdio.h>

#include "minimize.h"

//------------------------------------------------------------------------

extern "C" BInputServerFilter* instantiate_input_filter() {
	return new InputFilter();
}

//------------------------------------------------------------------------

InputFilter::InputFilter()
	:	fModifierKey( false )
{
}

filter_result InputFilter::Filter(BMessage *message, BList *outList) {

	switch (message->what) {

		case B_UNMAPPED_KEY_DOWN:
		case B_UNMAPPED_KEY_UP:

			int32 key;
			if (message->FindInt32("key", 0, &key) != B_OK)
				break;
			
			// 102 = left, 103 = right Windows key
			if (key==102 || key==103) {

				fModifierKey = !fModifierKey;
				// So you can still get a win-m key by pressing the second win-key!

			}
			break;
			
		case B_KEY_DOWN:
			{
	
				int32 ch;
				
				if (message->FindInt32("raw_char", 0, &ch) != B_OK)
					break;

				if ( fModifierKey && ch==109 ) { // 109 = m

					min_mode	min = min_minimize;
					if ( modifiers() & B_CONTROL_KEY )		min = min_maximize_last;
					else if ( modifiers() & B_SHIFT_KEY )	min = min_minimize_all;
					
					minimize( min );
					
					return B_SKIP_MESSAGE;

				}
				else {
				
					return B_DISPATCH_MESSAGE;
				
				}
			}
			break;
	}
	
	return B_DISPATCH_MESSAGE;
}

status_t InputFilter::InitCheck() {
	return B_OK;
}
