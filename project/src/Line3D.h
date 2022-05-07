#ifndef LINE_3D_H
#define LINE_3D_H

#include <QtOpenGL>

class Line3D
{
public:
	Line3D(QString name);
	void set_color(QColor color);
	void set_line_width(double value);
	void set_name(QString name);
	QString get_name();
	void set_min(QVector3D min);
	void set_max(QVector3D max);
	QVector3D get_min();
	QVector3D get_max();
	void add_data(const double& x, const double& y, const double& z);
	void add_data(const QVector<double>& x, const QVector<double>& y, const QVector<double>& z);
	void add_data(const QVector<QVector3D>& data);
	void add_data(const QVector3D& data);
	void clear();
	void draw() const;

private:
	/** ���� */
	QString m_name;
	/** ��ɫ */
	QColor m_color;
	/** �߿� */
	double m_line_width;
	/** ���� */
	QVector<QVector3D> m_data;
	/** ��Сֵ */
	QVector3D m_min;
	/** ���ֵ */
	QVector3D m_max;
};

#endif // LINE_3D_H