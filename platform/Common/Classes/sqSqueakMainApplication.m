//
//  sqSqueakMainApplication.m
//  
//
//  Created by John M McIntosh on 5/15/08.
//
/*
 Some of this code was funded via a grant from the European Smalltalk User Group (ESUG)
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
 V1.05b1 fix various issues with memory allocation/free, rework logic so you can free entire interpreter loop on quit. 
 */

//

#import <Foundation/Foundation.h>
#include <sys/stat.h>

#import "sqSqueakMainApplication.h"
#import "sqSqueakMainApp.h"
#import "sqSqueakAppDelegate.h"

#import "sq.h"
#import	"sqSqueakSoundCoreAudio.h"
#import "Queue.h"
#import "HostWindowPlugin.h"
#import "sqMacV2Memory.h"
#include "sqSCCSVersion.h"

// Forward declarations for sqImageFile functions
#define sqImageFile	int
typedef long long squeakFileOffsetType;
extern int sqImageFileOpen(const char *fileName, const char *mode);
extern void sqImageFileClose(int f);
extern size_t readImageFromFileHeapSizeStartingAt(int f, usqInt desiredHeapSize, squeakFileOffsetType imageOffset);

extern BOOL gSqueakHeadless;
extern sqSqueakAppDelegate *gDelegateApp;
char *fullExeName;
char *exeName;

@implementation sqSqueakMainApplication;
@synthesize vmPathStringURL;
@synthesize imageNameURL;
@synthesize inputSemaphoreIndex;
@synthesize currentVMEncoding;
@synthesize eventQueue;
@synthesize fileDirectoryLogic;
@synthesize infoPlistInterfaceLogic;
@synthesize soundInterfaceLogic;
@synthesize argsArguments;
@synthesize commandLineArguments;
@synthesize noHandlers;

extern sqInt interpret(void);  //This is a VM Callback

- (id) init {
	self = [super init];
	if (self) {
		[self setNoHandlers: NO];
	}
	return self;
}

- (void) setupFloat {
}

- (void) setupErrorRecovery {
	attachToSignals();
}

- (void) setInfoPlistInterfaceLogic:(sqSqueakInfoPlistInterface *)anObject {
	infoPlistInterfaceLogic = anObject;
}

- (sqSqueakInfoPlistInterface *) infoPlistInterfaceLogic {
	if (!infoPlistInterfaceLogic) {
		[self fetchPreferences];
	}

	return infoPlistInterfaceLogic;
}

- (sqSqueakInfoPlistInterface *) newSqSqueakInfoPlistInterfaceCreation {
	return [[sqSqueakInfoPlistInterface alloc] init];
}

- (void) fetchPreferences {
	self.infoPlistInterfaceLogic = [self newSqSqueakInfoPlistInterfaceCreation];
	[infoPlistInterfaceLogic parseInfoPlist]; 
	currentVMEncoding = NSUTF8StringEncoding;
}

- (void) doHeadlessSetup {
	gSqueakHeadless = false;
}

- (void) doMemorySetup {
}

- (void) parseUnixArgs {
	fullExeName = [[[[NSBundle mainBundle] executablePath]
					precomposedStringWithCanonicalMapping] UTF8String];
	exeName = strrchr(fullExeName,'/')
				? strrchr(fullExeName,'/') + 1
				: fullExeName;
}

- (void) setupMenus {
//	nothing to do so far since the menu is setup in the MainMenu.nib file
}

- (void) setupTimers {
	extern void SetUpTimers(void);
	SetUpTimers();
}

- (void) setupAIO {
	void aioInit(void);
	aioInit();
	ioInitThreads();
}

- (void) setupEventQueue {
	eventQueue = [[Queue alloc] init];
}

- (void) setupBrowserLogic {
}

- (void) setupSoundLogic {
	soundInterfaceLogic = [[sqSqueakSoundCoreAudio alloc] init];
}

- (sqSqueakFileDirectoryInterface *) newFileDirectoryInterfaceInstance {
	return [[sqSqueakFileDirectoryInterface alloc] init];
}

