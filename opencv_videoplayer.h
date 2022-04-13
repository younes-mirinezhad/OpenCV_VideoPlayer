#ifndef OPENCV_VIDEOPLAYER_H
#define OPENCV_VIDEOPLAYER_H

#include <QThread>
#include <QUrl>
#include <QSize>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <opencv_player_viewport.h>

class OpenCV_VideoPlayer : public QThread
{
    Q_OBJECT
    Q_PROPERTY(OpenCV_Player_ViewPort* viewPort READ viewPort WRITE setViewPort NOTIFY viewPortChanged)
    Q_PROPERTY(QUrl videoUrl READ videoUrl WRITE setVideoUrl NOTIFY videoUrlChanged)
    Q_PROPERTY(int videoFrameCount READ videoFrameCount NOTIFY videoFrameCountChanged)
    Q_PROPERTY(QSize inputResulation READ inputResulation NOTIFY inputResulationChanged)
    Q_PROPERTY(int frameNumber READ frameNumber WRITE setFrameNumber NOTIFY frameNumberChanged)
    Q_PROPERTY(PlaybackState playbackState READ playbackState WRITE setPlaybackState NOTIFY playbackStateChanged)

public:
    enum PlaybackState { StoppedState, PlayingState, PausedState };
    Q_ENUM(PlaybackState)

    explicit OpenCV_VideoPlayer(QObject *parent = nullptr);
    void paintFrame(QPainter *painter, OpenCV_Player_ViewPort *viewport);

    OpenCV_Player_ViewPort *viewPort() const;
    QUrl videoUrl() const;
    int videoFrameCount() const;
    int frameNumber() const;
    PlaybackState playbackState() const;
    QSize inputResulation() const;

public Q_SLOTS:
    void setViewPort(OpenCV_Player_ViewPort *newViewPort);
    void setVideoUrl(QUrl newVideoUrl);
    void setFrameNumber(int newFrameNumber);
    void setPlaybackState(OpenCV_VideoPlayer::PlaybackState newPlaybackState);

private:
    void paintFrame(QPainter *painter, const cv::Mat &mat, const QSize &size);
    void extractFrame();

    OpenCV_Player_ViewPort *m_viewPort;
    QUrl m_videoUrl;
    int m_videoFrameCount=-1;
    int m_frameNumber=-1;
    PlaybackState m_playbackState{StoppedState};
    QSize m_inputResulation;
    QAtomicInt _locker;
    cv::Mat _lastFrameMat;
    QMutex _captureMutex;
    cv::VideoCapture _videoCapture;
    cv::Mat _frame;
    double _fps=0;

protected:
    void run();

Q_SIGNALS:
    void updateRequired();
    void viewPortChanged(OpenCV_Player_ViewPort *viewPort);
    void videoUrlChanged(QUrl videoUrl);
    void videoFrameCountChanged(int videoFrameCount);
    void frameNumberChanged(int frameNumber);
    void playbackStateChanged(PlaybackState playbackState);
    void inputResulationChanged(QSize inputResulation);
    void videoFinished();
};

#endif // OPENCV_VIDEOPLAYER_H
