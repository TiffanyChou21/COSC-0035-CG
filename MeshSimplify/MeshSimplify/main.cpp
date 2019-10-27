#include "MeshSimplify.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
	string in, out;
	in = "./objFile/bunny.obj";
	out = "./objFile/output.obj";
	MeshSimplify ms;
	cout << "正在读取文件..." << endl;
	ms.readFile(in);
	cout << "文件读取成功" << endl;
	cout << "正在简化处理..." << endl;
	ms.simplify(0.01);
	cout << "简化处理成功" << endl;
	cout << "正在写入文件..." << endl;
	ms.writeFile(out);
	cout << "文件写入成功" << endl;
	return 0;
}