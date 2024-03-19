#include "ImageProcessingWidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>

#include <QDebug>

ImageProcessingWidget::ImageProcessingWidget(ImageOperationPluginManager& pluginManager, QWidget* parent)
    : QWidget(parent),
      m_pluginManager(pluginManager)
{
    m_browseButton = new QPushButton("Browse");
    connect(m_browseButton, &QPushButton::clicked, this, &ImageProcessingWidget::onBrowseButtonClicked);

    m_operationComboBox = new QComboBox();
    m_operationComboBox->addItems(m_pluginManager.operationNames());
    connect(
        m_operationComboBox,
        qOverload<int>(&QComboBox::currentIndexChanged),
        this,
        &ImageProcessingWidget::onOperationComboBoxCurrentIndexChanged);

    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(m_browseButton);
    topLayout->addStretch(1);
    topLayout->addWidget(new QLabel("Operation:"));
    topLayout->addWidget(m_operationComboBox);


    m_beforeImageDisplayWidget = new ImageDisplayWidget;
    m_afterImageDisplayWidget = new ImageDisplayWidget;

    auto bottomLayout = new QGridLayout;
    bottomLayout->addWidget(new QLabel("Before"), 0, 0);
    bottomLayout->addWidget(m_beforeImageDisplayWidget, 1, 0);
    bottomLayout->addWidget(new QLabel("After"), 0, 1);
    bottomLayout->addWidget(m_afterImageDisplayWidget, 1, 1);
    bottomLayout->setRowStretch(0, 0);
    bottomLayout->setRowStretch(1, 1);

    auto globalLayout = new QVBoxLayout;
    globalLayout->addLayout(topLayout);
    globalLayout->addLayout(bottomLayout, 1);

    setLayout(globalLayout);
}

ImageProcessingWidget::~ImageProcessingWidget() {}

void ImageProcessingWidget::onBrowseButtonClicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image File", "", "Images (*.png *.xpm *.jpg)");

    if (fileName.isEmpty())
    {
        return;
    }

    m_beforeImage.load(fileName);
    m_beforeImageDisplayWidget->setImage(m_beforeImage);

    updateAfterImage();
}

void ImageProcessingWidget::onOperationComboBoxCurrentIndexChanged(int index)
{
    updateAfterImage();
}

void ImageProcessingWidget::updateAfterImage()
{
    if (m_beforeImage.isNull())
    {
        m_afterImageDisplayWidget->setImage(QImage());
        return;
    }

    auto afterImage = m_pluginManager.process(m_operationComboBox->currentIndex(), m_beforeImage);
    if (afterImage.has_value())
    {
        m_afterImageDisplayWidget->setImage(*afterImage);
    }
    else
    {
        m_afterImageDisplayWidget->setImage(QImage());
    }
}
