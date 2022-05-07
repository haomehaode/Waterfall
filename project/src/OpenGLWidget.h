#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>
#include "Line3D.h"

#include "qgl.h"

class OpenGLWidget : public QGLWidget
{
public:
	OpenGLWidget(QWidget* parent = nullptr);
	~OpenGLWidget();

public:
	void add_line(Line3D* line);

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;

private:
	void draw_coordinate();
	void draw_axis();
	void draw_ticks();
	void draw_lable();

private:
	/** ���ݱ� */
	QVector3D m_scale;
	/** ����ɫ */
	QColor m_back_color;
	/** �Ŵ� */
	QVector3D m_translate;
	/** ��ת */
	QVector3D m_rotation;
	/** ��һ��λ�� */
	QPoint m_last_pos;
	/** ���� */
	QList<Line3D*> m_lines;
	/** X ���������� */
	QString m_x_lable;
	/** Y ���������� */
	QString m_y_lable;
	/** Z ���������� */
	QString m_z_lable;
	/** X ���������̶ֿ� */
	QVector<double> m_x_ticks;
	/** Y ���������̶ֿ� */
	QVector<double> m_y_ticks;
	/** Z ���������̶ֿ� */
	QVector<QString> m_z_ticks;
	/** ��������С���� */
	QVector3D m_min;
	/** ������������� */
	QVector3D m_max;
};
#endif // OPENGLWIDGET_H
