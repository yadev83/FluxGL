#include <fluxgl/math/aabb.h>
#include <algorithm>

namespace fluxgl {
    bool AABB::contains(const glm::vec2 point) const {
        return 
            point.x >= min.x &&
            point.x <= max.x &&
            point.y >= min.y &&
            point.y <= max.y;
    }

    bool AABB::intersects(const AABB& other) const {
        return
            min.x <= other.max.x &&
            max.x >= other.min.x &&
            min.y <= other.max.y &&
            max.y >= other.min.y;
    }

    // Intersection rayon/AABB par la méthode des slabs, restreinte à X/Y :
    // le rayon peut être 3D (caméra perspective ou ortho), mais la boîte
    // n'existe que dans le plan XY, donc on ignore la composante Z du rayon.
    // tHit reçoit la distance le long du rayon jusqu'au point de contact.
    bool AABB::intersects(const Ray& other, float& tHit) const {
        // Une direction est traitée comme "perpendiculaire" au plan XY dès que sa composante
        // est négligeable devant Z. La tolérance est relative et largement au-dessus du bruit
        // de précision flottante de cos/sin sur les angles (ex: cos(radians(-90°)) ~ -4e-8),
        // pour que la branche générique des slabs ne soit pas déclenchée à tort.
        const float perpThreshold = 1e-4f;
        bool perpX = std::abs(other.direction.x) <= std::abs(other.direction.z) * perpThreshold;
        bool perpY = std::abs(other.direction.y) <= std::abs(other.direction.z) * perpThreshold;

        float tMin = -std::numeric_limits<float>::infinity();
        float tMax =  std::numeric_limits<float>::infinity();

        // Slab X
        if (perpX) {
            if (other.origin.x < min.x || other.origin.x > max.x) return false;
        } else {
            float t1 = (min.x - other.origin.x) / other.direction.x;
            float t2 = (max.x - other.origin.x) / other.direction.x;
            if (t1 > t2) std::swap(t1, t2);
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);
            if (tMin > tMax) return false;
        }

        // Slab Y
        if (perpY) {
            if (other.origin.y < min.y || other.origin.y > max.y) return false;
        } else {
            float t1 = (min.y - other.origin.y) / other.direction.y;
            float t2 = (max.y - other.origin.y) / other.direction.y;
            if (t1 > t2) std::swap(t1, t2);
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);
            if (tMin > tMax) return false;
        }

        if (tMax < 0.0f) return false;

        // Cas d'un rayon perpendiculaire au plan des AABB (caméra orthographique) :
        // les slabs X/Y ne produisent aucun intervalle de t exploitable puisqu'il n'y a pas
        // de déplacement dans le plan. Les branches ci-dessus ont déjà vérifié que le point
        // est dans la boîte ; on renseigne alors t comme la distance le long du rayon jusqu'au
        // plan monde z = 0, afin que le filtrage "plus proche" de raycast reste valable.
        if (perpX && perpY) {
            if (std::abs(other.direction.z) < 1e-8f) return false;
            tHit = -other.origin.z / other.direction.z;
            return tHit >= 0.0f;
        }

        tHit = (tMin >= 0.0f) ? tMin : tMax; // origine à l'intérieur de la boîte -> tMin négatif

        return true;
    }
}