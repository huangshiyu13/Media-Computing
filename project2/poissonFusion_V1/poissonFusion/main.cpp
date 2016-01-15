#include <vector>

#include "BakImage.h"
#include "tool.h"

using namespace std;

int main() {

	//������ģʽ����ѡ��
	//selectModel();

	if (::MessageBoxA(NULL, "��ѡ��һ�ű���ȡ��ͼƬ", "����", 0x00000001L) == 1){
		char *cutImageFile = openFile();
		if (cutImageFile == NULL) return 0;

		if (::MessageBoxA(NULL, "��ѡ��һ�ű���ͼƬ", "����", 0x00000001L) != 1){
			return 0;
		}
		char *bakImageFile = openFile();
		if (bakImageFile == NULL) return 0;

		cutImage = new CutImage(cutImageFile, "cutImageWindow");
		bakImage = new BakImage(bakImageFile, "backgroundImageWindow");
		
		//��ʼ��
		cutImage->init();

		//��ʾ����
		cutImage->createWindow();
		bakImage->createWindow();

		//��������¼�
		cvSetMouseCallback(cutImage->winName.c_str(), cutImageMouseEvent, NULL);
		cvSetMouseCallback(bakImage->winName.c_str(), bakImageMouseEvent, NULL);

		while(cvWaitKey(10) != 27);//��ESC�˳�
	}
	return 0;
}

