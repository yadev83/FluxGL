#include <doctest/doctest.h>

#include <fluxgl/ui/ui_transform.h>

TEST_CASE("UITransform: anchor maps to a position on the screen") {
    using fluxgl::UIAlignment;

    fluxgl::UITransform t;
    t.anchor = UIAlignment::Center;
    t.pivot = UIAlignment::Center;
    t.offset = {10.0f, 20.0f};

    glm::vec3 pos = t.getComputedPosition({100.0f, 50.0f}, {800.0f, 600.0f});

    CHECK(pos.x == doctest::Approx(360.0f)); // 400 + 10 - 50
    CHECK(pos.y == doctest::Approx(295.0f)); // 300 + 20 - 25
    CHECK(pos.z == doctest::Approx(0.0f));   // layer
}

TEST_CASE("UITransform: top-left anchor and pivot keep the element corner-aligned") {
    using fluxgl::UIAlignment;

    fluxgl::UITransform t;
    t.anchor = UIAlignment::TopLeft;
    t.pivot = UIAlignment::TopLeft;
    t.offset = {5.0f, 5.0f};

    glm::vec3 pos = t.getComputedPosition({100.0f, 50.0f}, {800.0f, 600.0f});
    CHECK(pos.x == doctest::Approx(5.0f));
    CHECK(pos.y == doctest::Approx(5.0f));
}

TEST_CASE("UITransform: layer becomes the z coordinate") {
    using fluxgl::UIAlignment;

    fluxgl::UITransform t;
    t.layer = 3;
    t.anchor = UIAlignment::BottomRight;
    t.pivot = UIAlignment::BottomRight;

    glm::vec3 pos = t.getComputedPosition({100.0f, 50.0f}, {800.0f, 600.0f});
    CHECK(pos.z == doctest::Approx(3.0f));
    CHECK(pos.x == doctest::Approx(700.0f)); // 800 - 100
    CHECK(pos.y == doctest::Approx(550.0f)); // 600 - 50
}

TEST_CASE("UITransform: toAlignment maps every alignment") {
    using fluxgl::UIAlignment;

    CHECK(fluxgl::toAlignment(UIAlignment::TopLeft).x == doctest::Approx(0.0f));
    CHECK(fluxgl::toAlignment(UIAlignment::TopLeft).y == doctest::Approx(0.0f));
    CHECK(fluxgl::toAlignment(UIAlignment::Center).x == doctest::Approx(0.5f));
    CHECK(fluxgl::toAlignment(UIAlignment::Center).y == doctest::Approx(0.5f));
    CHECK(fluxgl::toAlignment(UIAlignment::BottomRight).x == doctest::Approx(1.0f));
    CHECK(fluxgl::toAlignment(UIAlignment::BottomRight).y == doctest::Approx(1.0f));
}

TEST_CASE("UITransform: alignmentFromIndex wraps within the 9 alignments") {
    CHECK(fluxgl::alignmentFromIndex(0) == fluxgl::UIAlignment::TopLeft);
    CHECK(fluxgl::alignmentFromIndex(8) == fluxgl::UIAlignment::BottomRight);
    CHECK(fluxgl::alignmentFromIndex(10) == fluxgl::UIAlignment::TopCenter);
}