#include "ImageOperationPluginManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

namespace py = pybind11;

constexpr const char* PLUGIN_FOLDER = "image_processing_plugins";

ImageOperationPluginManager::ImageOperationPluginManager()
{
    QDir pluginPath = QCoreApplication::applicationDirPath() + QDir::separator() + PLUGIN_FOLDER;
    if (!pluginPath.exists())
    {
        qWarning() << "The \"image_processing_plugins\" does not exist (" << pluginPath << ").";
        return;
    }

    m_sys = py::module::import("sys");
    qDebug() << "Using Python" << m_sys.attr("version").cast<std::string>().c_str();

    m_sys.attr("path").attr("append")(QCoreApplication::applicationDirPath().toStdString());

    for (auto& pluginFile : pluginPath.entryInfoList({"*.py"}, QDir::Files))
    {
        if (pluginFile.baseName() == "__init__")
        {
            continue;
        }

        try
        {
            std::string moduleName = std::string(PLUGIN_FOLDER) + "." + pluginFile.baseName().toStdString();
            py::module operationModule = py::module::import(moduleName.c_str());

            m_operationNames.append(operationModule.attr("name")().cast<std::string>().c_str());
            m_operationFunctions.append(operationModule.attr("process").cast<ProcessFunction>());
        }
        catch (py::error_already_set& e)
        {
            qWarning() << "Error loading " << pluginFile.path() << ": " << e.what();
        }
    }
}

const QStringList& ImageOperationPluginManager::operationNames() const
{
    return m_operationNames;
}

std::optional<QImage> ImageOperationPluginManager::process(int i, const QImage& image)
{
    if (i >= m_operationFunctions.size())
    {
        qWarning() << "Invalid process function index";
        return std::nullopt;
    }

    QImage bgrImage = image.convertToFormat(QImage::Format_RGB888).rgbSwapped();

    try
    {
        auto inputArray = qImageToPyArray(bgrImage);
        auto outputArray = m_operationFunctions[i](inputArray).cast<pybind11::array_t<uint8_t>>();
        return pyArrayToQImageCopy(outputArray);
    }
    catch (py::error_already_set& e)
    {
        qWarning() << "Image processing error: " << e.what();
        return std::nullopt;
    }
}

py::array_t<uint8_t> ImageOperationPluginManager::qImageToPyArray(QImage& bgrImage)
{
    py::buffer_info bufferInfo(
        bgrImage.bits(),
        sizeof(uint8_t),
        py::format_descriptor<uint8_t>::format(),
        3,  // Number of dimensions
        {bgrImage.height(), bgrImage.width(), 3},  // Buffer dimensions
        // Strides (in bytes) for each index
        {sizeof(uint8_t) * 3 * bgrImage.width(), sizeof(uint8_t) * 3, sizeof(uint8_t)});

    return py::array_t<uint8_t>(bufferInfo);
}

QImage ImageOperationPluginManager::pyArrayToQImageCopy(const pybind11::array_t<uint8_t>& bgrImage)
{
    if (bgrImage.ndim() != 3)
    {
        qWarning() << "The output array must have 3 dims (" << bgrImage.ndim() << ").";
        return QImage();
    }
    if (bgrImage.shape(2) != 3)
    {
        qWarning() << "The third dim shape must be 3.";
        return QImage();
    }

    return QImage(
               bgrImage.data(),
               static_cast<int>(bgrImage.shape(1)),
               static_cast<int>(bgrImage.shape(0)),
               QImage::Format_RGB888)
        .copy()
        .rgbSwapped();
}
