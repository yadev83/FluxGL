#include <doctest/doctest.h>

#include <fluxgl/math/aabb.h>
#include <fluxgl/math/ray.h>
#include <fluxgl/core/transform.h>
#include <fluxgl/physics/box_collider.h>

TEST_CASE("AABB: contains") {
    fluxgl::AABB box;
    box.min = {-1.0f, -1.0f};
    box.max = { 1.0f,  1.0f};

    CHECK(box.contains(glm::vec2(0.0f, 0.0f)));
    CHECK(box.contains(glm::vec2(1.0f, -1.0f))); // edge
    CHECK_FALSE(box.contains(glm::vec2(1.5f, 0.0f)));
    CHECK_FALSE(box.contains(glm::vec2(0.0f, -2.0f)));
}

TEST_CASE("AABB: intersects AABB") {
    fluxgl::AABB a, b;
    a.min = {-1.0f, -1.0f};
    a.max = { 1.0f,  1.0f};

    b.min = { 0.5f,  0.5f};
    b.max = { 2.0f,  2.0f};
    CHECK(a.intersects(b));

    b.min = { 1.0f,  1.0f};
    b.max = { 2.0f,  2.0f};
    CHECK(a.intersects(b)); // touching edges count

    b.min = { 1.1f,  1.1f};
    b.max = { 2.0f,  2.0f};
    CHECK_FALSE(a.intersects(b));

    // fully inside counts
    fluxgl::AABB inner;
    inner.min = {-0.2f, -0.2f};
    inner.max = { 0.2f,  0.2f};
    CHECK(a.intersects(inner));
}

TEST_CASE("AABB: ray intersection in the plane") {
    fluxgl::AABB box;
    box.min = {-1.0f, -1.0f};
    box.max = { 1.0f,  1.0f};

    fluxgl::Ray ray;
    ray.origin = {5.0f, 0.0f, 0.0f};
    ray.direction = {-1.0f, 0.0f, 0.0f};

    float t = -1.0f;
    CHECK(box.intersects(ray, t));
    CHECK(t == doctest::Approx(4.0f));

    // Ray pointing away -> no hit
    ray.direction = {1.0f, 0.0f, 0.0f};
    CHECK_FALSE(box.intersects(ray, t));

    // Ray missing the box vertically
    ray.origin = {0.0f, 5.0f, 0.0f};
    ray.direction = {0.0f, -1.0f, 0.0f};
    t = -1.0f;
    CHECK(box.intersects(ray, t));
    CHECK(t == doctest::Approx(4.0f));

    ray.origin = {0.0f, -5.0f, 0.0f};
    ray.direction = {0.0f, 1.0f, 0.0f};
    t = -1.0f;
    CHECK(box.intersects(ray, t));
    CHECK(t == doctest::Approx(4.0f));
}

TEST_CASE("AABB: orthographic ray perpendicular to the XY plane") {
    fluxgl::AABB box;
    box.min = {-1.0f, -1.0f};
    box.max = { 1.0f,  1.0f};

    fluxgl::Ray ray;
    ray.origin = {0.0f, 0.0f, 5.0f};
    ray.direction = {0.0f, 0.0f, -1.0f};

    float t = -1.0f;
    CHECK(box.intersects(ray, t));
    CHECK(t == doctest::Approx(5.0f));

    // Outside the box footprint -> no hit
    ray.origin = {3.0f, 0.0f, 5.0f};
    CHECK_FALSE(box.intersects(ray, t));
}

TEST_CASE("AABB: origin inside the box reports the exit distance") {
    fluxgl::AABB box;
    box.min = {-1.0f, -1.0f};
    box.max = { 1.0f,  1.0f};

    fluxgl::Ray ray;
    ray.origin = {0.0f, 0.0f, 0.0f};
    ray.direction = {1.0f, 0.0f, 0.0f};

    float t = -1.0f;
    CHECK(box.intersects(ray, t));
    CHECK(t == doctest::Approx(1.0f));
}

TEST_CASE("Transform: model matrix applies translation") {
    fluxgl::Transform t;
    t.position = {2.0f, 3.0f, 4.0f};

    glm::vec4 origin = t.getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    CHECK(origin.x == doctest::Approx(2.0f));
    CHECK(origin.y == doctest::Approx(3.0f));
    CHECK(origin.z == doctest::Approx(4.0f));
}

TEST_CASE("Transform: scaled model matrix") {
    fluxgl::Transform t;
    t.scale = {2.0f, 3.0f, 4.0f};

    glm::vec4 unit = t.getModelMatrix() * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    CHECK(unit.x == doctest::Approx(2.0f));
    CHECK(unit.y == doctest::Approx(3.0f));
    CHECK(unit.z == doctest::Approx(4.0f));
}

TEST_CASE("Transform: identity basis vectors") {
    fluxgl::Transform t;
    CHECK(t.front().x == doctest::Approx(0.0f));
    CHECK(t.front().z == doctest::Approx(-1.0f));
    CHECK(t.right().x == doctest::Approx(1.0f));
    CHECK(t.up().y == doctest::Approx(1.0f));
}

TEST_CASE("BoxCollider2D: AABB from transform and offset") {
    fluxgl::BoxCollider2D collider;
    collider.size = {2.0f, 2.0f};
    collider.offset = {0.5f, 0.0f};

    fluxgl::Transform t;
    t.position = {1.0f, 2.0f, 0.0f};

    auto aabb = collider.getAABB(t);
    CHECK(aabb.min.x == doctest::Approx(0.5f));
    CHECK(aabb.min.y == doctest::Approx(1.0f));
    CHECK(aabb.max.x == doctest::Approx(2.5f));
    CHECK(aabb.max.y == doctest::Approx(3.0f));
}

TEST_CASE("BoxCollider2D: collision layer rules") {
    using fluxgl::CollisionLayer;
    fluxgl::BoxCollider2D dc; dc.layer = CollisionLayer::Default;
    fluxgl::BoxCollider2D hi; hi.layer = CollisionLayer::Hitbox;
    fluxgl::BoxCollider2D hu; hu.layer = CollisionLayer::Hurtbox;

    CHECK(dc.shouldCollide(dc));
    CHECK(dc.shouldCollide(hi));
    CHECK(hi.shouldCollide(dc));
    CHECK(dc.shouldCollide(hu));

    CHECK(hi.shouldCollide(hu));
    CHECK(hu.shouldCollide(hi));

    CHECK_FALSE(hi.shouldCollide(hi));
    CHECK_FALSE(hu.shouldCollide(hu));
}