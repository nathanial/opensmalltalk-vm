/*
 *  sqMacHostWindow.m
 *  SqueakVMForCarbon
 *
 *  Created by John M McIntosh on Tue Jul 20 2004.
 *  Copyright Corporate Smalltalk Consulting Ltd 2008. All rights reserved.
 *
	July 15th 2005 add logic to flush QD buffers for os-x 10.4
	 3.8.15b3  Feb 19th, 2007 JMM add cursor set logic
	May 15th, 2008 IPhone
	Sept 6th, 2010 Cocoa

 */
/*
 Copyright (c) 2008 Corporate Smalltalk Consulting Ltd. All rights reserved.
 MIT License
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
 The end-user documentation included with the redistribution, if any, must include the following acknowledgment: 
 "This product includes software developed by Corporate Smalltalk Consulting Ltd (http://www.smalltalkconsulting.com) 
 and its contributors", in the same place and form as other third-party acknowledgments. 
 Alternately, this acknowledgment may appear in the software itself, in the same form and location as other 
 such third-party acknowledgments.
 */

//
#include <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>
#include "sq.h"
#include "sqVirtualMachine.h"
#include "sqMacHostWindow.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>

extern struct VirtualMachine *interpreterProxy;
sqInt RemoveWindowBlock(windowDescriptorBlock * thisWindow);
static windowDescriptorBlock *windowListRoot = NULL;

static inline NSScreen *screenForWindow(NSWindow *window) {
	NSScreen *screen = [window screen];
	return screen ? screen : [NSScreen mainScreen];
}

static inline CGFloat screenScale(NSScreen *screen) {
	CGFloat scale = [screen backingScaleFactor];
	return scale > 0.0 ? scale : 1.0;
}

static inline CGFloat windowScale(NSWindow *window) {
	if (window && [window respondsToSelector:@selector(backingScaleFactor)]) {
		CGFloat scale = [window backingScaleFactor];
		if (scale > 0.0) {
			return scale;
		}
	}
	return screenScale(screenForWindow(window));
}

static CGFloat globalMaxYPoints(void) {
	CGFloat maxY = -CGFLOAT_MAX;
	for (NSScreen *screen in [NSScreen screens]) {
		NSRect frame = [screen frame];
		CGFloat top = NSMaxY(frame);
		if (top > maxY) {
			maxY = top;
		}
	}
	return maxY == -CGFLOAT_MAX ? 0.0 : maxY;
}

static inline int16_t clampToInt16(NSInteger value) {
	if (value > INT16_MAX) {
		return INT16_MAX;
	}
	if (value < INT16_MIN) {
		return INT16_MIN;
	}
	return (int16_t)value;
}

static inline sqInt packShortPair(NSInteger hi, NSInteger lo) {
	uint16_t hiPart = (uint16_t)clampToInt16(hi);
	uint16_t loPart = (uint16_t)clampToInt16(lo);
	return (((sqInt)hiPart) << 16) | (sqInt)loPart;
}

static sqInt packTopLeftFromRect(NSRect rect, CGFloat scale, CGFloat maxYPoints) {
	CGFloat left = rect.origin.x;
	CGFloat topFromBottom = maxYPoints - NSMaxY(rect);
	NSInteger leftPixels = (NSInteger)llround(left * scale);
	NSInteger topPixels = (NSInteger)llround(topFromBottom * scale);
	return packShortPair(leftPixels, topPixels);
}

static sqInt packSizeFromRect(NSSize size, CGFloat scale) {
	NSInteger widthPixels = (NSInteger)llround(size.width * scale);
	NSInteger heightPixels = (NSInteger)llround(size.height * scale);
	if (widthPixels < 0) {
		widthPixels = 0;
	}
	if (heightPixels < 0) {
		heightPixels = 0;
	}
	return packShortPair(widthPixels, heightPixels);
}

