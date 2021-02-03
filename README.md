# fema_cmake

### TODO
- [X] Cmake e binding para python
- [ ] OPF no cmake
- [X] Inserir Feature Selection no projeto
- [X] Criar comandos de instalação do projeto
- [X] Criar comandos de instalação do binding de python

Pré-requisitos:
1. Compilar gnu ou clang  7+ (provavelmente 8+) com suporte ao OPENMP
2. CMake >= 3.14
3. [OPF](https://github.com/lbiaggi/LibOPF) NOTA: COMPILE E COLOQUE O bin no $PATH do shell. fork usado no teste.
3. Swig >= 4.0.1 [Opcional]
4. Python >= 3.6 [Opcional]
Nota: caso quiser mover o pacote python mover em conjunto com os arquivos `libFEMClassifier.py` `libfem_classifier.so`,`_pyFEMClassifier.so `

Procedimento Atual para utilização:

1. `git clone git@github.com:lbiaggi/fema_cmake.git`
2. `cmake -H. -Bbuild`
3. `cd build && make`
3. `make nslkdd (dataset NSL-KDD) ou make tornontor (dataset ICSXTOR-2016)`
4. `Resultados esterão em diretórios de fácil identificação no diretório build/test/`





### Agradecimentos
[@tcanabrava](https://github.com/tcanabrava) pelo apoio na geração e entendimento de recursos do cmake

