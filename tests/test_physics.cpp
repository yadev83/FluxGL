#include <doctest/doctest.h>

#include <fluxgl/core/scene.h>
#include <fluxgl/core/transform.h>
#include <fluxgl/math/ray.h>

#include <fluxgl/physics/physics_system.h>
#include <fluxgl/physics/rigidbody.h>
#include <fluxgl/physics/box_collider.h>
#include <fluxgl/physics/lifetime.h>
#include <fluxgl/physics/lifetime_system.h>

namespace {
    fluxgl::Entity makeGround(fluxgl::Scene& scene) {
        auto ground = scene.createEntity();
        auto& t = ground.addComponent<fluxgl::Transform>();
        t.position = {0.0f, -0.5f, 0.0f};
        auto& c = ground.addComponent<fluxgl::BoxCollider2D>();
        c.size = {10.0f, 1.0f};
        ground.addComponent<fluxgl::RigidBody2D>().type = fluxgl::RigidBodyType::Static;
        return ground;
    }

    // dynamic box whose collider already overlaps the ground by 0.1
    fluxgl::Entity makeFallingBox(fluxgl::Scene& scene, bool trigger = false) {
        auto box = scene.createEntity();
        auto& t = box.addComponent<fluxgl::Transform>();
        t.position = {0.0f, 0.4f, 0.0f};
        auto& c = box.addComponent<fluxgl::BoxCollider2D>();
        c.size = {1.0f, 1.0f};
        c.isTrigger = trigger;
        box.addComponent<fluxgl::RigidBody2D>();
        return box;
    }

    bool hasEvent(const std::vector<fluxgl::CollisionEvent>& events,
                  fluxgl::CollisionType type,
                  fluxgl::EntityID idA, fluxgl::EntityID idB) {
        for(const auto& e : events) {
            if(e.type != type) continue;
            fluxgl::EntityID min = (e.a.getID() < e.b.getID()) ? e.a.getID() : e.b.getID();
            fluxgl::EntityID max = (e.a.getID() > e.b.getID()) ? e.a.getID() : e.b.getID();
            if(min == idA && max == idB) return true;
        }
        return false;
    }
}

TEST_CASE("Physics: gravity only applies to dynamic bodies") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();

    auto dyn = scene.createEntity();
    dyn.addComponent<fluxgl::Transform>();
    dyn.addComponent<fluxgl::RigidBody2D>();

    auto stat = scene.createEntity();
    stat.addComponent<fluxgl::Transform>();
    stat.addComponent<fluxgl::RigidBody2D>().type = fluxgl::RigidBodyType::Static;

    auto kin = scene.createEntity();
    kin.addComponent<fluxgl::Transform>();
    kin.addComponent<fluxgl::RigidBody2D>().type = fluxgl::RigidBodyType::Kinematic;

    fluxgl::PhysicsSystem physics;
    physics.onFixedUpdate(scene, 1.0f / 60.0f);

    auto& dynRb = registry.getComponent<fluxgl::RigidBody2D>(dyn.getID());
    auto& statRb = registry.getComponent<fluxgl::RigidBody2D>(stat.getID());
    auto& kinRb = registry.getComponent<fluxgl::RigidBody2D>(kin.getID());

    CHECK(dynRb.velocity.y == doctest::Approx(-9.81f / 60.0f));
    CHECK(statRb.velocity.y == doctest::Approx(0.0f));
    CHECK(kinRb.velocity.y == doctest::Approx(0.0f));
}

TEST_CASE("Physics: integration updates position and velocity") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();

    auto body = scene.createEntity();
    auto& t = body.addComponent<fluxgl::Transform>();
    auto& rb = body.addComponent<fluxgl::RigidBody2D>();
    rb.velocity = {2.0f, 0.0f};

    fluxgl::PhysicsSystem physics;
    physics.onFixedUpdate(scene, 1.0f / 60.0f);

    CHECK(t.position.x == doctest::Approx(2.0f / 60.0f));
    CHECK(rb.velocity.y == doctest::Approx(-9.81f / 60.0f));
}

