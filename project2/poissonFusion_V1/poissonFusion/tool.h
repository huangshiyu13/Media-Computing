#ifndef TOOL_H
#define TOOL_H
#include "define.h"
#include <windows.h>

void selectModel(){
	char ans;
	
	cout << "是否启用裁剪优化? (y/n)"<<endl;
	cin >> ans;
	if (ans == 'y'){
		cutOptimize = true;	
	}
	else{
		cutOptimize = false;
	}

	cout << "是否计时? (y/n)"<<endl;
	cin >> ans;
	if (ans == 'y'){
		timeTest = true;
	}
	else{
		timeTest = false;
	}
}

char* openFile() {
	char szFileName[MAX_PATH] = {0};
	OPENFILENAME openFileName = {0};
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = "图片文件(*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.bmp\0所有文件(*.*)\0*.*\0\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!::GetOpenFileName(&openFileName)) return NULL;
	char *fileName = new char[999];
	strcpy(fileName, openFileName.lpstrFile);
	return fileName;
}

#endif