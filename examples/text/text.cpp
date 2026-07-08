    #include <fluxgl/fluxgl.h>
    #include <iostream>
    #include <sstream>

    class Text : public fluxgl::Scene {
        fluxgl::Entity text;
        fluxgl::Entity rect;

        public:
            void onLoad() override {
                registerSystem<fluxgl::RenderSystem>();

                getContext().resourceManager.addResource<fluxgl::Font>("arial", fluxgl::Font::loadFromMemory(getContext().vfs.read("assets/fonts/arial.ttf")));
                getContext().resourceManager.addResource<fluxgl::Shader>("text", fluxgl::Shader::defaultText());
                getContext().resourceManager.addResource<fluxgl::Shader>("ui", fluxgl::Shader::defaultUi());
            }

            void onInit() override {
                text = createEntity();
                auto& uiTransform = text.addComponent<fluxgl::UITransform>();
                auto& uiText = text.addComponent<fluxgl::UIText>();

                uiTransform.position = {400, 300};
                uiText.text = "Bonjour";
                uiText.font = "arial";
                uiText.shader = "text";                
            }

            void onUpdate(float deltaTime) override {
                if(getContext().inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                    getContext().window.setWindowShouldClose();
                }

                if(getContext().inputManager.isKeyDown(GLFW_KEY_LEFT)) {
                    text.getComponent<fluxgl::UITransform>().position.x -= deltaTime * 100;
                }
                if(getContext().inputManager.isKeyDown(GLFW_KEY_RIGHT)) {
                    text.getComponent<fluxgl::UITransform>().position.x += deltaTime * 100;
                }

                if(getContext().inputManager.isKeyDown(GLFW_KEY_UP)) {
                    text.getComponent<fluxgl::UITransform>().position.y -= deltaTime * 100;
                }
                if(getContext().inputManager.isKeyDown(GLFW_KEY_DOWN)) {
                    text.getComponent<fluxgl::UITransform>().position.y += deltaTime * 100;
                }
            }
    };

    int main() {
        fluxgl::App app(800, 600, "Text Example");

        fluxgl::SceneManager::get().registerScene<Text>("Text");
        app.run();

        return 0;
    }