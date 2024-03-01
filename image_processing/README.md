# image_processing
Ce projet montre comment créer une application Qt qui utilise des plugins Python afin d'appliquer des opérations sur des images

Pour exécuter l'application, il suffit d'exécuter les commandes suivants :
```bash
mkdir build
cd build
cmake ..
make
./image_processing
```

Les plugins se trouvent dans le dossier [image_processing_plugins](image_processing_plugins).
Chaque fichier Python dans ce dossier correspond à un plugin.
Un fichier de plugin doit avoir une fonction `name` et une fonction `process.`
La première fonction retourne une chaine de caractère contenant le nom de l'opération, tandis que la deuxième transforme une image BRG.
Le résultat de cette transformation doit aussi être une image BGR.

Le chargement des plugins se fait dans la classe [ImageOperationPluginManager](src/ImageOperationPluginManager.cpp).

Dans des cas plus complexes, un plugin Python pourrait créer une [nouvelle classe héritant d'une classe de base définie en C++](https://pybind11.readthedocs.io/en/stable/advanced/classes.html#overriding-virtual-functions-in-python) afin de la rendre accessible dans l'application C++.
