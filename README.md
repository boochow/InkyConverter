# InkyConverter

## Description

InkyConverter is an image conversion tool for [pimoroni Inky pHAT software](https://learn.pimoroni.com/tutorial/sandyj/getting-started-with-inky-phat).

It generates 212 x 104 pixels black and white and red (bwr) bitmap data in 8 bits PNG format with special palette data (palette 0 = while, palette 1 = black, palette 2 = red).

JPG or PNG or BMP files can be used as an input.
The image is converted to black and white bitmap.(Thanks to [ofxDither](http://www.julapy.com/blog/2011/03/09/ofxdither/).)
Red blobs in the image is extracted and processed separately and combined with black and white bitmap.

## Instructions

Click "Open File..." button or hit space bar to open an image file.
The image is shown in the window.

Move mouse pointer over the image. You'll see a "focus area" around the mouse pointer. 
You can scroll the source image by dragging it or hitting arrow keys.

The image under focus area is automatically converted to bwr-image and copy of the focus area is always shown as a capture preview in the center-right area of the window.

Locate focus area to where you want to capture image, then click left button of mouse or hit enter key.
The image under focus area is captured and shown in the bottom right of the window.

Click "Save as..." button to save the captured image as a PNG file.

## Adjusting image tone

There are several control sliders / buttons for adjusting image parameters.
- Contrast, Brightness: affects black and white pixels ratio
- Red Level: changes the intensity of red color
- Scale, Rotate: resize and rotate the image
- Width, Height: changes the image size to be captured. This can be useful if you need icon images for your python script.
- Dithering method: alternate methods for converting grayscale image to black-and-white image

## How to build

InkyConverter is developed on [openFrameworks](http://openframeworks.cc/).

You need to download and install openFrameworks to build InkyConverter.

Place InkyConverter source code folder into ```of_v0.9.x\apps\myApps``` folder.

Although openFrameworks is a cross-platform toolkit, I have tested only on Windows 7 + VisualStudio 2015.
