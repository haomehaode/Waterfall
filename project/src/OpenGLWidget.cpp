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
	//������ɫģʽ��ƽ����
	glShadeModel(GL_SMOOTH);
	//�����֮ǰ��������ɫ
	glClearColor(m_back_color.red(), m_back_color.green(), m_back_color.blue(), m_back_color.alpha());
	//��Ȼ���,���ó�ʼֵΪ1.0��С��1.0�Ĳ����ǿɼ���
	glClearDepth(1.0);
	//����OPenGL����ع��ܣ��ɲ�������������ָ
	//��������֮��OpenGL�ڻ��Ƶ�ʱ��ͻ��飬��ǰ����ǰ���Ƿ��б�����أ����������ص��������������Ͳ�����ƣ�Ҳ����˵��OpenGL��ֻ������ǰ���һ�㣩
	glEnable(GL_DEPTH_TEST);
	//�ƶ���Ȼ���Ƚ�ֵ
	//�������ָ���������������ֵС�ڻ��ߵ��ڲο�ֵ��ͨ��
	glDepthFunc(GL_LEQUAL);
	//����ݷ�����
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	//����ݳ�����
	glEnable(GL_MULTISAMPLE);
	//������ɫ���
	glEnable(GL_BLEND);
	//ָ���������
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLWidget::paintGL()
{
	//�����ɫ�������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//����ǰ��������Ļ���ģ��൱�ڸ�λ�Ĳ���
	glLoadIdentity();
	//ƽ�ƺ���,����ָ���Ƿֱ��X�ᣬY�ᣬZ��ƽ��
	glTranslatef(m_translate.x(), m_translate.y(), m_translate.z());
	//��ת����,����ָ���Ƿֱ��X�ᣬY�ᣬZ����ת
	glRotatef(m_rotation.y(), 1.0, 0.0, 0.0);
	glRotatef(m_rotation.x(), 0.0, 1.0, 0.0);
	glRotatef(m_rotation.z(), 0.0, 0.0, 1.0);

	glScalef(2,1,1);

	draw_coordinate();
	draw_axis();
	draw_ticks();
	draw_lable();


	//����ģ������,x,y,z �ֱ�Ϊģ����x,y,z �᷽������ű�
	QVector3D vec_length = m_max - m_min;
	QVector3D vec_scale = QVector3D(2, 2, 2.0f / (m_lines.count() + 1) * (m_lines.count() - 1)) / vec_length;
	if (vec_length.z()!=0)
		glScalef(vec_scale.x(), vec_scale.y(), vec_scale.z());
	else
		glScalef(vec_scale.x(), vec_scale.y(),1);
	//ƽ�ƺ���,����ָ���Ƿֱ��X�ᣬY�ᣬZ��ƽ��

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

	//���߻��Ƶ�������ĸ�λ��
	glViewport(0, 0, width, height);
	// ���þ���ģʽ������������ΪͶӰ����
	glMatrixMode(GL_PROJECTION);
	//��λ����
	glLoadIdentity();
	//����glFrustum�����ɾ����뵱ǰ������ˣ�����͸��Ч��
	GLdouble aspectRatio = (GLfloat)width / (GLfloat)height;
	GLdouble rFov = 45.0 * 3.14159265 / 180.0;
	GLdouble zNear = 0.1;
	GLdouble zFar = 1000.0;
	glFrustum(-zNear * tan(rFov / 2.0) * aspectRatio,
		zNear * tan(rFov / 2.0) * aspectRatio,
		-zNear * tan(rFov / 2.0),
		zNear * tan(rFov / 2.0),
		zNear, zFar);
	//�л�ģ����ͼ����
	glMatrixMode(GL_MODELVIEW);
	//��λ
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

	//renderText(1.3, -1, 0, "ϵ��", font);

	renderText(-1.15, 0, 1.15, "Y ��", font);

	renderText(0, -1, 1.4, "X ��", font);
}
