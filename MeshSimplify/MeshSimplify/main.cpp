#include "MeshSimplify.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
	string in, out;
	in = "./objFile/bunny.obj";
	out = "./objFile/output.obj";
	MeshSimplify ms;
	cout << "���ڶ�ȡ�ļ�..." << endl;
	ms.readFile(in);
	cout << "�ļ���ȡ�ɹ�" << endl;
	cout << "���ڼ򻯴���..." << endl;
	ms.simplify(0.01);
	cout << "�򻯴���ɹ�" << endl;
	cout << "����д���ļ�..." << endl;
	ms.writeFile(out);
	cout << "�ļ�д��ɹ�" << endl;
	return 0;
}