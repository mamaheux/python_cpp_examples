# python_cpp_examples

Ce projet illustre comment jumeler le C++ et le Python. Deux cas d'utilisation sont présentés :
1. [Création d'une bibliothèque Python à partir de code C++](mamaheux_signal_processing_cpp);
2. [Création de plugins en Python dans une application C++](image_processing).

Pour les deux cas d'utilisation, [pybind11](https://github.com/pybind/pybind11) est utilisé.
Les projets ont été testés sur Ubuntu 20.04 et nécessitent l'installation des dépendances suivantes.

```bash
sudo apt install pybind11-dev python3-numpy python3-numba python3-matplotlib python3-opencv qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```
