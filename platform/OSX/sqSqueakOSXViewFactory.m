#import "sqSqueakOSXViewFactory.h"
#import "SqueakOSXAppDelegate.h"
#import "sq.h"
#import "sqVirtualMachine.h"
#import "sqSqueakOSXMetalView.h"

extern SqueakOSXAppDelegate *gDelegateApp;
extern struct	VirtualMachine* interpreterProxy;

sqOSXRequestedViewType sqCurrentOSXRequestedViewType = SQ_OSX_REQUESTED_VIEW_TYPE_ANY;

@implementation sqSqueakOSXViewFactory

#pragma mark Initialization / Release

+ (id) getRequestedViewClass {
    // Only Metal view is supported
    return [sqSqueakOSXMetalView class];
}

+ (id) allocWithZone: (struct _NSZone *)zone {
    return [[self getRequestedViewClass] allocWithZone: zone];
}

+ (id) alloc {
    return [[self getRequestedViewClass] alloc];
}

+ (id) new {
    return [[self getRequestedViewClass] new];
}

@end
