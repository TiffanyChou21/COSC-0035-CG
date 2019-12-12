#include "MeshSimplify.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

MeshSimplify ms;

float scale = 1.0;
float xRotate = 30.0;
float yRotate = 30.0;
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

string in = "./objFile/";
string out = "./objFile/output.obj";


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	// ֱ����ʾ���
	cout << "��ѡ��򻯴���obj�ļ�" << endl;
	cout << "0.dragon\n1.Arma\n2.Block\n3.Buddha\n4.Bunny\n5.Dinosaur\n6.horse\n7.kitten\n";
	int ch; cin >> ch;
	cout << "��(y)��(n)ʹ��modify.obj�ӿ촦���ٶ��Ը��õĳ��������Ч��(����)\n";
	string  modify; cin >> modify;
	switch (ch)
	{
		case 0:if (modify == "y")in += "dragon_modify.obj"; else in += "dragon.obj"; break;
		case 1:if (modify == "y")in += "Arma_modify.obj"; else in += "Arma.obj"; break;
		case 2:if (modify == "y")in += "block_modify.obj"; else in += "block.obj"; break;
		case 3:if (modify == "y")in += "Buddha_modify.obj"; else in += "Buddha.obj"; break;
		case 4:if (modify == "y")in += "bunny_modify.obj"; else in += "bunny.obj"; break;
		case 5:if (modify == "y")in += "dinosaur_modify.obj"; else in += "dinosaur.obj"; break;
		case 6:if (modify == "y")in += "horse_modify.obj"; else in += "horse.obj"; break;
		case 7:if (modify == "y")in += "kitten_modify.obj"; else in += "kitten.obj"; break;
		default:if (modify == "y")in += "dragon_modify.obj"; else in += "dragon_modify.obj"; break;
	}
	cout << "ֱ����ʾ����ͼ��(y)����м򻯴���(n):";
	string s;
	cin >> s;
	if (s == "y" || s == "yes") {
		ms.readFile(in);
		//ms.delUselessLine();
		scale = 1/ms.maxLen * 0.75;
		Init();
		glutMainLoop();
	}
	//ms.readFile(in);
	////ms.delUselessLine();
	//scale = 1/ms.maxLen * 0.75;
	//Init();
	//glutMainLoop();

	 //��ȡ�ļ�
	time_t start = time(0);
	cout << "���ڶ�ȡ�ļ�..." << endl;
	ms.readFile(in);
	cout << "�ļ���ȡ�ɹ�" << endl;
	// �򻯴���
	cout << "������򻯱���(0-1֮��ĸ�����)" << endl;
	float rate;
	cin >> rate;
	if (rate > 1 || rate <= 0) {
		cout << "�������Ĭ������Ϊ0.1" << endl;
		rate = 0.1;
	}
	cout << "���ڼ򻯴���..." << endl;
	ms.simplify(rate);
	cout << "�򻯴���ɹ�" << endl;
	// д�����
	cout << "����д���ļ�..." << endl;
	ms.writeFile(out);
	cout << "�ļ�д��ɹ�" << endl;
	// ��ʾЧ��
	time_t end = time(0);
	cout << "��ʱ" << end - start << "s\n";
	cout << "�Ƿ�OpenGL��ʾ��ͼ��(y/n)" << endl;
	cout << "�����ת,�Ҽ�ƽ��,��������,����1��2��3ѡ��ͬ������ʾģʽ\n";
	cin >> s;
	if (s == "y" || s == "yes") {
		ms.readFile(out);
		//ms.delUselessLine();
		scale = 1 / ms.maxLen * 0.75;
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
	//����objģ��
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
		Oldy = y;
		glutPostRedisplay();
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