- (void) runSqueak {
  @autoreleasepool {
	extern BOOL gQuitNowRightNow;
	gQuitNowRightNow=false;

	[self setupFloat];  //JMM We have code for intel and powerpc float, but arm? 
	[self setupErrorRecovery];
	[self fetchPreferences];

	fileDirectoryLogic = [self newFileDirectoryInterfaceInstance];
	[self setVMPathFromApplicationDirectory];
	if (![self.fileDirectoryLogic setWorkingDirectory])
		return;

	[self parseUnixArgs];

	//JMM here we parse the unixArgs
	//JMM now we wait for the open document apple events (normally)

	[self doMemorySetup];

	if ([self ImageNameIsEmpty])
		[self findImageViaBundleOrPreferences];

	if ([self ImageNameIsEmpty])
		return;

	[self setupTimers];

	if (![self readImageIntoMemory])
		return;

	// The headless setup is now after the image setup on purpose. This is to
	// be able to select an image with the popup even when running headless
	[gDelegateApp runBlockOnMainThread:^{
		[self doHeadlessSetup];
		[self setupMenus];
		[self setupBrowserLogic];
		[self setupSoundLogic];
		[gDelegateApp makeMainWindow];
	  }];

	[self setupAIO];
	assert(getVMOSThread() != guiThread);
	assert(getVMOSThread() == ioCurrentOSThread());

	interpret();
  }
}


- (void) MenuBarRestore {
	//	nothing to do so far since the menu is setup in the MainMenu.nib file
}

- (void) ioExit {
	[self ioExitWithErrorCode: 0];
}

- (void) ioExitWithErrorCode: (int) ec {
	ioShutdownAllModules();
	[self MenuBarRestore];
	exit(ec);  //Will not return
}

#ifndef SPURVM
	void sqMacMemoryFree(void);
# endif

- (void)dealloc {
	sqMacMemoryFree();
	SUPERDEALLOC
}


