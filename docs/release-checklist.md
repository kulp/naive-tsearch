# Release checklist
Lists of steps one should perform prior to creating a new release.
In total 2 commits should be made: one for the release, and one for the post-release.

1. Remove all `build*` subdirectories
2. Make sure you're on the current master and that the tree is in pristine condition using `git status`.
Run `git reset --hard HEAD` and `git clean -x -f` when unsure.
3. Bump the version in `NAIVETSEARCH_VERSION` (found in the root directory) and rename the `[Unreleased]` section of the `Changelog.md` to the version used in `NAIVETSEARCH_VERSION`. Use the format `[x.y.z] - YYY-MM-DD` as name for the new release section.
  Remove empty added/changed/removed subsections.
  Update URLs at the bottom of the file.
  Read [keep a changelog](https://keepachangelog.com/) for the conventions.
  **Don't commit yet.**
4. Run the following commands to test the repository:
    ```
    rm -rf build_cmake
    mkdir build_cmake && cd build_cmake
    cmake ..
    cmake --build .
    ctest .
    cd ..
    rm -rf build_conan
    mkdir build_conan && cd build_conan
    python -m conans.conan install ..
    python -m conans.conan build ..
    ctest .
    cd ..
    ```
5. Create source tarballs to test. These are non-final.
    ```
    python create_source_tarball.py
    ```
6. Check whether the name of the tarball is correct. It should be `naive-tsearch-X.Y.Z.tar.XX`
7. Extract the source tarball and repeat all tests of step 5 (or skip step 4 and do the tests here instead)
8. Remove the tarballs. This avoid submitting accidental upload of pre-release tarballs.
9. Only `Changelog.md` and `NAIVETSEARCH_VERSION` should have been modified.
10. Add both files to git, and create a new commit named `Release X.Y.Z`
11. Tag the commit using the following name template `vX.Y.Z` (semantic versioning)
12. Create the final source tarballs:
    ```
    python create_source_tarball.py
    ```
13. Double check everything. Ask when unsure.
14. Push the commits + tag.
15. Upload the source tarballs to the release page.
16. Add a new section `[Unreleased]` to the `Changelog.md`, including empty `Added/Changed/Removed` subsections.
17. A new development cycle begins...
