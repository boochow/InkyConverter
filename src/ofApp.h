#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDither.h"

enum
{
	ORDERED_BAYER,
	FLOYD_STEINBERG,
	ATKINSON
};

enum
{
	IMG_ORIGINAL,
	IMG_SCALE,
	IMG_BRIGHTNESS,
	IMG_CONTRAST,
	IMG_REDLEVEL,
	IMG_ROTATE
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void processOpenFileSelection(ofFileDialogResult openFileResult);
		void processDither();
		void processCapture();
		void updateImages(int changedValue);
		void copyToClipboard(const string &text);
		ofImage original;
		ofImage scaled;
		ofImage img;
		ofImage grey_img;
		ofImage bw_img;
		ofImage red_img;
		ofImage r_img;
		ofImage bwr_img;
		ofPoint offset;
		int rotateIndex;
		ofRectangle focus; // bw image rectangle area to be drawn
		ofImage capture; // current selected bw image
		ofxDither	dither;
		int			ditherType;
		ofPoint clickPos; // view rectangle's offset from image origin point (0,0)

		ofTrueTypeFont font;


		ofxPanel imageGui;
		ofxButton openFile;
		void openFileClicked();
		ofParameter<int> imgBrightness;
		ofParameter<float> imgContrast;
		ofParameter<int> imgRedLevel;
		void imgBrightnessChanged(int &v);
		void imgContrastChanged(float &v);
		void imgRedLevelChanged(int &v);
		ofxButton rotateImage;
		void rotateImageClicked();

		ofxPanel focusGui;
		ofParameterGroup ditherSelect;
		
		ofParameter<int> focusWidth;
		ofParameter<int> focusHeight;
		ofParameter<float> scale;
		void focusSizeChanged(int & v);
		void scaleChanged(float & v);

		ofParameter<bool> ditherBayer;
		ofParameter<bool> ditherFloyd;
		ofParameter<bool> ditherAtkinson;
		void setDitherType(int d);
		void ditherBayerChanged(bool &v);
		void ditherFloydChanged(bool &v);
		void ditherAtkinsonChanged(bool &v);

		ofxPanel captureGui;
		ofxButton saveImage;
		void saveImageToFile();
};
