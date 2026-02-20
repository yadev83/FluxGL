#pragma once

#include <fluxgl/graphics/material.h>
#include <fluxgl/graphics/mesh.h>

namespace fluxgl {
    struct MeshRenderer {
        Material material;
        Mesh mesh;
    };
}