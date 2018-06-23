from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class DiConan(ConanFile):
    name        = "di"
    version     = "1.2.1"                     
    description = "Conan package for di."           
    url         = "https://github.com/acdemiralp/di"
    license     = "MIT"                         
    settings    = "arch", "build_type", "compiler", "os"

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)
        
    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
