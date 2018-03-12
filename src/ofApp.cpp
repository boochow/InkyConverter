#include "ofApp.h"
#include "FreeImage.h"

#define WIN_W (800)
#define WIN_H (600)
#define VIEW_W (560)
#define CROP_W (212)
#define CROP_H (104)
#define GUI_H (VIEW_W + (WIN_W - VIEW_W - CROP_W)/2)
#define FOCUS_Y (152)
#define CROPIMG_Y (FOCUS_Y + 276)

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowShape(WIN_W, WIN_H);
	ofBackground(54, 54, 54, 255);

	ofTrueTypeFont::setGlobalDpi(72);
	font.load("VeraSeBd.ttf", 24, true, true);

	offset = ofPoint(0, 0);
	rotateIndex = 0;
	ditherType = ATKINSON;


	imageGui.setup("Image","settings-image.xml");

	imageGui.setPosition(GUI_H, 8);
	
	openFile.addListener(this, &ofApp::openFileClicked);
	imageGui.add(openFile.setup("Open File..."));
	
	imgContrast.addListener(this, &ofApp::imgContrastChanged);
	imgBrightness.addListener(this, &ofApp::imgBrightnessChanged);
	imgRedLevel.addListener(this, &ofApp::imgRedLevelChanged);
	imageGui.add(imgContrast.set("Contrast", 1.0, 0.5, 2.0));
	imageGui.add(imgBrightness.set("Brightness", 0, -128, 128));
	imageGui.add(imgRedLevel.set("Red Level", 0, -32, 32));

	scale.addListener(this, &ofApp::scaleChanged);
	imageGui.add(scale.set("Scale", 1.0, 0.125, 2));

	rotateImage.addListener(this, &ofApp::rotateImageClicked);
	imageGui.add(rotateImage.setup("Rotate"));

	focusGui.setup("Crop image","settings-focus.xml");
	focusGui.setPosition(GUI_H, FOCUS_Y + CROP_H + 20);

	focusWidth.addListener(this, &ofApp::focusSizeChanged);
	focusHeight.addListener(this, &ofApp::focusSizeChanged);
	focusGui.add(focusWidth.set("Width", CROP_W, 8, CROP_W));
	focusGui.add(focusHeight.set("Height", CROP_H, 8, CROP_H));

	ditherAtkinson.addListener(this, &ofApp::ditherAtkinsonChanged);
	ditherFloyd.addListener(this, &ofApp::ditherFloydChanged);
	ditherBayer.addListener(this, &ofApp::ditherBayerChanged);
	ditherSelect.setName("Dithering Method");
	ditherSelect.add(ditherBayer.set("(1)Bayer", true));
	ditherSelect.add(ditherFloyd.set("(2)Floyd-Steinberg", true));
	ditherSelect.add(ditherAtkinson.set("(3)Atkinson", true));

	focusGui.add(ditherSelect);


	captureGui.setup("Save as...", "settings-capture.xml");
	captureGui.setPosition(GUI_H, CROPIMG_Y + CROP_H + 20);
	saveImage.addListener(this, &ofApp::saveImageToFile);
	captureGui.add(saveImage.setup("Save as..."));
}
void ofApp::openFileClicked()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a jpg or png or bmp");
	if (openFileResult.bSuccess) {
		processOpenFileSelection(openFileResult);
	}
}
//--------------------------------------------------------------
void ofApp::imgBrightnessChanged(int & v)
{
	updateImages(IMG_BRIGHTNESS);
}

void ofApp::imgContrastChanged(float & v)
{
	updateImages(IMG_CONTRAST);
}

void ofApp::imgRedLevelChanged(int & v)
{
	updateImages(IMG_REDLEVEL);
}

void ofApp::rotateImageClicked()
{
	rotateIndex = (rotateIndex + 3) % 4;
	updateImages(IMG_ROTATE);
}

//--------------------------------------------------------------
void ofApp::focusSizeChanged(int &v) {
	int height = (focusHeight + 4) / 8;
	focusHeight = height * 8;
	focus.setSize(focusWidth, focusHeight);
}

void ofApp::scaleChanged(float &v) {
	updateImages(IMG_SCALE);
}

