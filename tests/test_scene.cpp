#include <doctest/doctest.h>

#include <fluxgl/core/scene.h>
#include <fluxgl/ecs/behavior.h>
#include <fluxgl/ecs/entity.h>

namespace {
    class TrackingSystem : public fluxgl::System {
        public:
            int updates = 0;
            float lastDt = 0.0f;
            void onUpdate(fluxgl::Scene&, float dt) override {
                ++updates;
                lastDt = dt;
            }
    };

    class TrackingBehavior : public fluxgl::Behavior {
        public:
            int inits = 0;
            int updates = 0;
            int fixedUpdates = 0;
            int lateUpdates = 0;
            int renders = 0;

            void onInit() override { ++inits; }
            void onUpdate(float) override { ++updates; }
            void onFixedUpdate(float) override { ++fixedUpdates; }
            void onLateUpdate(float) override { ++lateUpdates; }
            void onRender(float) override { ++renders; }
    };
}

TEST_CASE("Scene: create and fetch entities") {
    fluxgl::Scene scene;
    auto e = scene.createEntity();
    CHECK(e.getID() > 0);
    CHECK(scene.getEntity(e.getID()).getID() == e.getID());
}

TEST_CASE("Scene: register / query systems by type") {
    fluxgl::Scene scene;

    CHECK_FALSE(scene.hasSystem<TrackingSystem>());
    scene.registerSystem<TrackingSystem>();
    CHECK(scene.hasSystem<TrackingSystem>());

    auto& sys = scene.getSystem<TrackingSystem>();
    CHECK(sys.updates == 0);
}

TEST_CASE("Scene: getSystem throws when missing") {
    fluxgl::Scene scene;
    CHECK_THROWS_AS(scene.getSystem<TrackingSystem>(), std::runtime_error);
}

TEST_CASE("Scene: updateSystems dispatches with the right dt") {
    fluxgl::Scene scene;
    scene.registerSystem<TrackingSystem>();

    scene.updateSystems(0.016f);
    scene.updateSystems(0.020f);

    auto& sys = scene.getSystem<TrackingSystem>();
    CHECK(sys.updates == 2);
    CHECK(sys.lastDt == doctest::Approx(0.020f));
}

TEST_CASE("Scene: clearSystems can be called safely") {
    fluxgl::Scene scene;
    scene.registerSystem<TrackingSystem>();
    scene.clearSystems();
    scene.clearSystems();
    CHECK_FALSE(scene.hasSystem<TrackingSystem>());
}

TEST_CASE("Scene: behaviors are initialized once") {
    fluxgl::Scene scene;
    auto entity = scene.createEntity();
    auto& behavior = entity.registerBehavior<TrackingBehavior>();

    CHECK_FALSE(behavior.isInitialized());
    scene.initBehaviors();
    CHECK(behavior.isInitialized());
    scene.initBehaviors(); // idempotent
    CHECK(behavior.inits == 1);
}

TEST_CASE("Scene: behavior lifecycle hooks are dispatched") {
    fluxgl::Scene scene;
    auto entity = scene.createEntity();
    auto& behavior = entity.registerBehavior<TrackingBehavior>();

    scene.updateBehaviors(0.016f);
    scene.fixedUpdateBehaviors(0.016f);
    scene.lateUpdateBehaviors(0.016f);
    scene.renderBehaviors(0.016f);

    CHECK(behavior.updates == 1);
    CHECK(behavior.fixedUpdates == 1);
    CHECK(behavior.lateUpdates == 1);
    CHECK(behavior.renders == 1);
}