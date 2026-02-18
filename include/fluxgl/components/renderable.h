#pragma once

#include <fluxgl/components/transform.h>
#include <fluxgl/graphics/material.h>
#include <fluxgl/graphics/mesh.h>

namespace fluxgl {
    struct Renderable {
        Transform transform;
        Material material;
        Mesh mesh;
    };
}