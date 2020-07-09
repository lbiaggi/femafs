import setuptools.command.install
import shutil
from distutils.sysconfig import get_python_lib


class CompiledLibInstall(setuptools.command.install.install):
    """
    Specialized install to install to python libs
    """

    def run(self):
        """
        Run method called by setup
        :return:
        """
        # Get filenames from CMake variable
        filenames = '/home/lbiaggi/projects/papers/fema/build/libFemClassifier/swig/python/libFEMClassifier.py;/home/lbiaggi/projects/papers/fema/build/libFemClassifier/swig/python/libfem_classifier.so'.split(';')

        # Directory to install to
        install_dir = get_python_lib()

        # Install files
        [shutil.copy(filename, install_dir) for filename in filenames]


if __name__ == '__main__':
    setuptools.setup(
        name='libFEMClassfier',
        version='1.0.0-dev',
        packages=['libFEMClassifier'],
        license='Apache License 2.0',
        author='Lucas Biaggi',
        author_email='lbjanuario@gmail.com',
        cmdclass={'install': CompiledLibInstall}
    )