TEST_CASE("Physics: dynamic box lands and rests on a static ground") {
    fluxgl::Scene scene;
    makeGround(scene);
    makeFallingBox(scene);

    fluxgl::PhysicsSystem physics;

    for(int i = 0; i < 60; ++i) {
        physics.onFixedUpdate(scene, 1.0f / 60.0f);
    }

    auto collidables = scene.getRegistry().query<fluxgl::Transform, fluxgl::RigidBody2D>();
    for(auto& e : collidables) {
        if(!e.hasComponent<fluxgl::BoxCollider2D>()) continue;
        auto& t = e.getComponent<fluxgl::Transform>();
        CHECK(t.position.y == doctest::Approx(0.5f).epsilon(0.01f)); // AABB bottom rests on ground top (0)
        return;
    }
    FAIL("no dynamic box found");
}

TEST_CASE("Physics: collision events enter / stay / exit") {
    fluxgl::Scene scene;
    auto ground = makeGround(scene);
    auto box = makeFallingBox(scene);

    fluxgl::PhysicsSystem physics;
    physics.onFixedUpdate(scene, 1.0f / 60.0f);

    CHECK(hasEvent(physics.getCollisions(), fluxgl::CollisionType::ENTER, ground.getID(), box.getID()));

    physics.onFixedUpdate(scene, 1.0f / 60.0f);
    CHECK(hasEvent(physics.getCollisions(), fluxgl::CollisionType::STAY, ground.getID(), box.getID()));

    // Move the box away: the pair stops colliding -> EXIT event
    auto& t = scene.getRegistry().getComponent<fluxgl::Transform>(box.getID());
    t.position.y = 20.0f;
    physics.onFixedUpdate(scene, 1.0f / 60.0f);

    REQUIRE(hasEvent(physics.getCollisions(), fluxgl::CollisionType::EXIT, ground.getID(), box.getID()));
}

TEST_CASE("Physics: triggers generate events but no resolution") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();
    makeGround(scene);
    auto trigger = makeFallingBox(scene, true);
    auto triggerRb = trigger.getID();
    (void)triggerRb;

    fluxgl::PhysicsSystem physics;
    physics.onFixedUpdate(scene, 1.0f / 60.0f);

    auto& t = registry.getComponent<fluxgl::Transform>(trigger.getID());
    auto& rb = registry.getComponent<fluxgl::RigidBody2D>(trigger.getID());

    // Trigger fell straight down, x unchanged, velocity untouched (not zeroed)
    CHECK(t.position.x == doctest::Approx(0.0f));
    CHECK(rb.velocity.y == doctest::Approx(-9.81f / 60.0f));
    CHECK_FALSE(rb.grounded);
}

TEST_CASE("Physics: disabled colliders do not collide") {
    fluxgl::Scene scene;
    auto ground = makeGround(scene);
    auto box = makeFallingBox(scene);
    box.disable();

    fluxgl::PhysicsSystem physics;
    physics.onFixedUpdate(scene, 1.0f / 60.0f);

    CHECK_FALSE(hasEvent(physics.getCollisions(), fluxgl::CollisionType::ENTER, ground.getID(), box.getID()));
}

TEST_CASE("Physics: parent/child colliders never collide") {
    fluxgl::Scene scene;
    auto parent = makeGround(scene);

    auto child = parent.appendChild();
    child.addComponent<fluxgl::Transform>();
    auto& c = child.addComponent<fluxgl::BoxCollider2D>();
    c.size = {4.0f, 4.0f};
    child.addComponent<fluxgl::RigidBody2D>();

    fluxgl::PhysicsSystem physics;
    physics.onFixedUpdate(scene, 1.0f / 60.0f);

    // child overlaps the parent but the pair must be ignored
    CHECK(physics.getCollisions().empty());
}

