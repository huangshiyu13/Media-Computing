#ifndef TOOL_H
#define TOOL_H
#include "define.h"
#include <windows.h>

void selectModel(){
	char ans;
	
	cout << "�Ƿ����òü��Ż�? (y/n)"<<endl;
	cin >> ans;
	if (ans == 'y'){
		cutOptimize = true;	
	}
	else{
		cutOptimize = false;
	}

	cout << "�Ƿ��ʱ? (y/n)"<<endl;
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