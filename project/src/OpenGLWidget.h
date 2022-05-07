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
	/** 横纵比 */
	QVector3D m_scale;
	/** 背景色 */
	QColor m_back_color;
	/** 放大 */
	QVector3D m_translate;
	/** 旋转 */
	QVector3D m_rotation;
	/** 上一次位置 */
	QPoint m_last_pos;
	/** 曲线 */
	QList<Line3D*> m_lines;
	/** X 坐标轴名称 */
	QString m_x_lable;
	/** Y 坐标轴名称 */
	QString m_y_lable;
	/** Z 坐标轴名称 */
	QString m_z_lable;
	/** X 坐标轴数字刻度 */
	QVector<double> m_x_ticks;
	/** Y 坐标轴数字刻度 */
	QVector<double> m_y_ticks;
	/** Z 坐标轴文字刻度 */
	QVector<QString> m_z_ticks;
	/** 坐标轴最小向量 */
	QVector3D m_min;
	/** 坐标轴最大向量 */
	QVector3D m_max;
};
#endif // OPENGLWIDGET_H
