# fema_cmake

### binding fema para python TODO
- [X] Cmake e binding para python
- [ ] OPF no cmake
- [X] Inserir Feature Selection no projeto <- Em andamento
- [ ] Criar comandos de instalação do projeto
- [ ] Criar comandos de instalação do binding de python

Pré-requisitos:
1. Compilar gnu ou clang  7+ (provavelmente 8+)
2. CMake >= 3.14

Procedimento Atual:

1. `git clone git@github.com:lbiaggi/fema_cmake.git`
2. `cmake -DBUILD_PYTHON=1 -Bbuild`
3. `cd build && make`
4. `cp libFemClassifier/swig/python/libFEMClassifier.py .`

Nota: caso quiser mover o pacote python mover em conjunto com os arquivos `libfem_classifier.so` e `_pyFEMClassifier.so`



### Agradecimentos
[@tcanabrava](https://github.com/tcanabrava) pelo apoio na geração e entendimento de recursos do cmake

