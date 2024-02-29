#ifndef IMAGE_PROCESSING_IMAGE_DISPLAY_WIDGET_H
#define IMAGE_PROCESSING_IMAGE_DISPLAY_WIDGET_H

#include <QWidget>

// Inspired by https://github.com/introlab/opentera-webrtc-ros/blob/main/opentera_webrtc_robot_gui/src/ROSCameraView.h
class ImageDisplayWidget : public QWidget
{
    Q_OBJECT

    QImage m_image;

public:
    explicit ImageDisplayWidget(QWidget* parent = nullptr);

public Q_SLOTS:
    void setImage(const QImage& image);

protected:
    void paintEvent(QPaintEvent* event) override;
};

inline void ImageDisplayWidget::setImage(const QImage& image)
{
    m_image = image.copy();
    update();
}

#endif