static inline NSWindow *windowFromHandle(usqIntptr_t handle) {
	if (handle == 0) {
		return nil;
	}
	return (__bridge NSWindow *)(void *)(uintptr_t)handle;
}

sqInt createWindowWidthheightoriginXyattrlength(sqInt w,sqInt h,sqInt x,sqInt y,  char * list, sqInt listLength) {
	return -1;
}

sqInt closeWindow(sqInt windowIndex) {
	NSWindow	*windowHandle;
	windowHandle = windowHandleFromIndex(windowIndex);
	if(windowHandle == NULL) 
		return 0;
	windowBlockFromIndex(windowIndex)->context = NULL;
	RemoveWindowBlock(windowBlockFromIndex(windowIndex));	
	[windowHandle close];
	return 1;
}

sqInt ioPositionOfWindow(wIndexType windowIndex)
{
	NSWindow *window = windowHandleFromIndex(windowIndex);
	if (window == nil) {
		return -1;
	}
	CGFloat scale = windowScale(window);
	CGFloat maxY = globalMaxYPoints();
	return packTopLeftFromRect([window frame], scale, maxY);
}

sqInt ioPositionOfWindowSetxy(wIndexType windowIndex, sqInt x, sqInt y)
{
	NSWindow *window = windowHandleFromIndex(windowIndex);
	if (window == nil) {
		return -1;
	}
	CGFloat scale = windowScale(window);
	CGFloat maxY = globalMaxYPoints();
	NSRect frame = [window frame];
	CGFloat desiredLeft = (CGFloat)x / scale;
	CGFloat desiredTop = (CGFloat)y / scale;
	CGFloat newOriginY = maxY - desiredTop - frame.size.height;
	NSPoint newOrigin = NSMakePoint(desiredLeft, newOriginY);
	[window setFrameOrigin:newOrigin];
	return ioPositionOfWindow(windowIndex);
}

sqInt ioSizeOfWindow(wIndexType windowIndex)
{
	NSWindow *window = windowHandleFromIndex(windowIndex);
	if (window == nil) {
		return -1;
	}
	CGFloat scale = windowScale(window);
	NSRect contentRect = [window contentRectForFrameRect:[window frame]];
	return packSizeFromRect(contentRect.size, scale);
}

sqInt ioPositionOfNativeDisplay(usqIntptr_t windowHandle)
{
	NSWindow *window = windowFromHandle(windowHandle);
	NSScreen *screen = window ? screenForWindow(window) : [NSScreen mainScreen];
	if (screen == nil) {
		return -1;
	}
	CGFloat scale = screenScale(screen);
	CGFloat maxY = globalMaxYPoints();
	return packTopLeftFromRect([screen frame], scale, maxY);
}

sqInt ioSizeOfWindowSetxy(wIndexType windowIndex, sqInt x, sqInt y)
{
	NSWindow *window = windowHandleFromIndex(windowIndex);
	if (window == nil) {
		return -1;
	}
	CGFloat scale = windowScale(window);
	NSSize desiredSize = NSMakeSize(((CGFloat)x) / scale, ((CGFloat)y) / scale);
	[window setContentSize:desiredSize];
	windowDescriptorBlock *block = windowBlockFromIndex(windowIndex);
	if (block) {
		block->width = x;
		block->height = y;
	}
	return ioSizeOfWindow(windowIndex);
}

sqInt ioSetTitleOfWindow(sqInt windowIndex, char * newTitle, sqInt sizeOfTitle) {
	NSWindow *window = windowHandleFromIndex(windowIndex);
	if (window == nil) {
		return -1;
	}
	NSString *title = AUTORELEASEOBJ([[NSString alloc] initWithBytes:newTitle length:sizeOfTitle encoding:NSUTF8StringEncoding]);
	if (title == nil) {
		return -1;
	}
	[window setTitle:title];
	return 1;
}