TEST_CASE("Physics: raycast returns the closest collider") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();

    auto ground = scene.createEntity();
    auto& gt = ground.addComponent<fluxgl::Transform>();
    gt.position = {0.0f, -0.5f, 0.0f};
    auto& gc = ground.addComponent<fluxgl::BoxCollider2D>();
    gc.size = {10.0f, 1.0f};

    auto box = scene.createEntity();
    auto& bt = box.addComponent<fluxgl::Transform>();
    bt.position = {0.0f, 5.0f, 0.0f};
    auto& bc = box.addComponent<fluxgl::BoxCollider2D>();
    bc.size = {2.0f, 2.0f};

    fluxgl::Ray ray;
    ray.origin = {0.0f, 10.0f, 0.0f};
    ray.direction = {0.0f, -1.0f, 0.0f};

    auto hit = fluxgl::PhysicsSystem::raycast(registry, ray);
    REQUIRE(hit.has_value());
    CHECK(hit->entity.getID() == box.getID());
    CHECK(hit->distance == doctest::Approx(4.0f));
    CHECK(hit->point.y == doctest::Approx(6.0f));
}

TEST_CASE("Physics: raycast ignores triggers by default, finds none when all are triggers") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();

    auto ground = scene.createEntity();
    ground.addComponent<fluxgl::Transform>();
    auto& gc = ground.addComponent<fluxgl::BoxCollider2D>();
    gc.size = {10.0f, 1.0f};
    gc.isTrigger = true;

    fluxgl::Ray ray;
    ray.origin = {0.0f, 5.0f, 0.0f};
    ray.direction = {0.0f, -1.0f, 0.0f};

    CHECK_FALSE(fluxgl::PhysicsSystem::raycast(registry, ray).has_value());

    auto hit = fluxgl::PhysicsSystem::raycast(registry, ray, false);
    REQUIRE(hit.has_value());
    CHECK(hit->entity.getID() == ground.getID());
}

TEST_CASE("Physics: raycastAll sorts hits by distance") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();

    for(int i = 0; i < 3; ++i) {
        auto e = scene.createEntity();
        auto& t = e.addComponent<fluxgl::Transform>();
        t.position = {0.0f, 2.0f + i * 2.0f, 0.0f};
        auto& c = e.addComponent<fluxgl::BoxCollider2D>();
        c.size = {2.0f, 2.0f};
    }

    fluxgl::Ray ray;
    ray.origin = {0.0f, 10.0f, 0.0f};
    ray.direction = {0.0f, -1.0f, 0.0f};

    auto hits = fluxgl::PhysicsSystem::raycastAll(registry, ray);
    REQUIRE(hits.size() == 3);
    for(size_t i = 1; i < hits.size(); ++i) {
        CHECK(hits[i - 1].distance <= hits[i].distance);
    }
}

TEST_CASE("Physics: raycast with no hit returns nullopt") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();

    fluxgl::Ray ray;
    ray.origin = {0.0f, 10.0f, 0.0f};
    ray.direction = {1.0f, 0.0f, 0.0f};

    CHECK_FALSE(fluxgl::PhysicsSystem::raycast(registry, ray).has_value());
}

TEST_CASE("Physics: LifetimeSystem destroys expired entities") {
    fluxgl::Scene scene;
    auto& registry = scene.getRegistry();

    auto shortLived = scene.createEntity();
    shortLived.addComponent<fluxgl::Lifetime>(0.5f);

    auto longLived = scene.createEntity();
    longLived.addComponent<fluxgl::Lifetime>(10.0f);

    fluxgl::LifetimeSystem lifetime;

    lifetime.onLateUpdate(scene, 0.2f);
    CHECK(registry.isAliveEntity(shortLived.getID()));

    lifetime.onLateUpdate(scene, 0.4f); // remaining: 0.5 - 0.6 <= 0
    CHECK_FALSE(registry.isAliveEntity(shortLived.getID())); // marked for destruction

    registry.update(0.016f);
    CHECK_FALSE(registry.isEntityInUse(shortLived.getID()));
    CHECK(registry.isEntityInUse(longLived.getID()));
}