#include "drawingscene.h"

#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QFile>
#include <QTextStream>
#include <QtMath>

DrawingScene::DrawingScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

void DrawingScene::setShapeType(ShapeType type)
{
    m_shapeType = type;
}

void DrawingScene::setOutlineColor(const QColor &color)
{
    m_outlineColor = color;
}

void DrawingScene::setFillColor(const QColor &color)
{
    m_fillColor = color;
}

void DrawingScene::setEraseMode(bool enabled)
{
    m_eraseMode = enabled;
}

void DrawingScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_eraseMode)
    {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if (item)
        {
            removeItem(item);
            delete item;
        }
        return;
    }

    m_startPoint = event->scenePos();

    QPen pen(m_outlineColor);
    QBrush brush(m_fillColor);

    switch (m_shapeType)
    {
    case Line:
        m_currentItem = addLine(QLineF(m_startPoint, m_startPoint), pen);
        break;
    case Rectangle:
        m_currentItem = addRect(QRectF(m_startPoint, m_startPoint), pen, brush);
        break;
    case Ellipse:
        m_currentItem = addEllipse(QRectF(m_startPoint, m_startPoint), pen, brush);
        break;
    }
}

// Двигаем мышь с зажатой кнопкой -> растягиваем фигуру
void DrawingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_currentItem)
        return;

    QPointF currentPoint = event->scenePos();

    switch (m_shapeType)
    {
    case Line:
    {
        auto item = static_cast<QGraphicsLineItem *>(m_currentItem);
        item->setLine(QLineF(m_startPoint, currentPoint));
        break;
    }
    case Rectangle:
    {
        auto item = static_cast<QGraphicsRectItem *>(m_currentItem);
        item->setRect(QRectF(m_startPoint, currentPoint).normalized());
        break;
    }
    case Ellipse:
    {
        auto item = static_cast<QGraphicsEllipseItem *>(m_currentItem);
        item->setRect(QRectF(m_startPoint, currentPoint).normalized());
        break;
    }
    }
}

void DrawingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    m_currentItem = nullptr;
}

void DrawingScene::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    const auto allItems = items();
    for (QGraphicsItem *item : allItems)
    {
        if (auto line = qgraphicsitem_cast<QGraphicsLineItem *>(item))
        {
            QLineF l = line->line();
            out << "Line;" << l.x1() << ';' << l.y1() << ';' << l.x2() << ';' << l.y2()
                << ';' << line->pen().color().name() << ';' << "#000000" << '\n';
        }
        else if (auto rect = qgraphicsitem_cast<QGraphicsRectItem *>(item))
        {
            QRectF r = rect->rect();
            out << "Rectangle;" << r.left() << ';' << r.top() << ';' << r.right() << ';' << r.bottom()
                << ';' << rect->pen().color().name() << ';' << rect->brush().color().name() << '\n';
        }
        else if (auto ell = qgraphicsitem_cast<QGraphicsEllipseItem *>(item))
        {
            QRectF r = ell->rect();
            out << "Ellipse;" << r.left() << ';' << r.top() << ';' << r.right() << ';' << r.bottom()
                << ';' << ell->pen().color().name() << ';' << ell->brush().color().name() << '\n';
        }
    }

    file.close();
}

void DrawingScene::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    clear();

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(';');
        if (parts.size() != 7)
            continue;

        QString type = parts[0];
        double x1 = parts[1].toDouble();
        double y1 = parts[2].toDouble();
        double x2 = parts[3].toDouble();
        double y2 = parts[4].toDouble();
        QColor outlineColor(parts[5]);
        QColor fillColor(parts[6]);

        QPen pen(outlineColor);
        QBrush brush(fillColor);

        if (type == "Line")
            addLine(QLineF(x1, y1, x2, y2), pen);
        else if (type == "Rectangle")
            addRect(QRectF(QPointF(x1, y1), QPointF(x2, y2)), pen, brush);
        else if (type == "Ellipse")
            addEllipse(QRectF(QPointF(x1, y1), QPointF(x2, y2)), pen, brush);
    }

    file.close();
}

DrawingScene::ShapeStats DrawingScene::calculateStats() const
{
    ShapeStats stats;

    const auto allItems = items();
    for (QGraphicsItem *item : allItems)
    {
        if (qgraphicsitem_cast<QGraphicsLineItem *>(item))
        {
            stats.lineCount++;
        }
        else if (auto rect = qgraphicsitem_cast<QGraphicsRectItem *>(item))
        {
            stats.rectangleCount++;
            QRectF r = rect->rect();
            stats.totalArea += r.width() * r.height();
        }
        else if (auto ell = qgraphicsitem_cast<QGraphicsEllipseItem *>(item))
        {
            stats.ellipseCount++;
            QRectF r = ell->rect();
            double a = r.width() / 2.0;
            double b = r.height() / 2.0;
            stats.totalArea += M_PI * a * b;
        }
    }

    stats.totalCount = stats.lineCount + stats.rectangleCount + stats.ellipseCount;
    return stats;
}