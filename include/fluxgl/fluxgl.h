#pragma once

#include "core/app.h"
#include "core/window.h"
#include "core/error.h"
#include "core/log.h"
#include "core/scene.h"
#include "core/scene_manager.h"

#include "ecs/entity.h"
#include "ecs/registry.h"
#include "ecs/system.h"
#include "ecs/behavior.h"
#include "ecs/systems/render_system.h"
#include "ecs/components/transform.h"
#include "ecs/components/mesh_renderer.h"
#include "ecs/components/camera.h"
#include "ecs/components/light.h"

#include "utils/file.h"
#include "utils/compute.h"

#include "input/input_manager.h"

#include "graphics/vertex.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/material.h"
#include "graphics/renderer.h"