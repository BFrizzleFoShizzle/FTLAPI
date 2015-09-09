#include "TextHelper.h"
#include <windows.h>
#include <gl\GL.h>

GLuint texFont;
GLubyte texFontData[128][128][4];

void drawString2(float x, float y, std::string text) {
	drawString(x, y, text.c_str());
}

void drawString(float x, float y, const char* text){
	// set up for text drawing
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texFont);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, texFontData);
	while(*text != 0) {
		drawChar(x, y, *text);
		x+=charW;
		text++;
	}
};


void drawChar(float x, float y, char text) {
	//For some reason v seems to be rounded down, so
	//we add an extra pixel to the v coord to fix this
	float pxl = 1.0/128;
	// width/height of a char in texture space
	float charSize = 1.0f/16;
	// t (y) texture coord of char
	float charT = ((float)(text%16))*charSize;
	// s (x) texture coord of char
	float charS = ((float)(text/16))*charSize;
	
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	 glTexCoord2f(charS, charT+pxl);
	 glVertex3f(x, y, 0.0); 
	 glTexCoord2f(charS+charSize, charT+pxl);
	 glVertex3f(x+charW, y, 0.0); 
	 glTexCoord2f(charS+charSize, charT+charSize+pxl);
	 glVertex3f(x+charW, y+charW, 0.0); 
	 glTexCoord2f(charS, charT+charSize+pxl);
	 glVertex3f(x, y+charW, 0.0); 
	glEnd();
	glDisable(GL_TEXTURE_2D);
};

void readBitmapFont(FILE* fp) {
	//generate texture for font, if needed
	if(!glIsTexture(texFont))
		glGenTextures(1, &texFont);
	//read up to the image data offset
	for(int i=0;i<0xA;i++) {
		fgetc(fp);
	}
	int dataOffset = 0;
	fread(&dataOffset, 4, 1, fp);
	//discard everything but the raw image data
	for(int i=0;i<dataOffset-0xE;i++) {
		fgetc(fp);
	}
	//read in image
	//(128*128)/8 = amount of raw data
	for(int i=0;i<(128*128)/8;i++) {
		int currByte = fgetc(fp);
		for(int currBit=0;currBit<8;currBit++) {
			int x = ((i*8)+currBit)%128;
			//fix
			int y = 128-(((i*8)+currBit)/128);
			//I'm not american, shocking!
			int colour = ((currByte>>(7-currBit))&1)*255;
			texFontData[y][x][0] = colour;
			texFontData[y][x][1] = colour;
			texFontData[y][x][2] = colour;
			texFontData[y][x][3] = colour;
		}
	}
};
