# Release checklist
Lists of steps one should perform prior to creating a new release.

1. Make sure you're on the current master and that the tree is in pristine condition.
Run `git reset --hard HEAD` when unsure.
2. Remove all `build*` subdirectories
3. Rename the `[Unreleased]` section of the `Changelog` to the version currently in `NAIVETSEARCH_VERSION`.
4. Run the following commands to test the repository:
    ```
    mkdir build_cmake && cd build_cmake
    python -m conans.main install ..
    python -m conans.main build ..
    ctest .
    cd ..
    mkdir build_conan && cd build_conan
    conan install ..
    conan build ..
    ctest .
    cd ..
    ```
5. Create source tarballs to test. These are non-final.
    ```
    python create_source_tarball.py
    ```
6. Check whether the name of the tarball is correct. It should be `naive-tsearch-X.Y.Z.tar.XX`
7. Extract the source tarball and repeat all tests of step 5 (or skip step 5 and do the tests here instead)
8. Remove the tarballs. This avoid submitting accidental upload of pre-release tarballs.
9. Only `NAIVETSEARCH_VERSION` and `Changelog` should have been modified.
10. Add both files to git, and create a new commit named `Release X.Y.Z`
11. Tag the commit using the following name template `vX.Y.Z` (semantic versioning)
12. Create the final source tarballs:
    ```
    python create_source_tarball.py
    ```
13. Double check everything. Ask when unsure.
14. Push the commits + tag.
15. Upload the source tarballs to the release page.
16. Bump the version in the `NAIVETSEARCH_VERSION` file. Remember to use semantic versioning.
17. Add a new section `[Unreleased]` to the `Changelog`, including `Added/Changed/Removed` subsections.
18. A new development cycle begins...
