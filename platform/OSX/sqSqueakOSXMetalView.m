//
//  sqSqueakOSXOpenMetalView.m
//  SqueakPureObjc
//
//  Created by Ronie Salgado on 10-11-18.
//  Event code taken from sqSqueakOSXOpenGLView.m
/*
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

#ifdef USE_METAL
#import <QuartzCore/QuartzCore.h>

#import "sqSqueakOSXMetalView.h"
#import "sqSqueakOSXScreenAndWindow.h"
#import "SqueakOSXAppDelegate.h"
#import "sqSqueakOSXApplication+events.h"
#import "sqSqueakOSXInfoPlistInterface.h"
#import "sq.h"

extern SqueakOSXAppDelegate *gDelegateApp;

static sqSqueakOSXMetalView *mainMetalView;
#if !SPURVM
extern sqInt cannotDeferDisplayUpdates;
#endif

#define STRINGIFY_SHADER(src) #src
static const char *squeakMainShadersSrc =
#include "SqueakMainShaders.metal"
;

typedef struct
{
	float x, y, z, w, s, t;
} ScreenQuadVertex;

static ScreenQuadVertex screenQuadVertices[] = {
	{-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	{ 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
	{-1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{ 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
};

#define MAX_NUMBER_OF_EXTRA_LAYERS 16

typedef struct LayerTransformation
{
    float scaleX, scaleY;
    float translationX, translationY;
} LayerTransformation;

@interface sqSqueakOSXMetalViewExtraDrawingLayer : NSObject {
	id<MTLTexture> texture;
	int x, y;
	int w, h;	
}

@property (nonatomic,assign) int x;
@property (nonatomic,assign) int y;
@property (nonatomic,assign) int w;
@property (nonatomic,assign) int h;
@property (nonatomic,strong) id<MTLTexture> texture;
@end

@implementation sqSqueakOSXMetalViewExtraDrawingLayer
@synthesize x, y, w, h, texture;
@end

@interface sqSqueakOSXMetalView ()
@property (nonatomic,assign) CGSize lastFrameSize;
@property (nonatomic,strong) id<MTLCommandQueue> graphicsCommandQueue;
@end

@implementation sqSqueakOSXMetalView
@synthesize lastFrameSize,graphicsCommandQueue;

#include "SqSqueakOSXView.m.inc"

+ (BOOL) isMetalViewSupported {
	// Try to create the MTL system device.
	id<MTLDevice> device = MTLCreateSystemDefaultDevice();
	if(!device)
		return NO;

	// Try to compile the shader library.
	id<MTLLibrary> library = [self compileShaderLibraryForDevice: device];
	if(!library)
		return NO;

	RELEASEOBJ(library);
	RELEASEOBJ(device);
	return YES;
}

#pragma mark Initialization / Release

- (id)initWithFrame:(NSRect)frameRect {
	self = [super initWithFrame:frameRect];

    [self setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    [self setAutoresizesSubviews:YES];

    [self initialize];

    return self;
}

- (void)awakeFromNib {
	[super awakeFromNib];
    [self initialize];
}

- (void)initialize {
	mainMetalView = self;

	self.paused = YES;
	self.enableSetNeedsDisplay = YES;
#if !SPURVM // Spur VMs pin the display bits and so textures remain valid
	cannotDeferDisplayUpdates = 1;
#endif

	NSMutableArray *drawingLayers = [NSMutableArray arrayWithCapacity: MAX_NUMBER_OF_EXTRA_LAYERS];
	for(int i = 0; i < MAX_NUMBER_OF_EXTRA_LAYERS; ++i) {
		[drawingLayers addObject: [sqSqueakOSXMetalViewExtraDrawingLayer new]];
	}
	extraDrawingLayers = drawingLayers;

	inputMark = NSMakeRange(NSNotFound, 0);
	inputSelection = NSMakeRange(0, 0);
    [self registerForDraggedTypes: [NSArray arrayWithObjects: NSPasteboardTypeFileURL, nil]];
	metalInitialized = NO;
	displayTexture = nil;
	displayTextureWidth = 0;
	displayTextureHeight = 0;

	dragInProgress = NO;
	dragCount = 0;
	clippyIsEmpty = YES;
	colorspace = CGColorSpaceCreateDeviceRGB();
	[self initializeSqueakColorMap];
    [[NSNotificationCenter defaultCenter] addObserver:self selector: @selector(didEnterFullScreen:) name:@"NSWindowDidEnterFullScreenNotification" object:nil];

    // macOS 10.5 introduced NSTrackingArea for mouse tracking
    NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect: [self frame]
    	options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveAlways | NSTrackingInVisibleRect)
    	owner: self userInfo: nil];
    [self addTrackingArea: trackingArea];
}

- (void) didEnterFullScreen: (NSNotification*) aNotification {
    //NSLog(@"Notification didEnterFullScreen");
}

- (void) initializeVariables {
}

- (void) dealloc {
	free(colorMap32);
	CGColorSpaceRelease(colorspace);
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    SUPERDEALLOC
}

#pragma mark Testing

- (BOOL) acceptsFirstResponder {
	return YES;
}

- (BOOL)isFlipped {
	return  YES;
}

- (BOOL)isOpaque {
	return YES;
}

- (NSRect) sqScreenSize {
  return [self convertRectToBacking: [self bounds]];
}


- (NSPoint) sqMousePosition: (NSEvent*)theEvent {
	/* Our client expects the mouse coordinates in Squeak's coordinates,
	 * but theEvent's location is in "user" coords. so we have to convert. */
	NSPoint local_pt = [self convertPoint: [theEvent locationInWindow] fromView:nil];
	NSPoint converted = [self convertPointToBacking: local_pt];
	// Squeak is upside down
	return NSMakePoint(converted.x, -converted.y);
}

