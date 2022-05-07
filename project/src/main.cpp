#pragma execution_character_set("utf-8")

#include "OpenGLWidget.h"
#include <QtWidgets/QApplication>

// �ٲ�ͼ��N��������ZֵӦ����0��ʼ��N-1;

// �ٲ�ͼ��ͬ�߶���ɫ��ͬ�ο�line3D draw ���滭����ɫ�ߴ���

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	OpenGLWidget w;
	w.setWindowState(Qt::WindowMaximized);
	//Line3D item1("����һ");
	//for (int i = -20; i <= 10; i++)
	//	item1.add_data(i,i, 0);
	//item1.set_color(Qt::blue);
	//item1.set_line_width(2);
	//w.add_line(&item1);
	Line3D item2("���Զ�");
	for (int i = 1; i <= 10; i++)
		item2.add_data(i, i, 0);
	item2.set_color(Qt::red);
	item2.set_line_width(2);
	w.add_line(&item2);
	Line3D item3("������");
	for (int i = 1; i <= 10; i++)
		item3.add_data(i, i, 1);
	item3.set_color(Qt::yellow);
	item3.set_line_width(2);
	w.add_line(&item3);
	Line3D item4("������");
	for (int i = 1; i <= 10; i++)
		item4.add_data(i, i, 2);
	item4.set_color(Qt::yellow);
	item4.set_line_width(2);
	w.add_line(&item4);
	Line3D item5("������");
	for (int i = 1; i <= 10; i++)
		item5.add_data(i, i, 3);
	item5.set_color(Qt::yellow);
	item5.set_line_width(2);
	w.add_line(&item5);
	w.show();
	return a.exec();
}
