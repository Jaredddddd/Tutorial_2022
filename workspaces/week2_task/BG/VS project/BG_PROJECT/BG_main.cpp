#include "BG_H.h"
int main()
{
    Recovery test;
    string filePath;
    cout << "��������Ƶ���Ե�ַ:" << endl;
    cout << "���磺D:/Github/Tutorial_2022/resource/video0.mp4" << endl;
    cin >> filePath;
    test.readVideo();
    test.process(filePath);
    test.showResult();
    return 0;
}