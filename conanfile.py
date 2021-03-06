from conans import CMake, ConanFile
import os


class NaiveTsearchConan(ConanFile):
    name = "naive-tsearch"
    description = "A simple tsearch() implementation for platforms without one"
    topics = ("tsearch", "tfind", "tdelete", "twalk")
    homepage = "https://github.com/kulp/naive-tsearch"
    url = "https://github.com/kulp/naive-tsearch"
    license = "MIT"
    exports_sources = "CMakeLists.txt", "create_release.py", "Changelog.md", "distfiles.yml", "README.md", "NAIVETSEARCH_VERSION", \
                      "*.c", "*.h", "*.in", "*.inc", "LICENSE", "tests/**"
    exports = "LICENSE"
    no_copy_source = True
    settings = "os", "arch", "compiler", "build_type"
    requires = "boost/1.73.0"
    generators = "cmake"
    options = {
        "header_only": [True, False],
    }
    default_options = {
        "header_only": True,
    }

    def set_version(self):
        self.version = open(os.path.join(self.recipe_folder, "NAIVETSEARCH_VERSION")).read().strip()

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

    def package_id(self):
        del self.info.options.header_only

    def package_info(self):
        if self.options.header_only:
            self.cpp_info.libs = ["naive-tsearch"]
        else:
            self.cpp_info.defines = ["NAIVE_TSEARCH_HDRONLY"]
        self.cpp_info.includedirs.append(os.path.join("include", "naive-tsearch"))
