/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "minimize_all.h"

#include <Application.h>

int main() {

	be_app = new BApplication("application/x-vnd.pecora-minimizeall");

	minimize_all();
	return 0;

	delete be_app;
	
}