- (NSPoint) sqDragPosition: (NSPoint)draggingLocation {
	// TODO: Reuse conversion from sqMousePosition:.
	NSPoint local_pt = [self convertPoint: draggingLocation fromView: nil];
	NSPoint converted = [self convertPointToBacking: local_pt];
	return NSMakePoint(converted.x, -converted.y);
}


#pragma mark Drawing

- (void) drawImageUsingClip: (CGRect) clip {

	if (clippyIsEmpty){
		clippy = clip;
		clippyIsEmpty = NO;
	} else {
		clippy = CGRectUnion(clippy, clip);
	}

	/* After updating clippy, tell the application to issue a display event.
	 * We do this via setNeedsDisplayInRect: for the entire frame but only
	 * once to avoid unnecessary updates. See initialize() where we pause
	 * the Metal loop and enableSetNeedsDisplay.
	 *
	 * Note that we do not have to communicate clippy in some way because
	 * drawRect: will always be called with the entire frame during event
	 * processing, whether we tell it to do so or not.
	 */
	if(!syncNeeded) {
		syncNeeded = YES;
		[self setNeedsDisplayInRect: [self frame]];
	}
}

- (void) drawTheLayers /* via ioForceDisplayUpdate() */{
    extern BOOL gSqueakHeadless;
	if (gSqueakHeadless) {
        firstDrawCompleted = YES;
        return;
    }

	/* Documentation only. DO NOT draw here but rely the application's event
	 * loop such as through image-side pumping. See ioProcessEvents().
	 *
	 * Note that we MUST NOT check deferDisplayUpdates because its semantics
	 * expect an extra display buffer which we do not have. We just record
	 * clippy without preserving the particular bits from displayBits. That
	 * is, relying on the application's event loop to "defer" display updates
	 * is not the same. Instead, it is about who manages the extra buffer if
	 * needed.
     */
	// NO: if (syncNeeded) { [self draw]; }
    // NO: if (!deferDisplayUpdates && syncNeeded) { [self draw]; }

	if (!firstDrawCompleted) {
		firstDrawCompleted = YES;
		extern sqInt getFullScreenFlag(void);
		if (getFullScreenFlag() == 0) {
			[self.window makeKeyAndOrderFront: self];
        }
	}
}

-(void)setupMetal
{
	metalInitialized = YES;
	if(self.device == nil)
		self.device = MTLCreateSystemDefaultDevice();

	[self buildPipelines];
	[self createScreenQuad];

	// Create the command queue.
	graphicsCommandQueue = [self.device newCommandQueue];
}

+ (id<MTLLibrary>) compileShaderLibraryForDevice: (id<MTLDevice>) device {
	NSString *shaderSource = [NSString stringWithCString: squeakMainShadersSrc encoding: NSUTF8StringEncoding];
	MTLCompileOptions* compileOptions = [ MTLCompileOptions new ];
	NSError *libraryError = nil;
	id<MTLLibrary> shaderLibrary = [device newLibraryWithSource: shaderSource options: compileOptions error: &libraryError];
	RELEASEOBJ(shaderSource);
	RELEASEOBJ(compileOptions);
	if(!shaderLibrary)
		NSLog(@"Shader library error: %@", libraryError.localizedDescription);

	return shaderLibrary;
}

