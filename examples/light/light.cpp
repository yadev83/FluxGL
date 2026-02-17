#include <fluxgl/fluxgl.h>

class LightExample : public fluxgl::App {
    public: using fluxgl::App::App;

    private:
        fluxgl::Renderable entity;
        fluxgl::Camera camera;

        fluxgl::Renderable lightEntity;
        fluxgl::Light light;

    public:
        void onInit() override {
            camera.transform.position = {0.0f, 0.0f, 3.0f};

            entity.material.albedoColor = {1.0f, 0.5f, 0.31f};
            entity.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/base.vertex.glsl", "assets/shaders/base.fragment.glsl");
            entity.mesh = fluxgl::Mesh::cube();

            lightEntity.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/base.vertex.glsl", "assets/shaders/light_source.fragment.glsl");
            lightEntity.transform.position = {1.2f, 1.0f, -2.0f};
            lightEntity.transform.scale = {0.2f, 0.2f, 0.2f};
            lightEntity.mesh = fluxgl::Mesh::sphere();
        }

        void onUpdate(float deltaTime) override {
            if(getInput().isKeyPressed(GLFW_KEY_ESCAPE)) getWindow().isMouseLocked() ? getWindow().setMouseLocked(false) : getWindow().quit();
            if(getInput().isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) getWindow().setMouseLocked(true);

            float velocity = 5.0f * deltaTime;
            if(getInput().isKeyDown(GLFW_KEY_W)) camera.transform.position += camera.transform.front() * velocity;
            if(getInput().isKeyDown(GLFW_KEY_S)) camera.transform.position -= camera.transform.front() * velocity;
            if(getInput().isKeyDown(GLFW_KEY_A)) camera.transform.position -= camera.transform.right() * velocity;
            if(getInput().isKeyDown(GLFW_KEY_D)) camera.transform.position += camera.transform.right() * velocity;

            if(getWindow().isMouseLocked()) {
                static double lastX = getInput().getMouseX(), lastY = getInput().getMouseY();
                double offsetX = getInput().getMouseX() - lastX;
                double offsetY = lastY - getInput().getMouseY();
                lastX = getInput().getMouseX();
                lastY = getInput().getMouseY();
                float sensitivity = 0.1f;

                camera.transform.rotation.y += offsetX * sensitivity;
                camera.transform.rotation.x += offsetY * sensitivity;

                if(camera.transform.rotation.x > 89.0f) camera.transform.rotation.x = 89.0f;
                if(camera.transform.rotation.x < -89.0f) camera.transform.rotation.x = -89.0f;
            }
        }

        void onRender() override {
            fluxgl::Renderer::clear();
            fluxgl::Renderer::draw(entity, &camera, &light);

            // Also render the light source
            lightEntity.material.albedoColor = light.color;
            fluxgl::Renderer::draw(lightEntity, &camera);
        }
};

int main() {
    LightExample app(800, 600, "Light");
    app.run();
    return 0;
}