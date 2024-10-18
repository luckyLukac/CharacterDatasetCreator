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
    bool m_Active;                                              // Drawing enabled if active.
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
        if (m_Active && event->button() == Qt::LeftButton) {
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
        if (m_Active && m_Drawing && event->buttons() & Qt::LeftButton) {
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
        if (m_Active && event->button() == Qt::LeftButton && m_Drawing) {
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
        if (!m_Active) {
            return;
        }

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
    DrawingCanvas(const bool active = false, QWidget* parent = nullptr) : QWidget(parent), m_Active(active), m_Drawing(false) {
        setAttribute(Qt::WA_StaticContents);
        m_Image.fill(Qt::white);              // Ensure canvas starts with a white background
    }

    /// <summary>
    /// Confirming the character.
    /// </summary>
    /// <returns>Vector of the character's points</returns>
    std::vector<Point> confirmCharacter() {
        std::vector<Point> points(m_Points);

        if (points.empty())
            return {};

        clearCanvas();

        return points;
    }

    /// <summary>
    /// Drawing the line on the canvas.
    /// </summary>
    /// <param name="p1">Start point</param>
    /// <param name="p2">End point</param>
    void drawLine(const Point& p1, const Point& p2) {
        QPainter painter(&m_Image);
        painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(QPoint(p1.x, p1.y), QPoint(p2.x, p2.y));
        m_LastPoint = QPoint(p2.x, p2.y);
        m_Points.push_back(Point(m_LastPoint.x(), m_LastPoint.y()));
        update();
    }

    /// <summary>
    /// Clearing of the canvas.
    /// </summary>
    void clearCanvas() {
        m_Image.fill(Qt::white);
        m_Points.clear();
        update();
    }

    /// <summary>
    /// Checking whether the canvas is empty.
    /// </summary>
    /// <returns>True if empty, false otherwise</returns>
    bool isEmpty() const {
        return m_Points.empty();
    }

    /// <summary>
    /// Disable the drawing on the canvas.
    /// </summary>
    void disable() {
        m_Active = false;
    }

    /// <summary>
    /// Enable the drawing on the canvas.
    /// </summary>
    void enable() {
        m_Active = true;
    }

    /// <summary>
    /// Checking whether the canvas is active.
    /// </summary>
    /// <returns></returns>
    bool isActive() const {
        return m_Active;
    }
};