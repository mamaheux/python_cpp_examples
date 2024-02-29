#ifndef IMAGE_PROCESSING_IMAGE_PROCESSING_WIDGET_H
#define IMAGE_PROCESSING_IMAGE_PROCESSING_WIDGET_H

#include "ImageDisplayWidget.h"
#include "ImageOperationPluginManager.h"

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

class ImageProcessingWidget : public QWidget
{
    Q_OBJECT

    QImage m_beforeImage;
    ImageOperationPluginManager& m_pluginManager;

public:
    explicit ImageProcessingWidget(ImageOperationPluginManager& pluginManager, QWidget* parent = nullptr);
    ~ImageProcessingWidget() override;

private Q_SLOTS:
    void onBrowseButtonClicked(bool checked);
    void onOperationComboBoxCurrentIndexChanged(int index);

private:
    void updateAfterImage();

private:
    QPushButton* m_browseButton;
    QComboBox* m_operationComboBox;

    ImageDisplayWidget* m_beforeImageDisplayWidget;
    ImageDisplayWidget* m_afterImageDisplayWidget;
};
#endif
