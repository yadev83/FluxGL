# Agents

Guidelines for AI agents working on the FluxGL codebase.

## Projet

FluxGL est un moteur graphique 2D/3D écrit en **C++23** avec **OpenGL** et **GLFW**. Il utilise une architecture ECS, une gestion de scènes, un système de physique 2D, de l'audio (miniaudio), du UI, et un VFS pour les assets.

- **Build system** : CMake (MinGW / Windows)
- **Dépendances** : gérées par vcpkg (`vcpkg.json`), sous-modules dans `vendor/`
- **Structure** :
  - `src/` — code source (ecs, core, graphics, physics, audio, ui, input, math, assets)
  - `include/fluxgl/` — headers publics (même structure que `src/`)
  - `tests/` — tests unitaires (doctest)
  - `examples/` — exemples d'utilisation
  - `scripts/` — scripts de build/config
  - `tools/` — outils (asset packer)
  - `resources/` — ressources (icône, etc.)

## Documents clés

| Document | Chemin |
|----------|--------|
| Roadmap v1 (platformer 2D) | `roadmaps/roadmap-v1.md` |
| Idées long terme | `roadmaps/ideas.md` |
| Changelog | `CHANGELOG.md` |
| Readme | `README.md` |
| Version du package | `vcpkg.json` |

## Tests

- Framework : **doctest** (via vcpkg)
- Fichiers : `tests/test_*.cpp`
- Le cœur du moteur (ECS, math, physique, UI, input, scene, VFS) est testable **sans OpenGL** via la cible statique `fluxgl_core`
- **Pour configurer et lancer les tests :**

```bash
# Configuration (depuis la racine)
bash scripts/configure-tests.sh

# Build
cmake --build build/tests

# Exécution
./build/tests/fluxgl_tests
```

- CTest est aussi disponible (un cas CTest par test doctest) :
```bash
cd build/tests && ctest
```

## Règles pour les agents

### 1. Toujours mettre à jour le changelog et la roadmap

Quand une modification fait avancer le projet (nouvelle feature, fix, refactor significatif) :

- **`CHANGELOG.md`** : ajouter une entrée dans la section `[Unreleased]` sous la catégorie appropriée (`Added`, `Changed`, `Fixed`). Suivre le format [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) et [Semantic Versioning](https://semver.org/spec/v2.0.0.html).
- **`roadmaps/`** : cocher `[x]` l'item correspondant dans la roadmap active (actuellement `roadmaps/roadmap-v1.md`) si la feature est implémentée, ou ajouter un nouvel item si nécessaire.

### 2. Toujours faire tourner les tests

Avant de valider toute modification :

```bash
bash scripts/configure-tests.sh && cmake --build build/tests && ./build/tests/fluxgl_tests
```

Toute régression détectée doit être corrigée avant de considérer la tâche terminée.

### 3. Mettre à jour les tests en cas de nouvelle feature core testable

Si une nouvelle feature touche le cœur du moteur (ECS, math, physique, UI, input, scene, VFS) :

- Ajouter des tests dans le fichier `tests/test_*.cpp` correspondant (ou en créer un nouveau si besoin)
- Le test doit couvrir les cas principaux et les cas limites
- Ne pas tester le code dépendant de GL (cible `fluxgl_core` uniquement pour les tests)

### 4. Commenter le code de manière récurrente

- **Commenter les snippets complexes** : algorithmes non évidents, logique physique, ECS patterns, branches tricky
- **Ne pas sur-commenter** : le code auto-explicatif (noms de variables/clairs, fonctions courtes) n'a pas besoin de commentaires
- **Style** : commentaires en anglais, concis, expliquer le *pourquoi* pas le *quoi*

### 5. Les idées intéressantes sont à noter

Si une idée émerge mais qu'elle est hors-scope ou qu'on décide de la remettre à plus tard, la consigner dans le fichier `roadmaps/ideas.md` prévu à cet effet. Il sera ensuite dispatché sur des roadmaps plus précises

### 6. Utilisation de git

Créer une branche depuis `/dev` quand on attaque un gros bloc de développement et la nommer correctement (feature/ bug/) selon ce qu'elle envisage de faire. Les hotfix/ partiront exceptionnellement de `/master` et seront mergés à la fois dans dev/ et master/