#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class Text : public fluxgl::Scene {
    fluxgl::Entity text;
    fluxgl::Entity rect;

    int anchorIndex = 0;
    int pivotIndex = 0;

    public:
        void onLoad() override {
            registerSystem<fluxgl::RenderSystem>();
            registerSystem<fluxgl::UISystem>();

            getContext().resourceManager.addResource<fluxgl::Font>("arial", fluxgl::Font::loadFromMemory(getContext().vfs.read("assets/fonts/arial.ttf"), 64));
            getContext().resourceManager.addResource<fluxgl::Shader>("text", fluxgl::Shader::defaultText());
            getContext().resourceManager.addResource<fluxgl::Shader>("ui", fluxgl::Shader::defaultUi());
        }

        void onInit() override {
            text = createEntity();
            auto& uiTransform = text.addComponent<fluxgl::UITransform>();
            auto& uiText = text.addComponent<fluxgl::UIText>();
            auto& uiButton = text.addComponent<fluxgl::UIButton>();
            auto& uiCollider = text.addComponent<fluxgl::UICollider>();
            auto& uiRect = text.addComponent<fluxgl::UIRect>();

            uiText.text = "Bonjour";
            uiText.font = "arial";
            uiText.fontSize = 32;
            uiText.shader = "text";

            auto textSz = getContext().resourceManager.getResource<fluxgl::Font>(uiText.font)->measureText(uiText.text, uiText.fontSize);
            uiCollider.size = {
                textSz.width,
                textSz.height
            };

            uiRect.shader = "ui";
            uiRect.size = {
                textSz.width,
                textSz.height
            };
        }

        void onUpdate(float deltaTime) override {
            if(getContext().inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                getContext().window.setWindowShouldClose();
            }
            
            if(getContext().inputManager.isKeyPressed(GLFW_KEY_F11)) {
                getContext().window.setWindowFullscreenMode(!getContext().window.getWindowFullscreenMode());
            }

            if(getContext().inputManager.isKeyDown(GLFW_KEY_LEFT)) {
                text.getComponent<fluxgl::UITransform>().offset.x -= deltaTime * 100;
            }
            if(getContext().inputManager.isKeyDown(GLFW_KEY_RIGHT)) {
                text.getComponent<fluxgl::UITransform>().offset.x += deltaTime * 100;
            }

            if(getContext().inputManager.isKeyDown(GLFW_KEY_UP)) {
                text.getComponent<fluxgl::UITransform>().offset.y -= deltaTime * 100;
            }
            if(getContext().inputManager.isKeyDown(GLFW_KEY_DOWN)) {
                text.getComponent<fluxgl::UITransform>().offset.y += deltaTime * 100;
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_Q)) {
                text.getComponent<fluxgl::UITransform>().anchor = fluxgl::alignmentFromIndex(++anchorIndex);
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_P)) {
                text.getComponent<fluxgl::UITransform>().pivot = fluxgl::alignmentFromIndex(++pivotIndex);
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_R)) {
                text.getComponent<fluxgl::UITransform>().offset = {0, 0};
                anchorIndex = 0;
                text.getComponent<fluxgl::UITransform>().anchor = fluxgl::alignmentFromIndex(anchorIndex);
                pivotIndex = 0;
                text.getComponent<fluxgl::UITransform>().pivot = fluxgl::alignmentFromIndex(pivotIndex);
            }

            auto& uiButton = text.getComponent<fluxgl::UIButton>();
            if(uiButton.pressed) FLUXGL_LOG_TRACE("BUTTON PRESSED");
        }
};

int main() {
    fluxgl::App app(800, 600, "Text Example");

    fluxgl::SceneManager::get().registerScene<Text>("Text");
    app.run();

    return 0;
}