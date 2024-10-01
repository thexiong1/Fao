#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>
#include "Window.h"

enum ButtonState {
    NORMAL,
    PRESSED,
    HOVERED
};


class Button {
public:
    SDL_Rect rect;
    std::function<void()> onPress;   // 按下事件
    std::function<void()> onRelease; // 弹起事件
    ButtonState state = NORMAL;  // 按钮状态

    Button(int x, int y, int width, int height);

    virtual void handleEvent(const SDL_Event& event);
    virtual void render(Window& window) = 0;

protected:
    bool isMouseInside(int mouseX, int mouseY);
};

// 普通图片按钮
class ImageButton : public Button {
public:
    SDL_Texture* texture;

    ImageButton(int x, int y, int width, int height, SDL_Texture* tex);
    void render(Window& window) override;
};

// 图片文字按钮
class ImageTextButton : public Button {
private:
    SDL_Texture* textureDow;
    SDL_Texture* textureUp;
    SDL_Texture* textTexture;  // 存储文本纹理
    std::string text;
    SDL_Color textColor;
    int fontSize;

public:
    ImageTextButton(int x, int y, int width, int height, SDL_Texture* textureDow,SDL_Texture* textureUp,SDL_Texture* textTexture,
                    std::string  txt, SDL_Color color, int size);
    void render(Window& window) override;
};

#endif // BUTTON_H
// 多图片文字按钮
//class MultiImageTextButton : public Button {
//private:
//    std::vector<SDL_Texture*> textures;
//    std::vector<std::string> texts;
//    SDL_Color textColor;
//    int fontSize;
//
//public:
//    MultiImageTextButton(int x, int y, int width, int height, const std::vector<SDL_Texture*>& texs, const std::vector<std::string>& txts, SDL_Color color, int size);
//    void render(Window& window) override;
//};


