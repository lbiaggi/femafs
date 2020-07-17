# fema_cmake

### binding fema para python TODO
- [X] Cmake e binding para python
- [ ] OPF no cmake
- [X] Inserir Feature Selection no projeto
- [X] Criar comandos de instalação do projeto
- [X] Criar comandos de instalação do binding de python

Pré-requisitos:
1. Compilar gnu ou clang  7+ (provavelmente 8+)
2. CMake >= 3.14
3. Swig >= 4.0.1
4. Python >= 3.6 [Opcional]

Procedimento Atual:

1. `git clone git@github.com:lbiaggi/fema_cmake.git`
2. `cmake -DBUILD_PYTHON=1 -Bbuild`
3. `cd build && make`

Nota: caso quiser mover o pacote python mover em conjunto com os arquivos `libFEMClassifier.py` `libfem_classifier.so`,`_pyFEMClassifier.so `



### Agradecimentos
[@tcanabrava](https://github.com/tcanabrava) pelo apoio na geração e entendimento de recursos do cmake

