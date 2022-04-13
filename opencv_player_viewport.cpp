#include "opencv_player_viewport.h"
#include <opencv_videoplayer.h>
#include <QPainter>

OpenCV_Player_ViewPort::OpenCV_Player_ViewPort(QQuickItem *parent) : QQuickPaintedItem(parent) { }

void OpenCV_Player_ViewPort::paint(QPainter *painter)
{
    if (Q_UNLIKELY(!m_openCV_Player)) {
        qWarning("empty frame");
        painter->fillRect(clipRect(), Qt::black);
        return;
    }
    m_openCV_Player->paintFrame(painter, this);
}

void OpenCV_Player_ViewPort::playbackStarted()
{
    update();
}

void OpenCV_Player_ViewPort::setOpenCV_Player(OpenCV_VideoPlayer *newOpenCV_Player)
{
    qDebug() << "---" << Q_FUNC_INFO;
    connect(newOpenCV_Player, &OpenCV_VideoPlayer::updateRequired, this, &QQuickItem::update);
    m_openCV_Player = newOpenCV_Player;
}
