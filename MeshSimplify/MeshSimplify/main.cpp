#include "MeshSimplify.h"
#include <iostream>
#include <string>
using namespace std;

MeshSimplify ms;

float scale = 1.0;
float xRotate = 0.0;
float yRotate = 0.0;
float xMove = 0.0;
float yMove = 0.0;

bool left_mouse_status = false;
bool right_mouse_status = false;
GLfloat Oldx = 0.0;
GLfloat Oldy = 0.0;
GLfloat OldMovex = 0.0;
GLfloat OldMovey = 0.0;

bool showFace = false;
bool showWires = true;

void Init();
void display();
void myMouse(int button, int status, int x, int y);
void myKeyboard(unsigned char key, int x, int y);
void onMouseMove(int x, int y);

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	// 输入输出文件
	string in, out;
	in = "./objFile/dragon.obj";
	out = "./objFile/output.obj";
	// 直接显示结果
	//ms.readFile(in);
	////ms.delUselessLine();
	//scale = 1/ms.maxLen * 0.75;
	//Init();
	//glutMainLoop();
	// 读取文件
	cout << "正在读取文件..." << endl;
	ms.readFile(in);
	cout << "文件读取成功" << endl;
	// 简化处理
	cout << "正在简化处理..." << endl;
	ms.simplify(0.01);
	cout << "简化处理成功" << endl;
	// 写入磁盘
	cout << "正在写入文件..." << endl;
	ms.writeFile(out);
	cout << "文件写入成功" << endl;
	// 显示效果
	cout << "是否显示简化图形(y/n)" << endl;
	string s;
	cin >> s;
	if (s == "y" || s == "yes") {
		ms.readFile(out);
		//ms.delUselessLine();
		Init();
		glutMainLoop();
	}
	return 0;
}

void Init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutCreateWindow("ObjLoader");

	glutDisplayFunc(display);
	glutMouseFunc(myMouse);
	glutMotionFunc(onMouseMove);
	glutKeyboardFunc(myKeyboard);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glClearDepth(2.0);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
void display() {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(xMove, yMove, 0.0);
	glScalef(scale, scale, scale);
	//cout << scale << endl;
	glRotatef(xRotate, 1.0, 0.0, 0.0);
	glRotatef(yRotate, 0.0, 1.0, 0.0);

	glEnable(GL_CULL_FACE);
	//绘制obj模型
	if (showFace) {
		ms.drawFace();
	}
	if (showWires) {
		ms.drawLine();
	}
	glFlush();
	glutSwapBuffers();
}
void myMouse(int button, int status, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN) {
		left_mouse_status = true;
		Oldx = x;
		Oldy = y;
		//cout << x << ":" << y << endl;
	}
	if (button == GLUT_LEFT_BUTTON && status == GLUT_UP) {
		left_mouse_status = false;
	}
	if (button == GLUT_RIGHT_BUTTON && status == GLUT_DOWN) {
		right_mouse_status = true;
		OldMovex = x;
		OldMovex = y;
		//cout << x << ":" << y << endl;
	}
	if (button == GLUT_RIGHT_BUTTON && status == GLUT_UP) {
		right_mouse_status = false;
	}
	if (button == 3 && status == GLUT_UP) {
		scale *= 1.1f; 
	}
	if (button == 4 && status == GLUT_UP) {
		scale *= 0.9f;
	}
	glutPostRedisplay();
}

void onMouseMove(int x, int y) {
	if (left_mouse_status) {
		yRotate += Oldx - x;
		Oldx = x;
		xRotate += Oldy - y;
		glutPostRedisplay();
		Oldy = y;
		//cout << xRotate << ":" << yRotate << endl;
	}
	else if (right_mouse_status) {
		//xMove += (OldMovex - x)*0.01;
		//yMove = (OldMovey - y)*0.01;
		int disX = OldMovex - x;
		int disY = OldMovey - y;
		xMove += (
			disX > 0 ? -0.02 :
			disX == 0 ? 0 : 0.02
		);
		yMove += (
			disY > 0 ? 0.02 :
			disY == 0 ? 0 : -0.02
			);
		glutPostRedisplay();
		OldMovex = x;
		OldMovey = y;
		//cout << xMove << ":" << yMove << endl;
	}
	glutPostRedisplay();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1':
		showFace = true;
		showWires = false;
		break;
	case '2':
		showFace = false;
		showWires = true;
		break;
	case '3':
		showFace = true;
		showWires = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}