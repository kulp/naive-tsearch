# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

(nothing)

### Changed
- Make .tar.gz release tarballs reproducible by zero'ing the time stamp

### Removed

(nothing)

## [0.1.1] - 2020-08-23

### Changed

- Fixed undefined behavior in `tdelete` (#10)

## [0.1.0] - 2020-07-22

### Added

- Initial import based on gist at github (https://gist.github.com/kulp/3971591)
- Add CMake build system
- Add initial unit tests using Boost.Test
- Use conan (conan.io) optionally for providing dependencies
- Fix MSVC compile warnings about /W4 overriding /W3

-------
[0.1.1]: https://github.com/kulp/naive-tsearch/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/kulp/naive-tsearch/releases/tag/v0.1.0

