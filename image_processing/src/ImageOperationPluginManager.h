#ifndef IMAGE_PROCESSING_IMAGE_OPERATION_PLUGIN_MANAGER_H
#define IMAGE_PROCESSING_IMAGE_OPERATION_PLUGIN_MANAGER_H

#include <QStringList>
#include <QImage>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>
#include <pybind11/embed.h>

#include <functional>
#include <optional>


class ImageOperationPluginManager
{
    pybind11::module m_sys;

    QStringList m_operationNames;

    using ProcessFunction = std::function<pybind11::array_t<uint8_t>(pybind11::array_t<uint8_t>)>;
    QList<ProcessFunction> m_operationFunctions;

public:
    ImageOperationPluginManager();

    ImageOperationPluginManager(const ImageOperationPluginManager&) = delete;
    ImageOperationPluginManager& operator=(const ImageOperationPluginManager&) = delete;

    ImageOperationPluginManager(ImageOperationPluginManager&&) = delete;
    ImageOperationPluginManager& operator=(ImageOperationPluginManager&&) = delete;

    QStringList operationNames() const;
    std::optional<QImage> process(int i, const QImage& image);

private:
    static pybind11::array_t<uint8_t> qImageToPyArray(QImage& bgrImage);
    static QImage pyArrayToQImageCopy(const pybind11::array_t<uint8_t>& image);
};

#endif
