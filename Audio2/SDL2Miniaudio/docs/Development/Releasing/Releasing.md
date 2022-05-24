# Releasing Steps
1) Compile the windows project in Release configuration
2) Compile the adnroid project with ARM platform in Release configuration
3) Specify the current version according to semantic versioning: https://semver.org/
4) Create a release-brach: feature/my-version-number
5) Tag the release-branch
6) Adjust the release number in $pack\CreateRelease.bat to match the above version number
7) Create package with $pack\CreateRelease.bat, but do not commit
8) Grab the package under $pack\my-version-number and release it on github