- (void) buildPipelines {
	id<MTLLibrary> shaderLibrary = [[self class] compileShaderLibraryForDevice: self.device ];
	screenQuadPipelineState = [self buildPipelineWithLibrary: shaderLibrary vertexFunction: @"screenQuadFlipVertexShader" fragmentFunction: @"screenQuadFragmentShader" translucent: NO];
	layerScreenQuadPipelineState = [self buildPipelineWithLibrary: shaderLibrary vertexFunction: @"layerScreenQuadVertexShader" fragmentFunction: @"screenQuadFragmentShader" translucent: NO];
	RELEASEOBJ(shaderLibrary);
}

- (id<MTLRenderPipelineState>) buildPipelineWithLibrary: (id<MTLLibrary>)shaderLibrary vertexFunction: (NSString*)vertexFunctionName fragmentFunction: (NSString*)fragmentFunctionName translucent: (BOOL)translucent{
	// Retrieve the shaders from the shader libary.
	id<MTLFunction> vertexShader = [shaderLibrary newFunctionWithName: vertexFunctionName];
	id<MTLFunction> fragmentShader = [shaderLibrary newFunctionWithName: fragmentFunctionName];
	if(!vertexShader || !fragmentShader)
	{
		RELEASEOBJ(shaderLibrary);
		return nil;
	}

	// Create the screen quad pipeline.
	MTLRenderPipelineDescriptor *pipelineDescriptor = [MTLRenderPipelineDescriptor new];
	pipelineDescriptor.vertexFunction = vertexShader;
	pipelineDescriptor.fragmentFunction = fragmentShader;
	pipelineDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassTriangle;
	MTLRenderPipelineColorAttachmentDescriptor *attachmentDescriptor = pipelineDescriptor.colorAttachments[0];
	attachmentDescriptor.pixelFormat = self.colorPixelFormat;
	if(translucent) {
		attachmentDescriptor.blendingEnabled = YES;
		attachmentDescriptor.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
		attachmentDescriptor.sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
		attachmentDescriptor.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
		attachmentDescriptor.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
	}

	NSError *pipelineError = NULL;
	id<MTLRenderPipelineState> pipeline = [self.device newRenderPipelineStateWithDescriptor: pipelineDescriptor error: &pipelineError];
	RELEASEOBJ(shaderLibrary);
	RELEASEOBJ(vertexShader);
	RELEASEOBJ(fragmentShader);
	if(!pipeline)
	{
		NSLog(@"Pipeline state creation error: %@", pipelineError.localizedDescription);
		return nil;
	}

	return pipeline;
}

- (void) createScreenQuad {
	screenQuadVertexBuffer = [self.device
		newBufferWithBytes: screenQuadVertices
		length: sizeof(screenQuadVertices)
		options: MTLResourceStorageModeManaged];
}

-(void)drawRect:(NSRect)rect
{
	if (!metalInitialized)
		[self setupMetal];

    /* Only draw if we have valid access to displayBits and if we have
     * something new communicated via clippy. During window resizing, we can
     * just ignore this as the framework will stretch the current contents as
     * preview. We might want to avoid this and show blank contents instead.
     * Just check clippyIsEmpty to decide whether to update rect, typically
     * the full frame, with something else or not.
     */
    if (!displayBits || clippyIsEmpty)
		return;

	// Always try to fill the texture with the pixels.
	[self loadTexturesSubRectangle: NSRectFromCGRect(clippy)];
	clippyIsEmpty = YES;
	syncNeeded = NO;

	MTLRenderPassDescriptor *renderPassDescriptor = self.currentRenderPassDescriptor;
	if(renderPassDescriptor != nil && self.currentDrawable)
	{
		currentCommandBuffer = [graphicsCommandQueue commandBuffer];
		currentRenderEncoder = [currentCommandBuffer renderCommandEncoderWithDescriptor: renderPassDescriptor];

		// Set the viewport.
		[currentRenderEncoder setViewport: (MTLViewport){0.0, 0.0, self.drawableSize.width, self.drawableSize.height}];

		// Draw the screen rectangle.
		[self drawScreenRect: rect];

		unsigned int drawnExtraDrawingLayerMask = 0;
		for(unsigned int i = 0; i < MAX_NUMBER_OF_EXTRA_LAYERS && drawnExtraDrawingLayerMask != allocatedExtraDrawingLayers; ++i) {
			if(allocatedExtraDrawingLayers & (1 << i)) {
				[self drawExtraDrawingLayer: i];
				drawnExtraDrawingLayerMask |= 1 << i;
			}
		}

		[currentRenderEncoder endEncoding];
		[currentCommandBuffer presentDrawable: self.currentDrawable];
		[currentCommandBuffer commit];

		currentCommandBuffer = nil;
		currentRenderEncoder = nil;
	}
}

