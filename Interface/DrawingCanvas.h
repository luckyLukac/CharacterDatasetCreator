#pragma once
#include <limits>
#include <QTabletEvent>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>

#include "Structs/Structs.h"


class DrawingCanvas : public QWidget {
    Q_OBJECT
private:
    bool m_Drawing;                                             // Drawing is on if true.
    QImage m_Image = QImage(5000, 5000, QImage::Format_RGB32);  // Actual "canvas" to draw onto.
    QPoint m_LastPoint;                                         // Position of the last point.
    std::vector<Point> m_Points;                                // Vector of the drawn points.


protected:
    /// <summary>
    /// If mouse is pressed, the drawing procedure is commenced.
    /// </summary>
    /// <param name="event">Mouse click</param>
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            m_Drawing = true;
            m_LastPoint = event->pos();
            m_Points.push_back(Point(m_LastPoint.x(), m_LastPoint.y()));
            emit canvasClicked(m_LastPoint);
        }
    }

    /// <summary>
    /// Drawing of the line between the current and previous points if drawing is enabled.
    /// </summary>
    /// <param name="event">Mouse move</param>
    void mouseMoveEvent(QMouseEvent* event) override {
        if (m_Drawing && event->buttons() & Qt::LeftButton) {
            QPainter painter(&m_Image);
            painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(m_LastPoint, event->pos());
            m_LastPoint = event->pos();
            m_Points.push_back(Point(m_LastPoint.x(), m_LastPoint.y()));
            update();
        }
    }

    /// <summary>
    /// If mouse is released, the drawing procedure is ended.
    /// </summary>
    /// <param name="event">Mouse release</param>
    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && m_Drawing) {
            m_Drawing = false;
            m_Points.push_back(Point(-1, -1));
            update();
        }
    }

    /// <summary>
    /// Tablet event for the Wacom tablet.
    /// </summary>
    /// <param name="event">Touch event</param>
    void tabletEvent(QTabletEvent* event) override {
        QPoint pos = event->position().toPoint();  // Get the position of the stylus.
        int pressure = event->pressure() * 100;    // Pressure (range 0 to 1, scaled to 0-100).

        if (pressure < 50) {
            return;
        }
        
        // Set pen thickness based on pressure.
        QPainter painter(&m_Image);
        painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        if (event->type() == QEvent::TabletPress) {
            m_Drawing = true;
            m_LastPoint = event->position().toPoint();
            m_Points.push_back(Point(m_LastPoint.x(), m_LastPoint.y()));
            emit canvasClicked(m_LastPoint);
        }
        else if (event->type() == QEvent::TabletMove) {
            painter.drawLine(m_LastPoint, pos);  // Draw the stroke with the given pressure.
            m_LastPoint = pos;
            m_Points.push_back(Point(m_LastPoint.x(), m_LastPoint.y()));
        }
        else if (event->type() == QEvent::TabletRelease) {
            m_Drawing = false;
            m_Points.push_back(Point(-1, -1));
        }

        update();         // Trigger a repaint to show updated image.
        event->accept();  // Accept the event to indicate it's been handled.
    }

    /// <summary>
    /// Painting of the image on the canvas.
    /// </summary>
    /// <param name="event">Paint event</param>
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.drawImage(event->rect(), m_Image, event->rect());
    }


signals:
    /// <summary>
    /// Click of the canvas.
    /// </summary>
    void canvasClicked(const QPoint& point);


public:
    /// <summary>
    /// Constructor of the canvas.
    /// </summary>
    /// <param name="parent">Widget parent</param>
    DrawingCanvas(QWidget* parent = nullptr) : QWidget(parent), m_Drawing(false) {
        setAttribute(Qt::WA_StaticContents);
        m_Image.fill(Qt::white);              // Ensure canvas starts with a white background
    }

    /// <summary>
    /// Min-max normalization of the character.
    /// </summary>
    /// <returns>Normalized vector of the character's points</returns>
    std::vector<Point> confirmCharacter() {
        if (m_Points.empty())
            return {};

        // Finding extreme coordinates.
        double minX = std::numeric_limits<double>::max();
        double maxX = 0.0;
        double minY = std::numeric_limits<double>::max();
        double maxY = 0.0;

        for (const Point& p : m_Points) {
            // Checking if the point is valid.
            if (p.x < 0.0)
                continue;

            // Finding minimum and maximum X.
            if (p.x < minX)
                minX = p.x;
            if (p.x > maxX)
                maxX = p.x;

            // Finding minimum and maximum Y.
            if (p.y < minY)
                minY = p.y;
            if (p.y > maxY)
                maxY = p.y;
        }

        double deltaX = maxX - minX;
        double deltaY = maxY - minY;

        // Normalization of the points.
        std::vector<Point> normalizedPoints;
        for (const Point& p : m_Points) {
            normalizedPoints.push_back(Point((p.x - minX) / deltaX, (p.y - minY) / deltaY));
        }

        clearCanvas();

        return normalizedPoints;
    }

    /// <summary>
    /// Clearing of the canvas.
    /// </summary>
    void clearCanvas() {
        m_Image.fill(Qt::white);
        m_Points.clear();
        update();
    }
};