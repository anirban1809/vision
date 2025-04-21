#import <Cocoa/Cocoa.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#import <QuartzCore/QuartzCore.h>
#include <GLFW/glfw3native.h>

@interface DraggableContentView : NSView
@end

@implementation DraggableContentView
- (void)mouseDown:(NSEvent *)event {
  NSWindow *window = [self window];
  NSPoint initialLocation = [event locationInWindow];

  while (true) {
    NSEvent *nextEvent =
        [window nextEventMatchingMask:NSEventMaskLeftMouseDragged |
                                      NSEventMaskLeftMouseUp];
    if ([nextEvent type] == NSEventTypeLeftMouseUp)
      break;

    NSPoint currentLocation = [nextEvent locationInWindow];
    NSPoint delta;
    delta.x = currentLocation.x - initialLocation.x;
    delta.y = currentLocation.y - initialLocation.y;

    NSRect frame = [window frame];
    frame.origin.x += delta.x;
    frame.origin.y += delta.y;
    [window setFrame:frame display:YES];
  }
}
@end

void ApplyRoundedCorners(NSView *view, CGFloat radius) {
  view.wantsLayer = YES;

  // Create a mask layer with rounded corners
  CAShapeLayer *maskLayer = [CAShapeLayer layer];
  NSRect bounds = view.bounds;
  CGPathRef path = CGPathCreateWithRoundedRect(
      CGRectMake(0, 0, bounds.size.width, bounds.size.height), radius, radius,
      NULL);
  maskLayer.path = path;
  CGPathRelease(path);

  view.layer.mask = maskLayer;
}

void MakeWindowBorderless(GLFWwindow *window) {
  if (!window)
    return;

  NSWindow *nsWindow = glfwGetCocoaWindow(window);
  if (!nsWindow)
    return;

  // Set window style
  NSWindowStyleMask mask =
      NSWindowStyleMaskBorderless | NSWindowStyleMaskResizable;
  [nsWindow setStyleMask:mask];
  [nsWindow setOpaque:NO];
  [nsWindow setBackgroundColor:[NSColor clearColor]];
  [nsWindow setHasShadow:YES];

  // Replace content view with draggable one
  NSView *contentView = [nsWindow contentView];
  NSRect frame = contentView.frame;
  DraggableContentView *dragView =
      [[DraggableContentView alloc] initWithFrame:frame];
  [dragView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
  [dragView setWantsLayer:YES];
  [dragView setLayer:contentView.layer];
  [nsWindow setContentView:dragView];

  // Apply rounded mask
  ApplyRoundedCorners(dragView, 10.0);

  // Update mask on resize
  [[NSNotificationCenter defaultCenter]
      addObserverForName:NSViewFrameDidChangeNotification
                  object:dragView
                   queue:nil
              usingBlock:^(NSNotification *note) {
                ApplyRoundedCorners(dragView, 10.0);
              }];
}
