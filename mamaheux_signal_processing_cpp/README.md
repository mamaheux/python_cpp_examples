# mamaheux_signal_processing_cpp

Cette bibliothèque Python programmée en C++ permet de réaliser des tâches simples de traitement de signal .

Pour l'installer, il suffit d'exécuter la commande suivante dans le présent répertoire.
```bash
python3 -m pip install .
```

Ensuite, il est possible d'exécuter les exemples présents dans le dossier [python_examples](python_examples).
- [bench_signal_statistics.py](python_examples/bench_signal_statistics.py) compare les temps d'exécution entre le Python, NumPy, le C++ et le C++ avec AVX2 pour trouver le minimum, le maximum et la moyenne d'un signal.
- [compressor_effect.py](python_examples/compressor_effect.py) montre le résultat de la sortie de l'effet audio compresseur.
- [image_histogram.py](python_examples/image_histogram.py) affiche l'histogramme d'une image aléatoire en niveaux de gris.

Voici les cas d'utilisation de pybind11:
- [Classe](src/SignalStatistics.cpp#L159-L172)
- [Utilisation de la STL](src/SignalStatistics.cpp#L191-L196)
- [Paramètres avec valeur par défaut](src/Effects.cpp#L66-L67)

Pour un projet plus complexe, il est recommandé de faire une bibliothèque utilisable en C++ qui est couvert par des tests automatisés.
Ensuite, il est possible de faire des bindings Python qui utilise cette bibliothèque C++. Ainsi, il est possible d'utiliser le code en C++ et en Python.
[OpenTera - WebRTC](https://github.com/introlab/opentera-webrtc/tree/main/opentera-webrtc-native-client/OpenteraWebrtcNativeClient) est une bibliothèque suivant cette recommandation.
