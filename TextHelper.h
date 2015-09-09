#pragma once
#include <cstdio>
#include <string>

const float charW = 8.0f;

void readBitmapFont(FILE* file);
void drawChar(float x, float y, char text);
void drawString(float x, float y, const char* text);
void drawString2(float x, float y, std::string);

