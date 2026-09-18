#include <doctest/doctest.h>

#include <fluxgl/core/scene.h>
#include <fluxgl/core/transform.h>
#include <fluxgl/physics/follow.h>
#include <fluxgl/physics/follow_system.h>

TEST_CASE("Follow2D: instant follow when smoothing is disabled") {
    fluxgl::Scene scene;

    auto target = scene.createEntity();
    auto& tt = target.addComponent<fluxgl::Transform>();
    tt.position = {3.0f, 4.0f, 0.0f};

    auto camera = scene.createEntity();
    auto& ct = camera.addComponent<fluxgl::Transform>();
    auto& follow = camera.addComponent<fluxgl::Follow2D>(target.getID());
    follow.offset = {0.0f, 2.0f};

    fluxgl::Follow2DSystem system;
    (void)system;
    scene.registerSystem<fluxgl::Follow2DSystem>();

    scene.updateSystems(1.0f / 60.0f);

    CHECK(ct.position.x == doctest::Approx(3.0f));
    CHECK(ct.position.y == doctest::Approx(6.0f));
}

TEST_CASE("Follow2D: smoothed follow converges to the target") {
    fluxgl::Scene scene;

    auto target = scene.createEntity();
    auto& tt = target.addComponent<fluxgl::Transform>();
    tt.position = {3.0f, 4.0f, 0.0f};

    auto camera = scene.createEntity();
    camera.addComponent<fluxgl::Transform>();
    auto& follow = camera.addComponent<fluxgl::Follow2D>(target.getID());
    follow.smoothing = 20.0f;

    fluxgl::Follow2DSystem system;
    (void)system;
    scene.registerSystem<fluxgl::Follow2DSystem>();
    for(int i = 0; i < 120; ++i) {
        scene.updateSystems(1.0f / 60.0f);
    }

    auto& ct = camera.getComponent<fluxgl::Transform>();
    CHECK(ct.position.x == doctest::Approx(3.0f).epsilon(0.01f));
    CHECK(ct.position.y == doctest::Approx(4.0f).epsilon(0.01f));
}

TEST_CASE("Follow2D: deadzone keeps the camera still") {
    fluxgl::Scene scene;

    auto target = scene.createEntity();
    auto& tt = target.addComponent<fluxgl::Transform>();
    tt.position = {0.5f, 0.0f, 0.0f};

    auto camera = scene.createEntity();
    auto& ct = camera.addComponent<fluxgl::Transform>();
    auto& follow = camera.addComponent<fluxgl::Follow2D>(target.getID());
    follow.deadzone = {2.0f, 2.0f};
    follow.smoothing = 1.0f;

    fluxgl::Follow2DSystem system;
    (void)system;
    scene.registerSystem<fluxgl::Follow2DSystem>();
    scene.updateSystems(1.0f / 60.0f);

    CHECK(ct.position.x == doctest::Approx(0.0f));
    CHECK(ct.position.y == doctest::Approx(0.0f));
}

TEST_CASE("Follow2D: target outside the deadzone starts moving the camera") {
    fluxgl::Scene scene;

    auto target = scene.createEntity();
    auto& tt = target.addComponent<fluxgl::Transform>();
    tt.position = {5.0f, 0.0f, 0.0f};

    auto camera = scene.createEntity();
    auto& ct = camera.addComponent<fluxgl::Transform>();
    auto& follow = camera.addComponent<fluxgl::Follow2D>(target.getID());
    follow.deadzone = {2.0f, 2.0f};
    follow.smoothing = 1.0f;

    fluxgl::Follow2DSystem system;
    (void)system;
    scene.registerSystem<fluxgl::Follow2DSystem>();
    scene.updateSystems(1.0f / 60.0f);

    CHECK(ct.position.x > 0.0f);
}

TEST_CASE("Follow2D: look-ahead pushes the camera in front of a moving target") {
    fluxgl::Scene scene;

    auto target = scene.createEntity();
    auto& tt = target.addComponent<fluxgl::Transform>();
    tt.position = {1.0f, 0.0f, 0.0f};

    auto camera = scene.createEntity();
    auto& ct = camera.addComponent<fluxgl::Transform>();
    auto& follow = camera.addComponent<fluxgl::Follow2D>(target.getID());
    follow.smoothing = 10.0f;
    follow.lookAheadDistance = 2.0f;
    follow.lookAheadSmoothing = 1.0f;

    scene.registerSystem<fluxgl::Follow2DSystem>();

    const float dt = 1.0f / 60.0f;
    for(int i = 0; i < 120; ++i) {
        tt.position.x += 2.0f * dt; // target keeps moving right
        scene.updateSystems(dt);
    }

    // Once the camera converges, it settles ahead of the target thanks to the look-ahead
    CHECK(ct.position.x > tt.position.x);
}

TEST_CASE("Follow2D: missing target transform is handled without crashing") {
    fluxgl::Scene scene;

    auto target = scene.createEntity();
    auto camera = scene.createEntity();
    camera.addComponent<fluxgl::Transform>();
    camera.addComponent<fluxgl::Follow2D>(target.getID());

    fluxgl::Follow2DSystem system;
    (void)system;
    scene.registerSystem<fluxgl::Follow2DSystem>();
    CHECK_NOTHROW(scene.updateSystems(1.0f / 60.0f));
}