from conans import CMake, ConanFile
from conans.errors import ConanException
import os


class NaiveTsearchConan(ConanFile):
    name = "naive-tsearch"
    description = "A simple tsearch() implementation for platforms without one"
    topics = ("tsearch", "tfind", "tdelete", "twalk")
    homepage = "https://github.com/kulp/naive-tsearch"
    url = "https://github.com/kulp/naive-tsearch"
    license = "MIT"
    exports_sources = "CMakeLists.txt", "README.md", "*.c", "*.h", "*.in", "*.inc", "LICENSE"
    exports = "LICENSE"
    no_copy_source = True
    settings = "os", "arch", "compiler", "build_type"
    requires = "boost/1.72.0"
    generators = "cmake"

    def set_version(self):
        import re
        m = re.search(r"project\(.*VERSION ([0-9a-zA-Z.-]+)[ )]",
                      open(os.path.join(self.recipe_folder, "CMakeLists.txt")).read())
        if not m:
            raise ConanException("Could not extract version from CMakeLists.txt")
        self.version = m.group(1)

    _cmake = None

    def configure(self):
        del self.settings.compiler.cppstd
        del self.settings.compiler.libcxx

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.definitions["NAIVE_TSEARCH_INSTALL"] = True
        self._cmake.definitions["NAIVE_TSEARCH_TESTS"] = True
        self._cmake.definitions["Boost_USE_STATIC_LIBS"] = not self.options["boost"].shared
        self._cmake.configure()
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["naive-tsearch"]
        self.cpp_info.includedirs.append(os.path.join("include", "naive-tsearch"))