void ofApp::setDitherType(int d)
{
	ditherAtkinson.setWithoutEventNotifications(d == ATKINSON);
	ditherBayer.setWithoutEventNotifications(d == ORDERED_BAYER);
	ditherFloyd.setWithoutEventNotifications(d == FLOYD_STEINBERG);
	ditherType = d;
	processDither();
}

void ofApp::ditherAtkinsonChanged(bool & v)
{
	setDitherType(ATKINSON);
}

void ofApp::ditherBayerChanged(bool &v) {
	setDitherType(ORDERED_BAYER);
}

void ofApp::ditherFloydChanged(bool &v) {
	setDitherType(FLOYD_STEINBERG);
}

//--------------------------------------------------------------
void ofApp::update(){

}
//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(80, 80, 80);
	ofRect(0, 0, VIEW_W, WIN_H);
	if (img.isAllocated()) {
		ofRectangle bounds = ofRectangle(0, 0, img.getWidth(), img.getHeight());
		ofRectangle sub = ofRectangle(0, 0, VIEW_W, WIN_H);
		sub.translate(offset);
		sub = sub.getIntersection(bounds);

		ofSetColor(255, 255, 255);
		if (mouseX < VIEW_W) {
			img.drawSubsection(sub.x - offset.x, sub.y - offset.y, sub.getWidth(), sub.getHeight(), sub.x, sub.y);
		}
		else {
			bwr_img.drawSubsection(sub.x - offset.x, sub.y - offset.y, sub.getWidth(), sub.getHeight(), sub.x, sub.y);
		}
		if (bwr_img.isAllocated()) {
			ofRectangle bounds = ofRectangle(0, 0, bwr_img.getWidth(), bwr_img.getHeight());
			ofRectangle sub = ofRectangle(0, 0, VIEW_W, WIN_H);
			sub.translate(offset);
			sub = sub.getIntersection(focus + offset);
			sub = sub.getIntersection(bounds);
			bwr_img.drawSubsection(sub.x - offset.x, sub.y - offset.y, sub.getWidth(), sub.getHeight(), sub.x, sub.y);
			bwr_img.drawSubsection(GUI_H, FOCUS_Y, sub.width, sub.height, sub.x, sub.y);
		}
	}
	else {
		ofSetColor(144, 144, 144);
		font.drawString("Hit SPACE to select an image", 40, 200);
	}
	if (capture.isAllocated()) {
		capture.draw(GUI_H, CROPIMG_Y);
	}
	imageGui.draw();
	focusGui.draw();
	captureGui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case ' ':
		openFileClicked();
		break;
	case '1':
		ditherBayer = true;
		break;
	case '2':
		ditherFloyd = true;
		break;
	case '3':
		ditherAtkinson = true;
		break;
	case '-':
	{
		float s = round((scale + 0.0624) / 0.125) * 0.125;
		scale = (s > 0.25) ? (s - 0.125) : 0.125;
		updateImages(IMG_SCALE);
	}
		break;
	case '+':
	{
		float s = round((scale + 0.0624) / 0.125) * 0.125;
		scale = (s < 2) ? (s + 0.125) : 2;
		updateImages(IMG_SCALE);
	}
		break;
	case OF_KEY_RIGHT:
		offset.x = offset.x + 1;
		break;
	case OF_KEY_LEFT:
		offset.x = offset.x - 1;
		break;
	case OF_KEY_DOWN:
		offset.y = offset.y + 1;
		break;
	case OF_KEY_UP:
		offset.y = offset.y - 1;
		break;
	case OF_KEY_RETURN:
		processCapture();
		break;
	}
	if (img.isAllocated())
		processDither();
}

