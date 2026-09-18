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
- Lifetime system : Add a lifetime component with a duration on an entity to make it disappear after the duration is elapsed
- Physics System : Apply basic physics to rigidbodies and adds box colliders based collision detection
- Virtual File System : Currently targetting a disk storage provider, but will be used later for packaging assets
- Added a Asset Packer tool
- Added a Package File provider for the VFS that reads Assets files packed with the dedicated tool
- Added a Facing2D component to track components directions. Has to be updated manually and is drawn by the physics system.
- Added UIText, UITransform and UIRect components to allow for UI render. Positionned using screen pixels coordinates for UIs
- Added UIButton component that detects hover and pressed states of a UITransform/UIContainer. Can be used to trigger actions in scenes and systems
- Added UICollider component with AABB based hit testing. Used by the UISystem to compute hovered/pressed states
- UITransform now supports anchor and pivot points (UIAlignment), and UIText supports auto-scaling (maxWidth/maxHeight) and multiline text
- Added Follow2D component and Follow2DSystem to smoothly follow a target entity with offset, deadzone, smoothing and look-ahead
- Core engine systems are now auto-registered by the SceneManager (RenderSystem, PhysicsSystem, Follow2DSystem, LifetimeSystem, UISystem). Manual registerSystem() calls in scenes are no longer required
- Mouse to world utilities : Ray struct, Camera::screenPointToRay(), Camera::screenToWorldPoint(), Camera::screenToWorldPointOnPlane(), AABB::intersects(Ray) and PhysicsSystem::raycast()/raycastAll() for picking
- Added Renderer::getViewportPosition() and Renderer::screenToViewport() helper methods
- Entities can now be enabled/disabled (Entity::isEnabled()) and tagged (addTag()/hasTag()/removeTag())
- App now loads an icon for the window automatically. It looks for the assets/img/icon.png file by default. Not parameterized yet
- Window getFullscreenMode() and setFullscreenMode(bool) methods added
- Unit tests on the engine core **without GL** using the `doctest` framework (via vcpkg) : static `fluxgl_core` target (ecs, math, core, physics, ui, input) with no GLFW/GLAD dependency, `FLUXGL_BUILD_TESTS` option and CTest discovery (`doctest_discover_tests` → 1 CTest case per doctest test case, 62 cases)

### Changed

- Roadmap déplacée dans `roadmaps/roadmap-v1.md` pour préparer l'accueil de plusieurs roadmaps (une par grande version du moteur)
- Ajout de `roadmaps/ideas.md` pour les idées long terme (ex. doc Doxygen) et lien vers la roadmap/le changelog dans le README
- Lifecycle is now a bit more precise with fixedUpdates and LateUpdates/RenderUpdates
- Registry management has been upgraded with many improvements
  - EntityIDs are now limited to a fixed number (10000) and managed automatically by the system to avoid invalidities
  - Registry can now create hierarchy between entities to (children => parent)
  - A lot of exceptions are now caught for easier debugging
  - Entity removal system and isAlive/isValid checks have been improved
- Scene Manager now properly unloads entities/systems/components/behaviors on a new scene load
- Headers have been reorganized thematically : ecs/components/* and ecs/systems/* were moved to graphics/, physics/, audio/, ui/ and core/. Include paths were updated accordingly. This is a breaking change for existing projects
- The prefab system has been removed (prefab_loader, Scene::loadLevel, App prefab loading)

### Fixed

- Added Orthographic camera view scaling to fix the ortho view.
- OrthoCam worldWidth = 24 units by default. Height is calculated based on the aspect ratio.
- Physics system mtvY sign is now properly used for grounded checks
- Fixed a memory leak on texture::loadFromMemory()
- Fixed BoxCollider2D size computation
- Fixed UICollider pivot conversion
- Fixed UIButton hover/collision detection
- UI rendering now uses viewport dimensions instead of framebuffer dimensions (scales height as well) and fixes UI depth sorting
- Fixed memory leaks in the Registry : component and behavior allocations (new) are now properly deleted on removeComponent, entity destruction and registry clear. Scenes now free their systems on destruction and the SceneManager frees registered scenes at shutdown
- Fixed `Registry::query` returning multi-component entities in duplicate (deduplication with `std::unordered_set`), detected by the new unit tests

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