sqInt ioCloseAllWindows(void) {
	windowDescriptorBlock *entry = windowListRoot;
	while (entry) {
		windowDescriptorBlock *next = entry->next;
		closeWindow(entry->windowIndex);
		entry = next;
	}
	return 1;
}



/* addendum to sqPlatformSpecific.h */
/* multiple host windows stuff */

/* end addendum to sqPlatformSpecific.h */

/* simple linked list management code */
/* window list management */

windowDescriptorBlock *windowBlockFromIndex(sqInt windowIndex) {
windowDescriptorBlock *entry;
	entry = windowListRoot;
	while(entry) {
		if(entry->windowIndex == windowIndex) return entry;
		entry = entry->next;
	}
	return NULL;
}

windowDescriptorBlock *windowBlockFromHandle(wHandleType windowHandle) {
windowDescriptorBlock *entry;
	entry = windowListRoot;
	while(entry) {
		if(entry->handle == windowHandle) return entry;
		entry = entry->next;
	}
	return NULL;
}


wHandleType windowHandleFromIndex(sqInt windowIndex)  {
windowDescriptorBlock *entry;
	entry = windowListRoot;
	while(entry) {
		if(entry->windowIndex == windowIndex) return entry->handle;
		entry = entry->next;
	}
	return NULL;
}

sqInt windowIndexFromHandle(wHandleType windowHandle) {
windowDescriptorBlock *entry;
	entry = windowListRoot;
	while(entry) {
		if(entry->handle == windowHandle) return entry->windowIndex;
		entry = entry->next;
	}
	return 0;
}

sqInt windowIndexFromBlock( windowDescriptorBlock * thisWindow) {
windowDescriptorBlock *entry;
	entry = windowListRoot;
	while(entry) {
		if(entry == thisWindow) return entry->windowIndex;
		entry = entry->next;
	}
	return 0;
}

static sqInt nextIndex = 1; 

windowDescriptorBlock *AddWindowBlock(void) {
/* create a new entry in the linkedlist of windows.
 * If the calloc fails, return NULL which will then go back to the
 * prim and fail it cleanly.
 * Initialize the block to a sensible state
 */
windowDescriptorBlock *thisWindow;

	thisWindow = (windowDescriptorBlock*) calloc(1, sizeof(windowDescriptorBlock));
	if ( thisWindow == NULL) {
		return NULL;
	}
	thisWindow->next = windowListRoot;
	thisWindow->windowIndex = nextIndex++;
	thisWindow->handle = NULL;
	windowListRoot = thisWindow;

	return windowListRoot;
}

/*
 * RemoveWindowBlock:
 * Remove the given entry from the list of windows.
 * free it, if found.
 */
 sqInt RemoveWindowBlock(windowDescriptorBlock * thisWindow) {
windowDescriptorBlock *prevEntry;


	/* Unlink the entry from the module chain */
	if(thisWindow == windowListRoot) {
		windowListRoot = thisWindow->next;
	} else {
		prevEntry = windowListRoot;
		while(prevEntry->next != thisWindow) {
			prevEntry = prevEntry->next;
			if (prevEntry == NULL) {
				return 0;
			}
		}
		prevEntry->next = thisWindow->next;
	}
	free(thisWindow);
	return 1;
}

sqInt getCurrentIndexInUse(void) {
	return nextIndex-1;
}

sqInt ioSizeOfNativeDisplay(usqIntptr_t windowHandle)
{
	NSWindow *window = windowFromHandle(windowHandle);
	NSScreen *screen = window ? screenForWindow(window) : [NSScreen mainScreen];
	if (screen == nil) {
		return -1;
	}
	CGFloat scale = screenScale(screen);
	return packSizeFromRect([screen frame].size, scale);
}

sqInt ioSizeOfNativeWindow(usqIntptr_t windowHandle)
{
	NSWindow *window = windowFromHandle(windowHandle);
	if (window == nil) {
		return -1;
	}
	CGFloat scale = windowScale(window);
	NSRect contentRect = [window contentRectForFrameRect:[window frame]];
	return packSizeFromRect(contentRect.size, scale);
}