#pragma mark Attributes Methods
- (char *) interpreterVersionString {
	static char data[255];
	bzero(data,sizeof(data));
	strlcat(data,interpreterVersion,sizeof(data));
	strlcat(data," ",sizeof(data));
	NSString *versionString =[[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleVersion"];

	if (versionString == nil)
		return data;
	const char *versonStringAsCString =  [versionString cStringUsingEncoding: [self currentVMEncoding]];

	if (versonStringAsCString == nil)
		return data;

	strlcat(data,versonStringAsCString,sizeof(data));

	return data;
}

- (const char *) getAttribute:(sqInt)indexNumber {
	//indexNumber is a postive/negative number
	if (indexNumber < 0) { // VM argument
        if (-indexNumber <= numVMArgs)
			return (char *) [[self.commandLineArguments objectAtIndex: -indexNumber] cStringUsingEncoding:[self currentVMEncoding]];
		return 0;
	}
#if BUILD_FOR_OSX
	if (indexNumber >= 2 && indexNumber <= 1000) {
        if (indexNumber < ([self.commandLineArguments count] - numVMArgs))
			return (char *) [[self.commandLineArguments objectAtIndex: indexNumber+numVMArgs] cStringUsingEncoding:[self currentVMEncoding]];
		return 0;
	}
#endif
	switch (indexNumber) {
		case 0: // VM name
			return fullExeName;
		case 1: // image name
			return [self getImageName];

		case 1004:  // Interpreter version string
			return [self interpreterVersionString];

		case 1009: // source tree version info
			return sourceVersionString(' ');
		case 1201: // macintosh file name size
			return "255";

		case 1202: // macintosh file error peek
			return "0";

		default: {
			int indexOfArg = indexNumber - 1;
			if (indexOfArg < [self.argsArguments count])
				return (char *) [[self.argsArguments objectAtIndex: (NSUInteger) indexOfArg] cStringUsingEncoding:[self currentVMEncoding]];
		}
	}
	return 0;
}

#pragma mark Cursor Methods
- (void) setCursor: (sqInt)  cursorBitsIndex withMask: (sqInt)  cursorMaskIndex
		   offsetX: (sqInt) offsetX offsetY: (sqInt) offsetY {
}

#pragma mark Events Methods
- (void) pumpRunLoopEventSendAndSignal:(BOOL)signal {
    //It should be redefined by my children
}

- (void) pumpRunLoop {
/*	static NSTimeInterval old = 0.0;
	
	NSTimeInterval now = [NSDate timeIntervalSinceReferenceDate];
	if ((now - old) < 1.0/60.0) {
		return;
	}
	
	old = now;
*/	
	/* This the carbon logic model 
	 described by http://developer.apple.com/qa/qa2001/qa1061.html
	 but fails on device, bug tracking number  5971848 */

	BOOL result = [[NSRunLoop mainRunLoop] 
            runMode:NSDefaultRunLoopMode 
            beforeDate:[NSDate distantPast]];
	
	//		while(CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, TRUE) == kCFRunLoopRunHandledSource);
	
	// TEST WHICH IS BETTER? 		SInt32 what = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
#pragma unused(result)
}

- (sqInt) ioGetNextEvent: (sqInputEvent *) evt {
	
	ioProcessEvents();
	id event = [eventQueue returnAndRemoveOldest];
	if (event) {
		@autoreleasepool {
            [self processAsOldEventOrComplexEvent: event placeIn: evt];
        }
        RELEASEOBJ(event);
		return 1;
	}
	return 0;
}

- (void ) processAsOldEventOrComplexEvent: (id) event placeIn: (sqInputEvent *) evt {
	
	if ([event isKindOfClass: [NSData class]]) {
		[event getBytes: evt length: sizeof(sqInputEvent)];
		return;
	}
}

#pragma mark Imagereadwrite Methods
- (void) findImageViaBundleOrPreferences {
}

- (BOOL) readImageIntoMemory {
			@autoreleasepool {
	const char * characterPathForImage = (const char *) [[NSFileManager defaultManager] fileSystemRepresentationWithPath: [self.imageNameURL path]];
	sqImageFile f;
	if (!characterPathForImage)  {
		return NO;
	}
	f = sqImageFileOpen(characterPathForImage, "rb");
	if (f == 0) {
		fprintf(stderr, "Failed to open image named %s", characterPathForImage);
		exit(-1);
	}

#ifdef SPURVM
    extern sqInt highBit(usqInt);
    usqInt memory = 0;
    {
        struct stat sb;
        stat(characterPathForImage, &sb);

        off_t size = (long)sb.st_size;
        size = 1 << highBit(size-1);
        size = size + size / 4;
        memory =  size + size / 4;
    }
#else
    usqInt memory = sqGetAvailableMemory();
#endif
	readImageFromFileHeapSizeStartingAt(f, memory, (squeakFileOffsetType) 0);  //This is a VM Callback
	sqImageFileClose(f);
            }
	return YES;
}

#pragma mark Sound Methods
- (void) ioBeep {
#ifdef BUILD_FOR_OSX
    NSBeep();
#endif
}

#pragma mark Vmandimagepath Methods
- (void) setVMPathFromApplicationDirectory {
    // in the default case return /Applications otherwise the path the .app is contained in
	self.vmPathStringURL = [NSURL fileURLWithPath: [[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent]];
}

- (BOOL) ImageNameIsEmpty {
	if (self.imageNameURL) 
		return NO;
	return YES;
}

- (const char *) getImageName {
	return [[[self.imageNameURL path] precomposedStringWithCanonicalMapping] UTF8String];
}

- (void) imageNameGet:(char *) sqImageName length:(sqInt)length {
	//sqImageName is actually the address of where to put the data
	//length is how many bytes the target can hold
	if (!sqImageName) 
		return;
	
	*sqImageName = 0x00;
	if (length > 0)
		strncpy(sqImageName, [self getImageName],(size_t) length); //This does not need to be strlcpy since the data is not null terminated
	/* ok do we need to check for length of getImageName? 
	 if length > strlen(getImageName) then it fills with 0x00
	 if length < strlen(getImageName) then we get a partial string and no trailing /0
	 if length == strlen(getImageName) then we get the string and no trailing /0 which is what is desired. */
}

- (void) imageNamePut:(const char *) sqImageName {
	
	if (!sqImageName) 
		return;
	self.imageNameURL = [NSURL fileURLWithPath: @(sqImageName) isDirectory: NO];
}

- (const char *) getVMPath {
    return [[[[self vmPathStringURL] path] precomposedStringWithCanonicalMapping] UTF8String];
}

- (void) vmPathGet:(char *) sqVMPath length:(sqInt)length {
	//sqVMPath is actually the address of where to put the data
	//length is how many bytes the target can hold
	
	if (!sqVMPath) 
		return;
	*sqVMPath = 0x00;
	if (length > 0) {
		strlcpy(sqVMPath, [self getVMPath], (size_t) length);
		sqVMPath[length-1] = '/'; // BUG length here is 1 offset, so need to subtract 1 to add on the '/' which is +1 via VMPathLength
	}
}

@end

int
plugInTimeToReturn(void) {
	extern BOOL	gQuitNowRightNow;

	if (gQuitNowRightNow)
		return true;
	return false;
}


sqInt
convertToSqueakTime(NSDate *givenDate)
{
	time_t unixTime = [givenDate timeIntervalSince1970];

#if defined(HAVE_TM_GMTOFF)
	unixTime+= localtime(&unixTime)->tm_gmtoff;
#elif defined(HAVE_TIMEZONE)
	unixTime+= ((daylight) * 60*60) - timezone;
#else
#  error: cannot determine timezone correction
#endif
	/* Squeak epoch is Jan 1, 1901.  Unix epoch is Jan 1, 1970: 17 leap years
	 and 52 non-leap years later than Squeak. */
	return unixTime + ((52*365UL + 17*366UL) * 24*60*60UL);
}
