#include <doctest/doctest.h>

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/behavior.h>

namespace {
    struct Position { float x = 0.0f; float y = 0.0f; };
    struct Velocity { float dx = 0.0f; float dy = 0.0f; };
    struct Marker {};
}

TEST_CASE("ECS Registry: create entity and reuse ids") {
    fluxgl::Registry registry;

    auto e1 = registry.createEntity();
    auto e2 = registry.createEntity();
    CHECK(e1.getID() != e2.getID());
    CHECK(registry.isValidEntity(e1.getID()));
    CHECK(registry.isEntityInUse(e1.getID()));

    registry.destroyEntity(e1.getID());
    CHECK_FALSE(registry.isAliveEntity(e1.getID()));

    registry.update(0.016f);
    CHECK_FALSE(registry.isEntityInUse(e1.getID()));

    auto e3 = registry.createEntity();
    CHECK(e3.getID() == e1.getID());
}

TEST_CASE("ECS Registry: entity capacity limit is enforced") {
    fluxgl::Registry registry;

    for(unsigned int i = 0; i < FLUXGL_MAX_ENTITIES; ++i) {
        registry.createEntity();
    }

    CHECK_THROWS_AS(registry.createEntity(), std::runtime_error);

    registry.destroyEntity(1);
    registry.update(0.016f);
    CHECK_NOTHROW(registry.createEntity());
}

TEST_CASE("ECS Registry: components add / get / has / remove") {
    fluxgl::Registry registry;
    auto entity = registry.createEntity();

    CHECK_FALSE(entity.hasComponent<Position>());

    auto& pos = entity.addComponent<Position>();
    pos.x = 5.0f;
    CHECK(entity.hasComponent<Position>());

    CHECK(entity.getComponent<Position>().x == doctest::Approx(5.0f));

    entity.removeComponent<Position>();
    CHECK_FALSE(entity.hasComponent<Position>());
    CHECK_FALSE(entity.hasComponent<Velocity>());
}

TEST_CASE("ECS Registry: addComponent is idempotent for a single entity") {
    fluxgl::Registry registry;
    auto e = registry.createEntity();

    auto& a = e.addComponent<Marker>();
    auto& b = e.addComponent<Marker>();
    CHECK(&a == &b);
}

TEST_CASE("ECS Registry: component access on invalid/missing data throws") {
    fluxgl::Registry registry;
    auto entity = registry.createEntity();

    CHECK_THROWS_AS(entity.getComponent<Position>(), std::runtime_error);
    CHECK_THROWS_AS(entity.removeComponent<Position>(), std::runtime_error);
    CHECK_THROWS_AS(registry.getComponent<Position>(FLUXGL_MAX_ENTITIES + 5), std::runtime_error);
}

TEST_CASE("ECS Registry: removing an entity frees its components and behaviors") {
    fluxgl::Registry registry;
    auto entity = registry.createEntity();
    entity.addComponent<Position>();
    entity.addComponent<Velocity>();

    registry.destroyEntity(entity.getID());
    registry.update(0.016f);

    // Id is reusable and the old component storages should not contain it anymore
    auto reused = registry.createEntity();
    CHECK_FALSE(registry.hasComponent<Position>(reused.getID()));
}

TEST_CASE("ECS Registry: query filters entities by component set") {
    fluxgl::Registry registry;

    auto player = registry.createEntity();
    player.addComponent<Position>();
    player.addComponent<Velocity>();

    auto staticProp = registry.createEntity();
    staticProp.addComponent<Position>();

    auto tagged = registry.createEntity();
    tagged.addComponent<Marker>();

    auto results = registry.query<Position, Velocity>();
    REQUIRE(results.size() == 1);
    CHECK(results[0].getID() == player.getID());

    CHECK(registry.query<Velocity>().size() == 1);
    CHECK(registry.query<Marker>().size() == 1);
}

TEST_CASE("ECS Registry: query skips entities marked for destruction") {
    fluxgl::Registry registry;

    auto e1 = registry.createEntity();
    e1.addComponent<Position>();
    auto e2 = registry.createEntity();
    e2.addComponent<Position>();

    registry.destroyEntity(e1.getID());
    auto results = registry.query<Position>();
    REQUIRE(results.size() == 1);
    CHECK(results[0].getID() == e2.getID());
}

TEST_CASE("ECS Registry: tags") {
    fluxgl::Registry registry;
    auto entity = registry.createEntity();

    CHECK_FALSE(entity.hasTag("enemy"));
    entity.addTag("enemy");
    entity.addTag("enemy"); // idempotent
    CHECK(entity.hasTag("enemy"));

    auto byTag = registry.queryByTag("enemy");
    REQUIRE(byTag.size() == 1);
    CHECK(byTag[0].getID() == entity.getID());

    entity.removeTag("enemy");
    CHECK_FALSE(entity.hasTag("enemy"));
    CHECK(registry.queryByTag("enemy").empty());
}

TEST_CASE("ECS Registry: enable / disable") {
    fluxgl::Registry registry;
    auto entity = registry.createEntity();
    CHECK(entity.isEnabled());

    entity.disable();
    CHECK_FALSE(entity.isEnabled());
    CHECK_FALSE(registry.isEntityEnabled(entity.getID()));

    entity.enable();
    CHECK(entity.isEnabled());
}

TEST_CASE("ECS Registry: hierarchy") {
    fluxgl::Registry registry;

    auto parent = registry.createEntity();
    auto childA = parent.appendChild();
    auto childB = parent.appendChild();

    CHECK(childA.getParent().getID() == parent.getID());
    CHECK(childB.getParent().getID() == parent.getID());

    auto children = registry.getChildren(parent.getID());
    REQUIRE(children.size() == 2);

    registry.setParent(childA.getID(), 0); // unparent
    CHECK_FALSE(childA.getParent().isValid());
    CHECK(registry.getChildren(parent.getID()).size() == 1);

    registry.destroyEntity(parent.getID());
    registry.update(0.016f);
    CHECK_FALSE(registry.isEntityInUse(parent.getID()));
    CHECK_FALSE(registry.isEntityInUse(childB.getID()));
}

TEST_CASE("ECS Registry: clear() resets everything") {
    fluxgl::Registry registry;
    auto entity = registry.createEntity();
    entity.addComponent<Position>();
    registry.destroyEntity(entity.getID());

    registry.clear();
    registry.update(0.016f);

    auto fresh = registry.createEntity();
    CHECK_FALSE(registry.hasComponent<Position>(fresh.getID()));
    CHECK(registry.query<Position>().empty());
    CHECK(registry.getAllBehaviors().empty());
}

TEST_CASE("ECS Registry: destroy then reuse before update keeps entity dead") {
    fluxgl::Registry registry;
    auto e = registry.createEntity();
    registry.destroyEntity(e.getID());

    auto pending = registry.query<Marker>(); // must not have any stored components
    CHECK(pending.empty());

    auto e2 = registry.createEntity();
    CHECK(e2.getID() > 0);
}