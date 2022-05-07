#pragma execution_character_set("utf-8")

#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
	:QGLWidget(parent)
{
	m_scale = QVector3D(1, 1, 1);
	m_rotation = QVector3D(0, 10, 0);
	//m_rotation = QVector3D(-45, 45, 0);
	m_translate = QVector3D(0, 0, -5);
	m_back_color = Qt::white;
	m_min = QVector3D(DBL_MAX, DBL_MAX, DBL_MAX);
	m_max = QVector3D(-DBL_MAX, -DBL_MAX, -DBL_MAX);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::add_line(Line3D* line)
{
	m_lines.append(line);

	QVector3D min = line->get_min(), max = line->get_max();
	if (max.x() > m_max.x()) m_max.setX(max.x());
	if (max.y() > m_max.y()) m_max.setY(max.y());
	if (max.z() > m_max.z()) m_max.setZ(max.z());
	if (min.x() < m_min.x()) m_min.setX(min.x());
	if (min.y() < m_min.y()) m_min.setY(min.y());
	if (min.z() < m_min.z()) m_min.setZ(min.z());

	m_z_ticks.append(line->get_name());

	update();
}

void OpenGLWidget::initializeGL()
{
	//设置着色模式，平滑的
	glShadeModel(GL_SMOOTH);
	//清除掉之前的所有颜色
	glClearColor(m_back_color.red(), m_back_color.green(), m_back_color.blue(), m_back_color.alpha());
	//深度缓存,设置初始值为1.0，小于1.0的部分是可见的
	glClearDepth(1.0);
	//启动OPenGL的相关功能，由参数决定，这里指
	//（启用了之后，OpenGL在绘制的时候就会检查，当前像素前面是否有别的像素，如果别的像素挡道了它，那它就不会绘制，也就是说，OpenGL就只绘制最前面的一层）
	glEnable(GL_DEPTH_TEST);
	//制定深度缓存比较值
	//这里参数指的是如果输入的深度值小于或者等于参考值则通过
	glDepthFunc(GL_LEQUAL);
	//看锯齿反走样
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	//抗锯齿出处理
	glEnable(GL_MULTISAMPLE);
	//启动颜色混合
	glEnable(GL_BLEND);
	//指定混合因子
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLWidget::paintGL()
{
	//清除颜色缓冲和深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//将当前点移置屏幕中心，相当于复位的操作
	glLoadIdentity();
	//平移函数,参数指的是分别从X轴，Y轴，Z轴平移
	glTranslatef(m_translate.x(), m_translate.y(), m_translate.z());
	//旋转函数,参数指的是分别从X轴，Y轴，Z轴旋转
	glRotatef(m_rotation.y(), 1.0, 0.0, 0.0);
	glRotatef(m_rotation.x(), 0.0, 1.0, 0.0);
	glRotatef(m_rotation.z(), 0.0, 0.0, 1.0);

	glScalef(2,1,1);

	draw_coordinate();
	draw_axis();
	draw_ticks();
	draw_lable();


	//设置模型缩放,x,y,z 分别为模型在x,y,z 轴方向的缩放比
	QVector3D vec_length = m_max - m_min;
	QVector3D vec_scale = QVector3D(2, 2, 2.0f / (m_lines.count() + 1) * (m_lines.count() - 1)) / vec_length;
	if (vec_length.z()!=0)
		glScalef(vec_scale.x(), vec_scale.y(), vec_scale.z());
	else
		glScalef(vec_scale.x(), vec_scale.y(),1);
	//平移函数,参数指的是分别从X轴，Y轴，Z轴平移

	glTranslatef(-(m_min.x()+vec_length.x()/2), -(m_min.y() + vec_length.y() / 2), -vec_length.z()/2);

	//glTranslatef(0,0,0);
	//glTranslatef((m_max.x() - m_min.x()) / 2, (m_max.y() - m_min.y()) / 2, (m_max.z() - m_min.z()) / 2);

	for (auto& item : m_lines)
		item->draw();

	glLoadIdentity();
}

void OpenGLWidget::resizeGL(int width, int height)
{
	//m_scale.setX(width/height);

	//告诉绘制到窗体的哪个位置
	glViewport(0, 0, width, height);
	// 设置矩阵模式，参数是设置为投影矩阵
	glMatrixMode(GL_PROJECTION);
	//复位操作
	glLoadIdentity();
	//调用glFrustum，生成矩阵与当前矩阵相乘，生成透视效果
	GLdouble aspectRatio = (GLfloat)width / (GLfloat)height;
	GLdouble rFov = 45.0 * 3.14159265 / 180.0;
	GLdouble zNear = 0.1;
	GLdouble zFar = 1000.0;
	glFrustum(-zNear * tan(rFov / 2.0) * aspectRatio,
		zNear * tan(rFov / 2.0) * aspectRatio,
		-zNear * tan(rFov / 2.0),
		zNear * tan(rFov / 2.0),
		zNear, zFar);
	//切回模型视图矩阵
	glMatrixMode(GL_MODELVIEW);
	//复位
	glLoadIdentity();

}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	m_last_pos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	int dx = event->x() - m_last_pos.x();
	int dy = event->y() - m_last_pos.y();

	if (event->buttons() & Qt::LeftButton)
	{
		m_rotation.setX(m_rotation.x() + dx / 5.0f);
		m_rotation.setY(m_rotation.y() + dy / 5.0f);
		update();
	}

	m_last_pos = event->pos();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	event->accept();

	double value = m_translate.z() + (double)event->delta()/200;
	if (value < -4)
		m_translate.setZ(value);

	update();
}

void OpenGLWidget::draw_coordinate()
{
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glColor4f(0, 0, 0, 0.5);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor4f(0, 0, 0, 0.5);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor4f(0, 0, 0, 0.5);
	glVertex3f(-1, -1, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glEnd();

	//glBegin(GL_LINE_LOOP);
	//glColor3f(0, 1, 0);
	//glVertex3f(1, 1, -1);
	//glVertex3f(1, -1, -1);
	//glVertex3f(1, -1, 1);
	//glVertex3f(1, 1, 1);
	//glEnd();
	//glBegin(GL_LINE_LOOP);
	//glColor3f(0, 0, 1);
	//glVertex3f(1, -1, 1);
	//glVertex3f(1, 1, 1);
	//glVertex3f(-1, 1, 1);
	//glVertex3f(-1, -1, 1);
	//glEnd();
	//glBegin(GL_LINE_LOOP);
	//glColor3f(0, 1, 0);
	//glVertex3f(-1, 1, 1);
	//glVertex3f(1, 1, 1);
	//glVertex3f(1, 1, -1);
	//glVertex3f(-1, 1, -1);
	//glEnd();
}

void OpenGLWidget::draw_axis() 
{
	float interval = 2.0f / (float)(m_z_ticks.count() + 1);
	//glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for (float i = -1; i <= 1; i = i + interval)
	{
		glVertex3f(-1, -1, i);
		glVertex3f(1.05,-1, i);
	}
	glEnd();

	interval = 2.0f / 9.0f;

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for (float i = -1; i <= 1; i = i + interval)
	{
		glVertex3f(-1, i,-1);
		glVertex3f(-1, i, 1);
	}
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for (float i = -1; i <= 1; i = i + interval)
	{
		glVertex3f(i,-1,-1);
		glVertex3f(i,-1,1.05);
	}
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for (float i = -1; i <= 1; i = i + interval)
	{
		glVertex3f(-1, -i,-1);
		glVertex3f(1.05, -i,-1);
	}
	glEnd();
}

void OpenGLWidget::draw_ticks()
{
	QFont font("Helvetica", 10);

	float interval = 2.0f / (float)(m_z_ticks.count() + 1);
	int index = 0;
	for (float i = -1 + interval; i < 1; i = i + interval)
	{
		renderText(1.1, -1, i, m_z_ticks.at(index), font);
		index++;
	}

	interval = 2.0f / 9.0f;

	index = 0;
	float x_min = m_min.x(), x_max = m_max.x(), x_gap = (x_max - x_min) / 9.0f;
	for (float i = -1; i <= 1; i = i + interval)
	{
		renderText(i, -1, 1.15, QString::number(x_min + index * x_gap, 'f', 2), font);
		index++;
	}

	index = 0;
	float y_min = m_min.y(), y_max = m_max.y(), y_gap = (y_max - y_min) / 9.0f;
	for (float i = -1; i <= 1; i = i + interval)
	{
		renderText(1.1, -i, -1, QString::number(y_max - index * y_gap, 'f', 2), font);
		index++;
	}

}

void OpenGLWidget::draw_lable()
{
	QFont font("Helvetica", 14);

	//renderText(1.3, -1, 0, "系列", font);

	renderText(-1.15, 0, 1.15, "Y 轴", font);

	renderText(0, -1, 1.4, "X 轴", font);
}
