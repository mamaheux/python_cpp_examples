#include "ImageProcessingWidget.h"
#include "ImageOperationPluginManager.h"

#include <QApplication>

#include <pybind11/embed.h>

namespace py = pybind11;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    py::scoped_interpreter guard{};
    ImageOperationPluginManager pluginManager;

    ImageProcessingWidget w(pluginManager);
    w.show();
    return a.exec();
}
