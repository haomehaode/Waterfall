#include "Line3D.h"

Line3D::Line3D(QString name)
{
	m_name = name;
	m_color = Qt::blue;
	m_line_width = 1;
	m_min = QVector3D(DBL_MAX, DBL_MAX, DBL_MAX);
	m_max = QVector3D(-DBL_MAX, -DBL_MAX, -DBL_MAX);
}

void Line3D::set_color(QColor color)
{
	m_color = color;
}

void Line3D::set_line_width(double value)
{
	m_line_width = value;
}

void Line3D::set_name(QString name)
{
	m_name = name;
}

QString Line3D::get_name()
{
	return m_name;
}

void Line3D::set_min(QVector3D min)
{
	m_min = min;
}

void Line3D::set_max(QVector3D max)
{
	m_max = max;
}

QVector3D Line3D::get_min()
{
	return m_min;
}

QVector3D Line3D::get_max()
{
	return m_max;
}

void Line3D::add_data(const double& x, const double& y, const double& z)
{
	add_data(QVector3D(x, y, z));
}

void Line3D::add_data(const QVector<double>& x, const QVector<double>& y, const QVector<double>& z)
{
	int size = x.size();
	for (int i = 0; i < size; i++)
		add_data(x[i], y[i], z[i]);
}

void Line3D::add_data(const QVector<QVector3D>& data)
{
	int size = data.size();
	for (int i = 0; i < size; i++)
		add_data(data[i]);
}

void Line3D::add_data(const QVector3D& data)
{
	if (data.x() > m_max.x()) m_max.setX(data.x());
	if (data.y() > m_max.y()) m_max.setY(data.y());
	if (data.z() > m_max.z()) m_max.setZ(data.z());
	if (data.x() < m_min.x()) m_min.setX(data.x());
	if (data.y() < m_min.y()) m_min.setY(data.y());
	if (data.z() < m_min.z()) m_min.setZ(data.z());

	m_data.push_back(data);
}

void Line3D::clear()
{
	m_data.clear();
}

void Line3D::draw() const
{

	glColor3f(m_color.redF(), m_color.greenF(), m_color.blueF());
	glLineWidth(m_line_width);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < m_data.size(); i++)
		glVertex3f(m_data[i].x(), m_data[i].y(), m_data[i].z());
	glEnd();

	//画有颜色的线
	//glBegin(GL_LINE_STRIP);

	//glVertex3f(0.0f, 0.0f, 1.0f);
	//glColor4ub(0, 255, 0, 255);

	//glVertex3f(1.0f, 0.0f, 1.0f);
	//glColor4ub(0, 0, 255, 255);

	//glVertex3f(1.0f, -2.0f, 1.0f);
	//glColor4ub(255, 255, 255, 255);

	//glEnd();
}
