import os
import shutil

from conans import ConanFile
from conans.tools import download, unzip


class GccConan(ConanFile):
    name = "gcc"
    with open(os.path.join(os.path.dirname(os.path.realpath(
            __file__)), "VERSION.txt"), 'r') as version_file:
        version = version_file.read()
    settings = {"os": ["Macos"], "arch": ["x86_64"]}
    exports = "VERSION.txt"
    generators = "cmake", "virtualenv"
    url = "https://github.com/kapilsh/conan-packages"
    license = "https://github.com/gcc-mirror/gcc/blob/master/COPYING"

    def source(self):
        zip_name = "gcc-{}-bin.tar.gz".format(self.version)
        download("http://prdownloads.sourceforge.net/hpc/{}".format(
            zip_name), zip_name)
        unzip(zip_name)
        os.unlink(zip_name)
        shutil.move("usr/local".format(self.version), "gcc")
        shutil.rmtree("usr")

    def config(self):
        self.settings.remove("build_type")
        self.settings.remove("compiler")

    def build(self):
        return

    def package(self):
        self.copy('*', src="gcc", dst="")

    def package_info(self):
        self.env_info.DYLD_LIBRARY_PATH.append(os.path.join(
            self.package_folder, "lib"))
        self.env_info.CPATH.append(os.path.join(
            self.package_folder, "include"))
        self.env_info.LIBRARY_PATH.append(os.path.join(
            self.package_folder, "lib"))

        self.env_info.PATH.append(os.path.join(self.package_folder, 'bin'))
        self.env_info.CC = os.path.join(self.package_folder, 'bin', 'gcc')
        self.env_info.CXX = os.path.join(self.package_folder, 'bin', 'g++')

        self.cpp_info.libdirs = ['lib']