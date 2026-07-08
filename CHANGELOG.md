# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Sprite component and basic sprite rendering
- World unit vectors in the transform component
- Debug renderer to draw lines, rectangles, etc... on screen for debugging purposes
- ResourceManager can now be used to store resources for later use. Also makes ECS components lighter since they now hold resource handles instead of raw pointers. It makes initialization and prefabs easier to implement.
- PrefabLoader added to the engine.
  - Prefabs are json assets that can be used to generate complex entities from data instead of code
- Lifetime system : Add a lifetime component with a duration on an entity to make it disappear after the duration is elapsed
- Physics System : Apply basic physics to rigidbodies and adds box colliders based collision detection
- Virtual File System : Currently targetting a disk storage provider, but will be used later for packaging assets
- Added a Asset Packer tool
- Added a Package File provider for the VFS that reads Assets files packed with the dedicated tool
- Added a Facing2D component to track components directions. Has to be updated manually and is drawn by the physics system.
- Added UIText, UITransform and UIRect components to allow for UI render. Positionned using screen pixels coordinates for UIs

### Changed

- Lifecycle is now a bit more precise with fixedUpdates and LateUpdates/RenderUpdates
- Registry management has been upgraded with many improvements
  - EntityIDs are now limited to a fixed number (10000) and managed automatically by the system to avoid invalidities
  - Registry can now create hierarchy between entities to (children => parent)
  - A lot of exceptions are now caught for easier debugging
  - Entity removal system and isAlive/isValid checks have been improved
- Scene Manager now properly unloads entities/systems/components/behaviors on a new scene load

### Fixed

- Added Orthographic camera view scaling to fix the ortho view.
- OrthoCam worldWidth = 24 units by default. Height is calculated based on the aspect ratio.
- Physics system mtvY sign is now properly used for grounded checks

## [0.4.0] - 2026-02-25

### Added

- Added a basic audioEngine (load / play / start) based on miniaudio lib
- Implemented the ECS Audio_System with AudioSource and AudioListener components for audio capabilities

### Fixed

- Fixed a rotation compute error that made the directional lights not work properly

## [0.3.2] - 2026-02-24

### Fixed

- Incorrect defaultLit() shader use of u_DirectionalLightCount

## [0.3.1] - 2026-02-24

### Added

- defaultLit() and defaultUnlit() shaders builders

### Fixed

- Compilation option for examples

## [0.3.0] - 2026-02-24

### Added

- Basic ECS system with a registry, entity, and component system
- SceneManager and Scene classes for scene management
- Systems implementation with the ECS (used only in generic rendering for now)
- Behaviors system for "custom" actions without going through the whole systems/components loop. (Imagine MonoBehaviors from Unity and similar stuff)
- Lighting system with different light types and a heavy shader update (materials pipeline included)

### Changed

- App lifecycle now goes through scenes and App should not be inherited directly
- Letterboxing and aspect ratio respect in rendering system
- Updated the build system to make examples optional and fluxgl buildable as a standalone library for other projects

## [0.2.0] - 2026-02-18

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
- Added a "Transform" component, and a "Renderable" component to hold Transform/Mesh/Material and improve the Renderer pipeline
- Added a "Camera" component to hold a Transform and build projection/view matrices
- Added a "Cube" mesh and a "Sphere" mesh with a "3d" example file to show these off
- Window glfw callbacks and input manager

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
