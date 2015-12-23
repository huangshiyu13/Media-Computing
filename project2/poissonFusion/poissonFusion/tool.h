#ifndef TOOL_H
#define TOOL_H
#include "define.h"
#include <windows.h>

char* openFile() {
	char szFileName[MAX_PATH] = {0};
	OPENFILENAME openFileName = {0};
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = "ͼƬ�ļ�(*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.bmp\0�����ļ�(*.*)\0*.*\0\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!::GetOpenFileName(&openFileName)) return NULL;
	char *fileName = new char[999];
	strcpy(fileName, openFileName.lpstrFile);
	return fileName;
}

#endif