- (void)loadTexturesSubRectangle: (NSRect) subRect {
	CGSize drawableSize = CGSizeMake(displayWidth, displayHeight);

    if ( !CGSizeEqualToSize(lastFrameSize,drawableSize)
    	|| !displayTexture
    	|| currentDisplayStorage != displayBits) {
		// NSLog(@"old %f %f new %f %f", lastFrameSize.width,lastFrameSize.height,drawableSize.width,drawableSize.height);
        lastFrameSize = drawableSize;
		currentDisplayStorage = displayBits;
		[self updateDisplayTextureStorage: drawableSize];
    }

	// Clip the subrect against the texture bounds, to avoid an edge condition
	// that ends crashing the VM.
	subRect = NSIntersectionRect(subRect, NSMakeRect(0, 0, displayTextureWidth, displayTextureHeight));
	if(NSIsEmptyRect(subRect))
	{
		// Discard the update of empty texture regions.
		return;
	}

	MTLRegion region = MTLRegionMake2D(subRect.origin.x, displayTextureHeight - subRect.origin.y - subRect.size.height, subRect.size.width, subRect.size.height);

	unsigned int sourcePitch = displayTextureWidth * 4;

#define BYTES_PER_PIXEL 4

	char *source = (char*)displayBits
				+ BYTES_PER_PIXEL *
					(unsigned long)(subRect.origin.x
								+ displayTextureWidth * (displayTextureHeight - subRect.origin.y - subRect.size.height));
	[displayTexture replaceRegion: region mipmapLevel: 0 withBytes: source bytesPerRow: sourcePitch];
}

-(void) updateDisplayTextureStorage: (CGSize) drawableSize {
	displayTextureWidth = drawableSize.width;
	displayTextureHeight = drawableSize.height;
	displayTexturePitch = displayTextureWidth * 4;

	if(displayTexture)
	{
		RELEASEOBJ(displayTexture);
		displayTexture = nil;
	}

	MTLTextureDescriptor *descriptor = [MTLTextureDescriptor
		texture2DDescriptorWithPixelFormat: MTLPixelFormatBGRA8Unorm
		width: displayTextureWidth
		height: displayTextureHeight
		mipmapped: NO];
	displayTexture = [self.device newTextureWithDescriptor: descriptor];
}

-(void)drawScreenRect:(NSRect)rect {
	if(displayTexture == nil || screenQuadPipelineState == nil || screenQuadVertexBuffer == nil)
		return;

	[currentRenderEncoder setRenderPipelineState: screenQuadPipelineState];

	[currentRenderEncoder setVertexBuffer: screenQuadVertexBuffer offset: 0 atIndex: 0];
	[currentRenderEncoder setFragmentTexture: displayTexture atIndex: 0];

	// Draw the the 4 vertices of the of the screen quad.
	[currentRenderEncoder drawPrimitives: MTLPrimitiveTypeTriangleStrip
		vertexStart: 0
		vertexCount: 4];
}

- (void) drawExtraDrawingLayer: (unsigned int) extraDrawingLayerIndex {
	sqSqueakOSXMetalViewExtraDrawingLayer *layer = extraDrawingLayers[extraDrawingLayerIndex];
	if(!layer.texture)
		return;

	if(layerScreenQuadPipelineState == nil || screenQuadVertexBuffer == nil)
		return;

	[currentRenderEncoder setRenderPipelineState: layerScreenQuadPipelineState];

	NSRect screenRect = self.frame;
	LayerTransformation transformation;
	transformation.scaleX = (float)layer.w / screenRect.size.width;
	transformation.scaleY = (float)layer.h / screenRect.size.height;
	transformation.translationX = (2.0f*layer.x + layer.w) / screenRect.size.width - 1.0f;
	transformation.translationY = 1.0f - (2.0f*layer.y + layer.h) / screenRect.size.height;

	[currentRenderEncoder setVertexBuffer: screenQuadVertexBuffer offset: 0 atIndex: 0];
	[currentRenderEncoder setVertexBytes: &transformation length: sizeof(transformation) atIndex: 1];
	[currentRenderEncoder setFragmentTexture: layer.texture atIndex: 0];

	// Draw the the 4 vertices of the of the screen quad.
	[currentRenderEncoder drawPrimitives: MTLPrimitiveTypeTriangleStrip
		vertexStart: 0
		vertexCount: 4];
}


