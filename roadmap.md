# FluxGL Roadmap

Objectif à court terme : rendre FluxGL utilisable pour développer un **platformer 2D simple** de bout en bout.

Légende : `[x]` fait — `[ ]` à faire

## Architecture / ECS

- [x] Registre ECS (entités, composants, `query<T...>`, `queryByTag`)
- [x] Hiérarchie parent/enfant (`setParent`, `getChildren`, `getParent`)
- [x] Enable/disable des entités (`Entity::enable/disable/isEnabled`)
- [x] Tags d'entités (`addTag` / `hasTag` / `removeTag`)
- [x] Behaviors (~MonoBehaviour) avec `onInit/onUpdate/onFixedUpdate/onLateUpdate/onRender`
- [x] Systèmes core auto-enregistrés par le `SceneManager` (Render, Physics, Follow2D, Lifetime, UI)
- [x] Cycle de vie fixed timestep + variable dt (update / fixedUpdate / lateUpdate / render)
- [x] Scenes + transitions (`SceneManager::loadScene`, unload propre)
- [x] En-têtes réorganisés thématiquement, bibliothèque compilable standalone
- [ ] **Scene reload + payload** : recharger la scène courante, passer des données entre scènes (GameOver, score, vie)
- [ ] Perf `query()` (archetypes / chunks) si les limites 10k entités sont atteintes

## Graphismes

- [x] Sprites avec layers et UV (`Sprite::layer`, `uvMin/uvMax`)
- [x] Meshes 3D + shaders + textures + matériaux
- [x] Éclairage ambiant / directionnel / ponctuel
- [x] Rendu orthographique et perspective (letterboxing, viewport)
- [x] Debug renderer (lines, rects, crosses)
- [x] UI : `UITransform` (anchor/pivot, `UIAlignment`), `UIRect`, `UIText` (multiligne, autoscale maxWidth/maxHeight)
- [x] UI : `UICollider` + `UIButton` (hover / pressed) gérés par `UISystem`
- [ ] **AnimatedSprite** : clips de spritesheet (durée, boucle, ping-pong) + flip selon le facing, branché sur `Sprite::uvMin/uvMax`
- [ ] Tri de rendu par Y entre sprites (au-delà des layers)

## Input

- [x] Clavier : down / pressed / released
- [x] Souris : boutons, position, scroll
- [x] Fenêtre : plein écran, icône, curseur verrouillé
- [ ] **Couche InputAction** : actions rebindables (Jump / Move / Interact...) au lieu des keycodes GLFW bruts dans le gameplay

## Audio

- [x] Audio (moteur miniaudio, `AudioSource`, listener)
- [ ] **SFX one-shot** : helper `playOneShot(sound, volume)` + vérifier pan/atténuation sur l'audio

## Physique 2D

- [x] `RigidBody2D` (static / dynamic / kinematic, gravité, `grounded`)
- [x] `BoxCollider2D` (size, offset, triggers, layers Hitbox/Hurtbox/Default)
- [x] Résolution de collisions AABB + événements enter / stay / exit
- [x] Raycast 2D : `Ray`, `Camera::screenPointToRay`, `AABB::intersects(Ray)`, `PhysicsSystem::raycast/raycastAll`
- [ ] **CharacterController2D** : composant/système de mouvement plateforme "feelé" (accélération sol/air, vitesse max, friction, saut variable, coyote time, jump buffer)
- [ ] **One-way platforms** : plateformes traversables par dessous, atterrissage par dessus, drop-through
- [ ] **Sweep / CCD optionnel** : résolution move-and-correct axe par axe pour le personnage, évite le tunnelling à haute vitesse
- [ ] **Tilemap** : composant grille (tile size, tileset, UVs) + génération de colliders statiques pour fabriquer des niveaux sans centaines de `createEntity()`

## Caméra

- [x] `Follow2D` (offset, deadzone, smoothing, look-ahead)
- [x] Souris → monde : `Camera::screenToWorldPoint`, `screenToWorldPointOnPlane`
- [ ] **Caméra bornée au niveau** : min/max pour `Follow2D`
- [ ] Interpolation du fixed timestep pour une caméra ultra fluide

## Autres systèmes

- [x] `Lifetime` (destruction d'entité après délai)
- [x] VFS + Asset Packer + `ResourceManager` (handles au lieu de pointeurs)

## Qualité / release

- [x] **Fix des fuites mémoire** : `removeComponent`, `removeEntity` et `clear()` ne `delete` pas les allocations `new` des composants/behaviors → fuites à chaque destruction de scène (restart = leak)
- [ ] Tests unitaires sur le cœur du moteur **sans GL** (framework `doctest` via vcpkg) :
  - [ ] Infra : cible statique `fluxgl_core` (ecs, math, core, physics, ui, input) sans dépendance GLFW/GLAD + option `FLUXGL_BUILD_TESTS` + CTest
  - [ ] ECS : registry (IDs, capacité max, destruction différée, `clear()`), composants (add/get/has/remove, libération mémoire), `query`/tags, behaviors (init unique), hiérarchie
  - [ ] Math : `AABB` (`contains`, `intersects`, raycast, cas perpendiculaires), `Transform` (model matrix, front/right/up)
  - [ ] Physique : gravité (static/dynamic/kinematic), résolution AABB/MTV, événements enter/stay/exit, triggers, skip parent-enfant, `raycast`/`raycastAll`, `LifetimeSystem`, `Follow2DSystem`
  - [ ] Core : `Scene` (serveurs de systèmes), `SceneManager` (transitions, fixed timestep), VFS + `DirectoryStorageProvider`
  - [ ] UI / Input : `UITransform` (anchor/pivot), `InputManager` (pressed/released) — exécutables headless
- [ ] CharacterController2D / One-way platforms / Tilemap : couverts par des tests d'intégration sur la physique une fois implémentés
- [ ] Exemple "plateforme complet" minimal (saut, one-way, animation, tilemap, caméra) servant aussi de documentation vivante
- [ ] Bump de version + date dans `CHANGELOG.md` et `vcpkg.json` pour la prochaine release