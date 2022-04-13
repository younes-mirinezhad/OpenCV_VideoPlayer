#ifndef OPENCV_PLAYER_VIEWPORT_H
#define OPENCV_PLAYER_VIEWPORT_H

#include <QQuickPaintedItem>

class OpenCV_VideoPlayer;
class OpenCV_Player_ViewPort : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit OpenCV_Player_ViewPort(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    void setOpenCV_Player(OpenCV_VideoPlayer *newOpenCV_Player);

public Q_SLOTS:
    void playbackStarted();

private:
    OpenCV_VideoPlayer *m_openCV_Player;

};

#endif // OPENCV_PLAYER_VIEWPORT_H