#pragma mark Fullscreen

- (void) ioSetFullScreen: (sqInt) fullScreen {
	if ((self.window.styleMask & NSFullScreenWindowMask) != (fullScreen == 1)) {
        [self.window toggleFullScreen: nil];
	}
}

- (void) preDrawThelayers {
}

- (unsigned int) createTextureLayerHandle {
	unsigned int i;
	unsigned int bit;
	for(i = 0; i < MAX_NUMBER_OF_EXTRA_LAYERS; ++i) {
		bit = 1<<i;
		if(!(allocatedExtraDrawingLayers & bit)) {
			allocatedExtraDrawingLayers |= bit;
			return i + 1;
		}
	}
	return 0;
}

- (void) destroyTextureLayerHandle: (unsigned int) handle {
	unsigned int bit = 1 << (handle - 1);
	if(allocatedExtraDrawingLayers & bit) {
		sqSqueakOSXMetalViewExtraDrawingLayer *layer = extraDrawingLayers[handle - 1];
		if(layer.texture) {
			RELEASEOBJ(layer.texture);
			layer.texture = nil;
		}

		allocatedExtraDrawingLayers &= ~bit;

		// Redraw the screen.
		[self draw];
	}	
}

- (void) setExtraLayer: (unsigned int) handle texture: (id<MTLTexture>) texture x: (int) x y: (int) y w: (int) w h: (int) h {
	unsigned int bit = 1 << (handle - 1);
	if(allocatedExtraDrawingLayers & bit) {
		sqSqueakOSXMetalViewExtraDrawingLayer *layer = extraDrawingLayers[handle - 1];
		RETAINOBJ(texture);
		if(layer.texture)
			RELEASEOBJ(texture);
		layer.texture = texture;
		layer.x = x;
		layer.y = y;
		layer.w = w;
		layer.h = h;

		// Swap the buffers
		if(mainMetalView)
			[mainMetalView draw];
	}
}

#pragma mark Color Map Methods

-(void)initializeSqueakColorMap
{
	colorMap32=(unsigned int*)malloc(sizeof(unsigned int)*256);

#undef SetColorEntry
#define SetColorEntry(i,r,g,b)	[self setColorEntry:i red:r green:g blue:b]

	/* 1-bit colors (monochrome) */
	SetColorEntry(0, 65535, 65535, 65535);    /* white or transparent */
	SetColorEntry(1,     0,     0,     0);    /* black */

	/* additional colors for 2-bit color */
	SetColorEntry(2, 65535, 65535, 65535);    /* opaque white */
	SetColorEntry(3, 32768, 32768, 32768);    /* 1/2 gray */
	/* additional colors for 4-bit color */
	SetColorEntry( 4, 65535,     0,     0);   /* red */
	SetColorEntry( 5,     0, 65535,     0);   /* green */
	SetColorEntry( 6,     0,     0, 65535);   /* blue */
	SetColorEntry( 7,     0, 65535, 65535);   /* cyan */
	SetColorEntry( 8, 65535, 65535,     0);   /* yellow */
	SetColorEntry( 9, 65535,     0, 65535);   /* magenta */

	SetColorEntry(10,  8192,  8192,  8192);   /* 1/8 gray */
	SetColorEntry(11, 16384, 16384, 16384);   /* 2/8 gray */
	SetColorEntry(12, 24576, 24576, 24576);   /* 3/8 gray */
	SetColorEntry(13, 40959, 40959, 40959);   /* 5/8 gray */
	SetColorEntry(14, 49151, 49151, 49151);   /* 6/8 gray */
	SetColorEntry(15, 57343, 57343, 57343);   /* 7/8 gray */

	/* additional colors for 8-bit color */
	/* 24 more shades of gray (does not repeat 1/8th increments) */
	SetColorEntry(16,  2048,  2048,  2048);   /*  1/32 gray */
	SetColorEntry(17,  4096,  4096,  4096);   /*  2/32 gray */
	SetColorEntry(18,  6144,  6144,  6144);   /*  3/32 gray */
	SetColorEntry(19, 10240, 10240, 10240);   /*  5/32 gray */
	SetColorEntry(20, 12288, 12288, 12288);   /*  6/32 gray */
	SetColorEntry(21, 14336, 14336, 14336);   /*  7/32 gray */
	SetColorEntry(22, 18432, 18432, 18432);   /*  9/32 gray */
	SetColorEntry(23, 20480, 20480, 20480);   /* 10/32 gray */
	SetColorEntry(24, 22528, 22528, 22528);   /* 11/32 gray */
	SetColorEntry(25, 26624, 26624, 26624);   /* 13/32 gray */
	SetColorEntry(26, 28672, 28672, 28672);   /* 14/32 gray */
	SetColorEntry(27, 30720, 30720, 30720);   /* 15/32 gray */
	SetColorEntry(28, 34815, 34815, 34815);   /* 17/32 gray */
	SetColorEntry(29, 36863, 36863, 36863);   /* 18/32 gray */
	SetColorEntry(30, 38911, 38911, 38911);   /* 19/32 gray */
	SetColorEntry(31, 43007, 43007, 43007);   /* 21/32 gray */
	SetColorEntry(32, 45055, 45055, 45055);   /* 22/32 gray */
	SetColorEntry(33, 47103, 47103, 47103);   /* 23/32 gray */
	SetColorEntry(34, 51199, 51199, 51199);   /* 25/32 gray */
	SetColorEntry(35, 53247, 53247, 53247);   /* 26/32 gray */
	SetColorEntry(36, 55295, 55295, 55295);   /* 27/32 gray */
	SetColorEntry(37, 59391, 59391, 59391);   /* 29/32 gray */
	SetColorEntry(38, 61439, 61439, 61439);   /* 30/32 gray */
	SetColorEntry(39, 63487, 63487, 63487);   /* 31/32 gray */

	/* The remainder of color table defines a color cube with six steps
		for each primary color. Note that the corners of this cube repeat
		previous colors, but simplifies the mapping between RGB colors and
		color map indices. This color cube spans indices 40 through 255.
	*/
	{
		unsigned int r,g,b;

		for(r=0;r<6;r++){
			for(g=0;g<6;g++){
				for(b=0;b<6;b++){
					unsigned int i=40+((36*r)+(6*b)+g);
					if(i>255){
						fprintf(stderr,"index out of range "
								"in color table compuation\n");
					}
					SetColorEntry(i,(r*65535)/5,(g*65535)/5,(b*65535)/5);
				}
			}
		}
	}
}//initializeSqueakColorMap;


