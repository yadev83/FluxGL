# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Minimal logging system with debug, info, warning, and error levels
- Mesh class with vertex and index buffers
- Shader class with vertex and fragment shaders
- File reading system for assets (to string only for now)
- Renderer class for meshes with shaders while still decoupling stuff
- Texture class for loading images as textures
- Material struct used by a renderer to draw meshes (geometry) with a given material (shader, texture, color...)
    - Keep in mind : there is still a fair amount of coupling between shaders and renderer/materials. the API will evolve over time
- Addded an "App" class to wrap the fluxgl functionnality (and updated examples to use this)

## [0.1.1] - 2026-02-11

### Fixed
- Fixed a bug that caused the program to crash on startup

## [0.1.0] - 2026-02-11

Initial development release. Contains the basics needed to build/run the library on Windows.

### Added
- Readme Template
- Changelog Template
- Build/Config Scripts
- vcpkg integration via git submodules
- Basic window creation
- Basic Error Handling and debug logging
- Hello World example program