//--------------------------------------------------------------
void ofApp::updateImages(int changedValue) {
	if (original.isAllocated()) {
		if (changedValue == IMG_ORIGINAL || changedValue == IMG_SCALE || changedValue == IMG_ROTATE) {
			scaled.clone(original);
			scaled.resize(scaled.getWidth() * scale, scaled.getHeight() * scale);
			scaled.rotate90(rotateIndex);
		}
	}
	if (scaled.isAllocated()) {
		img.clone(scaled);
		ofPixels p = img.getPixels();
		int br = 128 + imgBrightness;
		int s = img.getWidth() * img.getHeight() * p.getBytesPerPixel();
		for (int i = 0; i < s; i++) {
			int c = p[i] - 128;
			c = imgContrast * c + br;
			c = c < 0 ? 0 : c;
			p[i] = c > 255 ? 255 : c;
		}
		img.setFromPixels(p);

		grey_img.clone(img);
		grey_img.setImageType(OF_IMAGE_GRAYSCALE);

		red_img.clone(img);
		ofPixels red = red_img.getPixels();
		ofPixels r = red.getChannel(0);
		ofPixels g = red.getChannel(1);
		ofPixels b = red.getChannel(2);
		s = img.getWidth() * img.getHeight();
		for (int i = 0; i < s; i++) {
			int t = 0.7 * r[i];
			if ((r[i] < (128 - imgRedLevel)) || (t < g[i]) || (t < b[i]) || ((r[i] - min(g[i], b[i])) < (64 - imgRedLevel))) {
				r[i] = 0;
			} else {
				r[i] = min(255.0, (r[i] - min(g[i], b[i]) - 64 + imgRedLevel) * 1.33);
			}
			g[i] = 0;
			b[i] = 0;
		}
		red_img.setFromPixels(r);

		processDither();
	}
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {

	ofFile file(openFileResult.getPath());

	if (file.exists()) {
		string fileExtension = ofToUpper(file.getExtension());
		if (fileExtension == "JPG" || fileExtension == "PNG" || fileExtension == "BMP") {
			original.load(openFileResult.getPath());
			imgContrast = 1.0;
			imgBrightness = 0;
			imgRedLevel = 0;
			rotateIndex = 0;
			scale = 1.0;
			offset.set(0, 0);
			updateImages(IMG_ORIGINAL);
		}
	}
}

//--------------------------------------------------------------
void ofApp::processDither() {
	if (grey_img.isAllocated()) {
		switch (ditherType)
		{
		case ORDERED_BAYER:

			dither.dither_ordered(grey_img, bw_img, DITHER_ORDERED_8x8);
			break;

		case FLOYD_STEINBERG:

			dither.dither_floyd_steinberg(grey_img, bw_img);
			break;

		case ATKINSON:

			dither.dither_atkinson(grey_img, bw_img);
			break;
		}
	}
	if (red_img.isAllocated()) {
		switch (ditherType)
		{
		case ORDERED_BAYER:

			dither.dither_ordered(red_img, r_img, DITHER_ORDERED_8x8);
			break;

		case FLOYD_STEINBERG:

			dither.dither_floyd_steinberg(red_img, r_img);
			break;

		case ATKINSON:

			dither.dither_atkinson(red_img, r_img);
			break;
		}
	}
	if (img.isAllocated()) {
		bwr_img.clone(img);
		ofPixels p = bwr_img.getPixels();
		ofPixels mono = bw_img.getPixels();
		p.setChannel(0, mono);
		p.setChannel(1, mono);
		p.setChannel(2, mono);
		ofPixels r = p.getChannel(0);
		ofPixels g = p.getChannel(1);
		ofPixels b = p.getChannel(2);
		ofPixels red = r_img.getPixels();
		int s = bwr_img.getWidth() * bwr_img.getHeight();
		for (int i = 0; i < s; i++) {
			if (red[i] != 0) {
				r[i] = 255;
				g[i] = 0;
				b[i] = 0;
			}
		}
		p.setChannel(0, r);
		p.setChannel(1, g);
		p.setChannel(2, b);
		bwr_img.setFromPixels(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	focus.setPosition(x-focus.width / 2, y-focus.height / 2);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if ((x < VIEW_W) && (y < WIN_H)) {
		offset = clickPos - ofPoint(x, y) + offset;
		clickPos.set(x, y);
		focus.setPosition(x - focus.width / 2, y - focus.height / 2);
	}
}

//--------------------------------------------------------------

void ofApp::processCapture() {
	if (bw_img.isAllocated()) {
		capture.clear();
		ofRectangle bounds = ofRectangle(0, 0, bwr_img.getWidth(), bwr_img.getHeight());
		ofRectangle sub = ofRectangle(0, 0, VIEW_W, WIN_H);
		sub.translate(offset);
		sub = sub.getIntersection(focus + offset);
		sub = sub.getIntersection(bounds);
		capture.cropFrom(bwr_img, sub.x, sub.y, sub.width, sub.height);
	}
}

//--------------------------------------------------------------

void ofApp::mousePressed(int x, int y, int button){
	if ((x < VIEW_W) && (y < WIN_H)) {
		switch (button)
		{
		case 0:
			clickPos = ofPoint(x, y);
			processCapture();
			break;
		default:
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void ofApp::copyToClipboard(const string &text) {
	ofGetWindowPtr()->setClipboardString(text);
}

void ofApp::saveImageToFile()
{
	ofFileDialogResult saveFileResult = ofSystemSaveDialog("Untitled.png", "Save");
	if (saveFileResult.bSuccess) {
		int width = capture.getWidth();
		int height = capture.getHeight();
		ofPixels pix = capture.getPixelsRef();
		FIBITMAP* bmp = FreeImage_Allocate(width, height, pix.getBitsPerPixel());
		if (bmp != nullptr) {
			int srcStride = width * pix.getBytesPerPixel();
			int dstStride = FreeImage_GetPitch(bmp);
			unsigned char* src = (unsigned char*)(pix.getData());
			unsigned char* dst = FreeImage_GetBits(bmp);
			if (srcStride != dstStride) {
				for (int i = 0; i < (int)height; i++) {
					memcpy(dst, src, srcStride);
					src += srcStride;
					dst += dstStride;
				}
			}
			else {
				memcpy(dst, src, dstStride*height);
			}
			FreeImage_FlipVertical(bmp);

			FIBITMAP *tmp;
			tmp = FreeImage_ColorQuantize(bmp, FIQ_LFPQUANT);
			if (tmp != nullptr) {
				RGBQUAD *palette = FreeImage_GetPalette(tmp);

				// move red color palette to palette no.2
				// since FreeImage pixel is bgr format, we change palette ff0000 to 0000ff. 
				BYTE c;
				BYTE swap;
				RGBQUAD savec;
				
				swap = 2;
				c = 0;
				for (int i = 0; i < 256; i++) {
					if (palette[i].rgbBlue >= 0xFE && palette[i].rgbGreen == 0x00 && palette[i].rgbRed == 0x00) {
						palette[i].rgbRed = palette[i].rgbBlue;
						palette[i].rgbBlue = 0;
						if (i != 2) {
							c = i;
							FreeImage_SwapPaletteIndices(tmp, &c, &swap);
							savec.rgbBlue = palette[i].rgbBlue;
							savec.rgbGreen = palette[i].rgbGreen;
							savec.rgbRed = palette[i].rgbRed;
							palette[i].rgbBlue = palette[2].rgbBlue;
							palette[i].rgbGreen = palette[2].rgbGreen;
							palette[i].rgbRed = palette[2].rgbRed;
							palette[2].rgbBlue = savec.rgbBlue;
							palette[2].rgbGreen = savec.rgbGreen;
							palette[2].rgbRed = savec.rgbRed;
						}
						break;
					}
				}

				// move white color palette to palette no.0
				swap = 0;
				for (int i = 0; i < 256; i++) {
					if (palette[i].rgbRed >= 0xFE && palette[i].rgbBlue >= 0xFE && palette[i].rgbGreen >= 0xFE) {
						if (i != 0) {
							c = i;
							FreeImage_SwapPaletteIndices(tmp, &c, &swap);
							savec.rgbBlue = palette[i].rgbBlue;
							savec.rgbGreen = palette[i].rgbGreen;
							savec.rgbRed = palette[i].rgbRed;
							palette[i].rgbBlue = palette[0].rgbBlue;
							palette[i].rgbGreen = palette[0].rgbGreen;
							palette[i].rgbRed = palette[0].rgbRed;
							palette[0].rgbBlue = savec.rgbBlue;
							palette[0].rgbGreen = savec.rgbGreen;
							palette[0].rgbRed = savec.rgbRed;
						}
						break;
					}
				}

				FreeImage_Save(FIF_PNG, tmp, saveFileResult.getPath().c_str(), PNG_DEFAULT);
				FreeImage_Unload(bmp);
				FreeImage_Unload(tmp);
			}
		}

//		capture.saveImage(saveFileResult.getPath(), OF_IMAGE_QUALITY_BEST);
	}
}
