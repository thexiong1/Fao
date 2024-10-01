
#include "Button.h"

#include <utility>

Button::Button(int x, int y, int width, int height) {
    rect = {x,y,width,height};
    onPress = nullptr;
    onRelease = nullptr;
}
// 处理鼠标事件，检查是否点击了按钮
void Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        if (isMouseInside(event.button.x, event.button.y)) {
            state = PRESSED;
            if (onPress) {
                onPress();
            }
        }
    } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        if (isMouseInside(event.button.x, event.button.y)) {
            state = NORMAL;
            if (onRelease) {
                onRelease();
            }
        } else {
            state = NORMAL;  // 鼠标移出按钮区域时恢复正常状态
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        if (!isMouseInside(event.motion.x, event.motion.y) && state != PRESSED) {
            state = NORMAL;  // 如果鼠标移出并且没有按下，恢复到 NORMAL
        }
    }
}


// 判断鼠标是否在按钮区域内
bool Button::isMouseInside(int mouseX, int mouseY) {
    SDL_Point point = {mouseX, mouseY};
    return SDL_PointInRect(&point, &rect);
}


ImageButton::ImageButton(int x, int y, int width, int height, SDL_Texture *tex) :
Button(x, y, width, height), texture(tex){
    onPress = [](){
        std::cout << "图片按钮被按下!" << std::endl;
    };
    onRelease = [](){};
}

void ImageButton::render(Window &window) {
    window.Draw(rect.x,rect.y,texture,rect.w,rect.h);
}

ImageTextButton::ImageTextButton(int x, int y, int width, int height, SDL_Texture *textureDow, SDL_Texture *textureUp,SDL_Texture* textTexture,
                                 std::string txt, SDL_Color color, int size):
    Button(x, y, width, height), textureUp(textureUp),textureDow(textureDow),textTexture(textTexture),text(std::move(txt)), textColor(color),fontSize(size){

}

void ImageTextButton::render(Window &window) {
    // 渲染文本，假设 window.RenderText 返回的是文本的 SDL_Texture
//    SDL_Texture* textTexture = window.RenderText(text, textColor, fontSize);
    // 检查文本纹理是否已存在
    if (textTexture == nullptr) {
        // 如果不存在，则生成新的纹理
        textTexture = window.RenderText(text, textColor, fontSize);
    }

    // 获取文本纹理的宽和高
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    // 设置背景的大小，基于文本的宽高
    int padding = 4; // 添加一些边距，使背景稍微大于文本
    rect.w = textWidth + padding;
    rect.h = textHeight + padding;

    // 根据按钮状态选择背景纹理
    SDL_Texture* currentTexture = state ? textureDow : textureUp;


    // 绘制按钮背景（根据文本大小调整的背景大小）
    window.Draw(rect.x, rect.y, currentTexture, rect.w, rect.h);

    // 计算文本的绘制位置
    int textX = rect.x + (rect.w - textWidth) / 2;
    int textY = rect.y + (rect.h - textHeight) / 2;

    // 绘制文本
    window.Draw(textX, textY, textTexture, textWidth, textHeight);
}