sqInt ioPositionOfNativeWindow(usqIntptr_t windowHandle)
{
	NSWindow *window = windowFromHandle(windowHandle);
	if (window == nil) {
		return -1;
	}
	CGFloat scale = windowScale(window);
	CGFloat maxY = globalMaxYPoints();
	return packTopLeftFromRect([window frame], scale, maxY);
}

sqInt ioPositionOfScreenWorkArea(sqIntptr_t windowIndex)
{
	NSWindow *window = windowHandleFromIndex(windowIndex);
	NSScreen *screen = window ? screenForWindow(window) : [NSScreen mainScreen];
	if (screen == nil) {
		return -1;
	}
	CGFloat scale = screenScale(screen);
	CGFloat maxY = globalMaxYPoints();
	return packTopLeftFromRect([screen visibleFrame], scale, maxY);
}

sqInt ioSizeOfScreenWorkArea(sqIntptr_t windowIndex)
{
	NSWindow *window = windowHandleFromIndex(windowIndex);
	NSScreen *screen = window ? screenForWindow(window) : [NSScreen mainScreen];
	if (screen == nil) {
		return -1;
	}
	CGFloat scale = screenScale(screen);
	return packSizeFromRect([screen visibleFrame].size, scale);
}

sqInt ioScreenRectangles(void)
{
	if (interpreterProxy == NULL) {
		return 0;
	}
	NSArray<NSScreen *> *screens = [NSScreen screens];
	NSInteger count = screens.count;
	if (count <= 0) {
		return 0;
	}
	sqInt array = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classArray(), (sqInt)(count * 2));
	if (!array) {
		return 0;
	}
	CGFloat maxY = globalMaxYPoints();
	for (NSInteger i = 0; i < count; i++) {
		NSScreen *screen = screens[i];
		CGFloat scale = screenScale(screen);
		NSRect frame = [screen frame];
		sqInt originPacked = packTopLeftFromRect(frame, scale, maxY);
		sqInt extentPacked = packSizeFromRect(frame.size, scale);
		interpreterProxy->storePointerofObjectwithValue((sqInt)(i * 2), array, interpreterProxy->integerObjectOf(originPacked));
		interpreterProxy->storePointerofObjectwithValue((sqInt)(i * 2 + 1), array, interpreterProxy->integerObjectOf(extentPacked));
	}
	return array;
}

sqInt ioSetCursorPositionXY(long x, long y)
{
	CGPoint target = CGPointMake((CGFloat)x, (CGFloat)y);
	CGError warpResult = CGWarpMouseCursorPosition(target);
	CGAssociateMouseAndMouseCursorPosition(true);
	return warpResult == kCGErrorSuccess ? 1 : -1;
}

sqInt ioSetIconOfWindow(sqIntptr_t windowIndex, char *iconPath, sqInt sizeOfPath)
{
	NSWindow *window = windowHandleFromIndex(windowIndex);
	if (window == nil) {
		return -1;
	}
	NSString *path = AUTORELEASEOBJ([[NSString alloc] initWithBytes:iconPath length:sizeOfPath encoding:NSUTF8StringEncoding]);
	if (path == nil) {
		return -1;
	}
	NSImage *image = AUTORELEASEOBJ([[NSImage alloc] initWithContentsOfFile:path]);
	if (image == nil) {
		return -1;
	}
	[window setRepresentedFilename:path];
	[window setMiniwindowImage:image];
	if ([NSApp respondsToSelector:@selector(setApplicationIconImage:)]) {
		[NSApp setApplicationIconImage:image];
	}
	return 1;
}

void *ioGetWindowHandle(void)
{
	NSWindow *window = windowHandleFromIndex(1);
	return (__bridge void *)window;
}