-(void)setColorEntry:(unsigned int)i red:(unsigned int)r green:(unsigned int)g blue:(unsigned int)b
{
     colorMap32[i]=NSSwapHostIntToBig((((r)&0xff00)<<16)|(((g)&0xff00)<<8)|((b)&0xff00)|0xff);
}

#pragma mark Bitmap Conversion Methods

#undef debug
#undef CHECKANDRETURN

#define debug(a)

#define CHECKANDRETURN(expr) \
	{ NSBitmapImageRep* bitmap=expr; if(bitmap!=NULL){ CGImageRef ref = [bitmap CGImage]; return(ref);}\
        NSLog(@"initData:%08x\npixelsWide:%d\npixelsHigh:%d\nbitsPerSample:%d\nsamplesPerPixel:%d\nhasAlpha:%d\nisPlanar:%d\ncolorSpaceName:%@\nbytesPerRow:%d\nbitsPerPixel:%d\n",\
				(int)dBits,right-left,bottom-top,bitsPerSample,\
				samplesPerPixel,NO,NO,colorSpace,bytesPerRow,bitsPerPixel);\
		return(0);\
	}

-(CGImageRef)extractPixels_1_to_32:(void*)srcBits
					 srcPixelWidth:(int)srcPixelWidth height: (int) height
							  left:(int)left right:(int)right
							   top:(int)top bottom:(int)bottom
						  colorMap: (unsigned int *) colorMap
						tempMemory: (void **) tempMemory

