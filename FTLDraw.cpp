#include "FTLDraw.h"
#include "DirtyHooker.h"
#include <windows.h>
#include <gl\GL.h>
#include <vector>


//vector of functions that take no arguments and return nothing
std::vector<std::function<void(void)>> hooks;

void drawTriangle (float x1,float y1,float x2,float y2,float x3,float y3) {
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_TRIANGLES);
	 glVertex3f(x1, y1, 0.0);
	 glVertex3f(x2, y2, 0.0);
	 glVertex3f(x3, y3, 0.0);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

void setColor(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void drawRect (float x,float y,float w,float h) {
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_QUADS);
	 glVertex3f(x, y, 0.0); 
	 glVertex3f(x+w, y, 0.0); 
	 glVertex3f(x+w, y+h, 0.0); 
	 glVertex3f(x, y+h, 0.0); 
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

//Don't inline or it'll mess up the hook's stack frame!
__declspec(noinline) void callHooks(void) {
	for(int i=0;i<hooks.size();i++) {
		try {
			std::function<void(void)> f = hooks[i];
			f();
		}
		catch (std::exception e) {
			MessageBox(NULL, e.what(), "Fail!", MB_OK + MB_ICONINFORMATION);
		}
	}
}

void openGLFinishHook(void) {
	callHooks();
	//call the original function
	glFinish();
};

void addDrawHook(std::function<void(void)> function) {
	hooks.push_back(function);
}

void hookGLFinish(void) {
	HANDLE FTLProcess = GetCurrentProcess();
	RETHook6Byte((0x0025DBB8+0x00400000),openGLFinishHook,FTLProcess);
}