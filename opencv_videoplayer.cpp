#include "opencv_videoplayer.h"
#include <QImage>
#include <QPainter>

OpenCV_VideoPlayer::OpenCV_VideoPlayer(QObject *parent) : QThread(parent) {}

OpenCV_Player_ViewPort *OpenCV_VideoPlayer::viewPort() const {
  return m_viewPort;
}
void OpenCV_VideoPlayer::setViewPort(OpenCV_Player_ViewPort *newViewPort) {
  qDebug() << "---" << Q_FUNC_INFO;
  if (m_viewPort == newViewPort)
    return;

  newViewPort->setOpenCV_Player(this);
  m_viewPort = newViewPort;
  Q_EMIT viewPortChanged(m_viewPort);
}

QUrl OpenCV_VideoPlayer::videoUrl() const { return m_videoUrl; }
void OpenCV_VideoPlayer::setVideoUrl(const QUrl newVideoUrl) {
  qDebug() << "---" << Q_FUNC_INFO << newVideoUrl;
  if (m_videoUrl == newVideoUrl)
    return;
  m_videoUrl = newVideoUrl;
  Q_EMIT videoUrlChanged(m_videoUrl);

  if (_videoCapture.isOpened())
    _videoCapture.release();

  auto videoPath = newVideoUrl.toLocalFile().toStdString();
  if (!_videoCapture.open(videoPath)) {
    qDebug() << "------Video capture unable to open the file -> "
             << newVideoUrl;
    return;
  }

  _fps = static_cast<double>(_videoCapture.get(cv::CAP_PROP_FPS));
  m_videoFrameCount =
      static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_COUNT));
  Q_EMIT videoFrameCountChanged(m_videoFrameCount);

  auto framesWidth =
      static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
  auto framesHeight =
      static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));
  m_inputResulation = {framesWidth, framesHeight};
  Q_EMIT inputResulationChanged(m_inputResulation);

  m_frameNumber = 0;
  m_playbackState = PausedState;
  extractFrame();
}
int OpenCV_VideoPlayer::videoFrameCount() const { return m_videoFrameCount; }
QSize OpenCV_VideoPlayer::inputResulation() const { return m_inputResulation; }

int OpenCV_VideoPlayer::frameNumber() const { return m_frameNumber; }
void OpenCV_VideoPlayer::setFrameNumber(int newFrameNumber) {
  qDebug() << "---" << Q_FUNC_INFO << newFrameNumber;
  if (m_frameNumber == newFrameNumber)
    return;

  if (!_videoCapture.isOpened()) {
    qDebug() << "------ Video capture is not open";
    return;
  }

  newFrameNumber--;
  if (newFrameNumber > m_videoFrameCount)
    newFrameNumber = m_videoFrameCount - 1;
  if (newFrameNumber < 1)
    newFrameNumber = 0;

  cv::Mat frame;
  _captureMutex.lock();
  _videoCapture.set(cv::CAP_PROP_POS_FRAMES, newFrameNumber - 1);
  _videoCapture >> frame;
  int current_frame = _videoCapture.get(cv::CAP_PROP_POS_FRAMES);
  while (current_frame < newFrameNumber) {
    _videoCapture >> frame;
    current_frame = _videoCapture.get(cv::CAP_PROP_POS_FRAMES);
  }
  _captureMutex.unlock();
  m_frameNumber = newFrameNumber;
  Q_EMIT frameNumberChanged(m_frameNumber);

  if (m_playbackState == PausedState)
    extractFrame();
}

void OpenCV_VideoPlayer::extractFrame() {
  _captureMutex.lock();
  _videoCapture >> _frame;
  _captureMutex.unlock();

  if (!_frame.empty() && _frame.rows > 0 && _frame.cols > 0) {
    m_frameNumber++;
    Q_EMIT frameNumberChanged(m_frameNumber);

    // fill in _lastFrameMat
    cv::cvtColor(_frame, _lastFrameMat, cv::COLOR_BGR2RGB);

    // resize and fill in _lastFrameMat
    // cv::cvtColor(_frame, _frame, cv::COLOR_BGR2RGB);
    // if (!_locker) {
    //   cv::resize(_frame, _lastFrameMat,
    //              cv::Size(_frame.size().width / 4, _frame.size().height /
    //              4));
    // }

    Q_EMIT updateRequired();
  }
}

void OpenCV_VideoPlayer::paintFrame(QPainter *painter, OpenCV_Player_ViewPort *viewport) {
  _locker.ref();
  paintFrame(painter, _lastFrameMat, viewport->size().toSize());
  _locker.deref();
}
void OpenCV_VideoPlayer::paintFrame(QPainter *painter, const cv::Mat &mat, const QSize &size) {
  if (mat.empty()) {
    // set black screen if frame is empty
    // painter->fillRect(QRect{0, 0, size.width(), size.height()}, Qt::red);
    return;
  }

  auto _frameImage = QImage((uchar *)mat.data, mat.cols, mat.rows, mat.step,
                            QImage::Format_RGB888);
  painter->drawImage({0, 0, size.width(), size.height()}, _frameImage,
                     _frameImage.rect());
}

OpenCV_VideoPlayer::PlaybackState OpenCV_VideoPlayer::playbackState() const {
  return m_playbackState;
}
void OpenCV_VideoPlayer::setPlaybackState(PlaybackState newPlaybackState) {
  qDebug() << "---" << Q_FUNC_INFO << newPlaybackState;
  if (m_playbackState == newPlaybackState)
    return;

  switch (newPlaybackState) {
  case PlayingState:
    if (!_videoCapture.isOpened()) {
      qWarning("------ Video Capture is not open");
      return;
    }
    if (!isRunning())
      start();
    break;
  case PausedState:
    break;
  case StoppedState:
    _captureMutex.lock();
    _videoCapture.release();
    _captureMutex.unlock();
    break;
  }

  m_playbackState = newPlaybackState;
  Q_EMIT playbackStateChanged(m_playbackState);
}

void OpenCV_VideoPlayer::run() {
  qDebug() << "---" << Q_FUNC_INFO;
  if (!_videoCapture.isOpened()) {
    qDebug() << "------ Video capture is not open";
    return;
  }

  while (m_playbackState == PlayingState) {
    extractFrame();

    if (m_frameNumber >= m_videoFrameCount) {
      qDebug() << "------ Video ended";
      Q_EMIT videoFinished();
      break;
    }
  }
}