{
	int                 bitsPerSample=8;
	int                 samplesPerPixel=3;
	int                 bitsPerPixel=32;
	int                 bytesPerRow=bytesPerLine(srcPixelWidth, 32);
	NSString*			colorSpace=NSDeviceRGBColorSpace;
	size_t				totalSize=bytesPerRow * ((bottom-top)+1);
	int*				dBits=(int*)malloc(totalSize);
	*tempMemory = dBits;


	copyImage1To32(srcBits, dBits, srcPixelWidth, height, left, top, right, bottom,colorMap);


	CHECKANDRETURN(AUTORELEASEOBJ([[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(unsigned char **)&dBits
														   pixelsWide:right-left
														   pixelsHigh:bottom-top
														bitsPerSample:bitsPerSample
													  samplesPerPixel:samplesPerPixel
															 hasAlpha:NO
															 isPlanar:NO
													   colorSpaceName:colorSpace
														  bytesPerRow:bytesPerRow
														 bitsPerPixel:bitsPerPixel]));
}//extractPixels_1_to_32;


-(CGImageRef)extractPixels_2_to_32:(void*)srcBits
					 srcPixelWidth:(int)srcPixelWidth height: (int) height
							  left:(int)left right:(int)right
							   top:(int)top bottom:(int)bottom
						  colorMap: (unsigned int *) colorMap
						tempMemory: (void **) tempMemory
{
	int                 bitsPerSample=8;
	int                 samplesPerPixel=3;
	int                 bitsPerPixel=32;
	int                 bytesPerRow=bytesPerLine(srcPixelWidth, 32);
	NSString*			colorSpace=NSDeviceRGBColorSpace;
	size_t				totalSize=bytesPerRow * ((bottom-top)+1);
	int*				dBits=(int*)malloc(totalSize);
	*tempMemory = dBits;

	copyImage2To32(srcBits, dBits, srcPixelWidth, height, left, top, right, bottom,colorMap);


	CHECKANDRETURN(AUTORELEASEOBJ([[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(unsigned char **)&dBits
														   pixelsWide:right-left
														   pixelsHigh:bottom-top
														bitsPerSample:bitsPerSample
													  samplesPerPixel:samplesPerPixel
															 hasAlpha:NO
															 isPlanar:NO
													   colorSpaceName:colorSpace
														  bytesPerRow:bytesPerRow
														 bitsPerPixel:bitsPerPixel]));
}//extractPixels_2_to_32;

-(CGImageRef)extractPixels_4_to_32:(void*)srcBits
					 srcPixelWidth:(int)srcPixelWidth height: (int) height
							  left:(int)left right:(int)right
							   top:(int)top bottom:(int)bottom
						  colorMap: (unsigned int *) colorMap
						tempMemory: (void **) tempMemory
{
	int                 bitsPerSample=8;
	int                 samplesPerPixel=3;
	int                 bitsPerPixel=32;
	int                 bytesPerRow=bytesPerLine(srcPixelWidth, 32);
	NSString*			colorSpace=NSDeviceRGBColorSpace;
	size_t				totalSize=bytesPerRow * ((bottom-top)+1);
	int*				dBits=(int*)malloc(totalSize);
	*tempMemory = dBits;

	copyImage4To32(srcBits, dBits, srcPixelWidth, height, left, top, right, bottom,colorMap);


	CHECKANDRETURN(AUTORELEASEOBJ([[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(unsigned char **)&dBits
														   pixelsWide:right-left
														   pixelsHigh:bottom-top
														bitsPerSample:bitsPerSample
													  samplesPerPixel:samplesPerPixel
															 hasAlpha:NO
															 isPlanar:NO
													   colorSpaceName:colorSpace
														  bytesPerRow:bytesPerRow
														 bitsPerPixel:bitsPerPixel]));
}//extractPixels_4_to_32;


-(CGImageRef)extractPixels_8_to_32:(void*)srcBits
					 srcPixelWidth:(int)srcPixelWidth height: (int) height
			left:(int)left right:(int)right
			top:(int)top bottom:(int)bottom
						  colorMap: (unsigned int *) colorMap
						tempMemory: (void **) tempMemory
	{
		int                 bitsPerSample=8;
		int                 samplesPerPixel=3;
		int                 bitsPerPixel=32;
		int                 bytesPerRow=bytesPerLine(srcPixelWidth, 32);
		NSString*			colorSpace=NSDeviceRGBColorSpace;
		size_t				totalSize=bytesPerRow * ((bottom-top)+1);
		int*				dBits=(int*)malloc(totalSize);
		*tempMemory = dBits;

		copyImage8To32(srcBits, dBits, srcPixelWidth, height, left, top, right, bottom,colorMap);


		CHECKANDRETURN(AUTORELEASEOBJ([[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(unsigned char **)&dBits
				pixelsWide:right-left
				pixelsHigh:bottom-top
				bitsPerSample:bitsPerSample
				samplesPerPixel:samplesPerPixel
				hasAlpha:NO
				isPlanar:NO
				colorSpaceName:colorSpace
				bytesPerRow:bytesPerRow
				bitsPerPixel:bitsPerPixel]));
	}//extractPixels_8_to_32;


-(CGImageRef)extractPixels_16_to_32:(void*)srcBits
					  srcPixelWidth:(int)srcPixelWidth height: (int) height
							   left:(int)left right:(int)right
								top:(int)top bottom:(int)bottom
						 tempMemory: (void **) tempMemory
{
	int                 bitsPerSample=8;
	int                 samplesPerPixel=3;
	int                 bitsPerPixel=32;
	int                 bytesPerRow=bytesPerLine(srcPixelWidth, 32);
	NSString*			colorSpace=NSDeviceRGBColorSpace;
	size_t				totalSize=bytesPerRow * ((bottom-top)+1);
	int*				dBits=(int*)malloc(totalSize);
	*tempMemory = dBits;

	copyImage16To32(srcBits, dBits, srcPixelWidth, height, left, top, right, bottom);

	CHECKANDRETURN(AUTORELEASEOBJ([[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(unsigned char **)&dBits
														   pixelsWide:right-left
														   pixelsHigh:bottom-top
														bitsPerSample:bitsPerSample
													  samplesPerPixel:samplesPerPixel
															 hasAlpha:NO
															 isPlanar:NO
													   colorSpaceName:colorSpace
														  bytesPerRow:bytesPerRow
														 bitsPerPixel:bitsPerPixel]));
}//extractPixels_16_to_32;

-(CGImageRef)computeBitmapFromBitsIndex:(void*)srcBits
                                                        width:(int)width height:(int)height depth:(int)depth
                                                         left:(int)left right:(int)right
                                                          top:(int)top bottom:(int)bottom
															tempMemory: (void**) tempMemory
{
    /*
     It seems that NSImage will replace its NSBitmapImageRep with a
     NXCachedImageRep of the whole bitmap upon receiving a
     composite:fromRect:toPoint: message.
     Therefore:
              1- it is useless to keep our own "cached" image;
              2- we may as well copy to the right format & depth the affected
     rectangle in a temporary bitmap, and draw it directly.
     */
    debug(int dummy=fprintf(stderr,"%s\n",sel_getName(_cmd));\
          int fummy=fflush(stderr);)
    switch(depth){
        case 1:
  			return([self extractPixels_1_to_32:srcBits srcPixelWidth:width height: height
                                          left:left right:right top:top bottom:bottom
									  colorMap: colorMap32
									tempMemory: tempMemory]);
            break;
        case 2:
   			return([self extractPixels_2_to_32:srcBits srcPixelWidth:width height: height
                                          left:left right:right top:top bottom:bottom
									  colorMap: colorMap32
									tempMemory: tempMemory]);
			break;
		case 4:
  			return([self extractPixels_4_to_32:srcBits srcPixelWidth:width height: height
                                          left:left right:right top:top bottom:bottom
									  colorMap: colorMap32
									tempMemory: tempMemory]);
			break;
        case 8:
			return([self extractPixels_8_to_32:srcBits srcPixelWidth:width height: height
                                          left:left right:right top:top bottom:bottom
									  colorMap: colorMap32
									tempMemory: tempMemory]);
			break;
        case 16:
            return([self extractPixels_16_to_32:srcBits srcPixelWidth:width height: height
										   left:left right:right top:top bottom:bottom
									 tempMemory: tempMemory]);
			break;
        default:
            switch(NSRunAlertPanel(@"Error",
                                   @"Cocoa user interface for Squeak doesn't support image depth of %d.\n",
                                   @"Continue",@"Quit",NULL,depth)){
                case NSAlertDefaultReturn:
                    return(0);
                case NSAlertAlternateReturn:
                    ioExit();
                    return(0);
                case NSAlertErrorReturn:
                default:
                    NSLog(@"Cocoa user interface for Squeak doesn't support image depth of %d.\n",depth);
                    ioExit();
                    return(0);
                    }
            }
}

@end

id<MTLDevice>
getMainWindowMetalDevice(void) {
	return mainMetalView ? mainMetalView.device : nil;
}

id<MTLCommandQueue>
getMainWindowMetalCommandQueue(void) {
	return mainMetalView ? mainMetalView.graphicsCommandQueue : nil;	
}

unsigned int
createMetalTextureLayerHandle(void) {
	return mainMetalView ? [ mainMetalView createTextureLayerHandle ] : 0;
}

void
destroyMetalTextureLayerHandle(unsigned int handle) {
	if(!mainMetalView)
	 	return;

	[ mainMetalView destroyTextureLayerHandle: handle ];
}

void
setMetalTextureLayerContent(unsigned int handle, id<MTLTexture> texture, int x, int y, int w, int h) {
	if(!mainMetalView)
	 	return;

	[ mainMetalView setExtraLayer: handle texture: texture x: x y: y w: w h: h];
}

#endif // USE_METAL
