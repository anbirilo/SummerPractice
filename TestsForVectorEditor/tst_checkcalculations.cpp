#include <QtTest>
#include "drawingscene.h"

class CheckCalculations : public QObject
{
    Q_OBJECT

public:
    CheckCalculations();
    ~CheckCalculations() override;

private slots:
    void testEmptyScene();
    void testOnlyRectangles();
};

CheckCalculations::CheckCalculations() {}

CheckCalculations::~CheckCalculations() = default;

void CheckCalculations::testEmptyScene()
{
    DrawingScene scene;

    DrawingScene::ShapeStats stats = scene.calculateStats();

    QCOMPARE(stats.lineCount, 0);
    QCOMPARE(stats.rectangleCount, 0);
    QCOMPARE(stats.ellipseCount, 0);
    QCOMPARE(stats.totalCount, 0);
    QCOMPARE(stats.totalArea, 0.0);
}

void CheckCalculations::testOnlyRectangles()
{
    DrawingScene scene;

    scene.addRect(QRectF(0, 0, 10, 5));
    scene.addRect(QRectF(0, 0, 4, 4));

    DrawingScene::ShapeStats stats = scene.calculateStats();

    QCOMPARE(stats.rectangleCount, 2);
    QCOMPARE(stats.totalCount, 2);
    QCOMPARE(stats.totalArea, 66.0);
}

QTEST_MAIN(CheckCalculations)
#include "tst_checkcalculations.moc"