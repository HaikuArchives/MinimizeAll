/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _MINIMIZE_H
#define _MINIMIZE_H

enum min_mode { min_minimize, min_minimize_all, min_maximize_last };

void minimize( min_mode min = min_minimize );

#endif
