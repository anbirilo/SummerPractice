#ifndef DRAWINGSCENE_H
#define DRAWINGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QColor>

class DrawingScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum ShapeType
    {
        Line,
        Rectangle,
        Ellipse
    };
    struct ShapeStats
    {
        int lineCount = 0;
        int rectangleCount = 0;
        int ellipseCount = 0;
        int totalCount = 0;
        double totalArea = 0.0; // сумма площадей прямоугольников и эллипсов
    };

    ShapeStats calculateStats() const;

    explicit DrawingScene(QObject *parent = nullptr);

    void setShapeType(ShapeType type);
    void setOutlineColor(const QColor &color);
    void setFillColor(const QColor &color);
    void setEraseMode(bool enabled);

    void saveToFile(const QString &fileName);
    void loadFromFile(const QString &fileName);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    ShapeType m_shapeType = Rectangle;
    QColor m_outlineColor = Qt::black;
    QColor m_fillColor = Qt::black;

    QPointF m_startPoint;
    QGraphicsItem *m_currentItem = nullptr;

    bool m_eraseMode = false;
};

#endif // DRAWINGSCENE_H