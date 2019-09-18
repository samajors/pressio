
# Pressio

Pressio is a collection of projects providing reduced-order models (ROMs) capabilties.

## License
Pressio is released with the following [LICENSE](./LICENSE).

## Cloning
To clone this repo, use this command:
```
git clone --recursive https://github.com/Pressio/pressio.git
```
The recursive option is necessary to clone a git submodule for TriBITS.
TriBITS (https://tribits.org/) provides the development framework for Pressio.

## Configuring and Building
Configuring and building Pressio can be done in two ways: 

* by clonig and using the following helper repo: `git clone https://github.com/Pressio/pressio-builder.git`

* since Pressio uses TriBITS (which uses CMake), you can use a typical CMake configure/build/install process. Note that some TPLs are needed. 

The advantage of using the helper repo is that it automates the installation of TPLs. 

This document is in progress, more detailed info to come soon.
