#ifndef FAO_WINDOW_H
#define FAO_WINDOW_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>


class Window {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int SCREEN_WIDTH = 320;  // 默认宽度
    int SCREEN_HEIGHT = 480; // 默认高度

public:
    // 初始化 SDL、TTF 并创建窗口和渲染器
    void Init(const std::string& title,const std::string& hexColor);

    //颜色16
    SDL_Color HexToSDLColor(const std::string& hex);

    int GET_SCREEN_WIDTH();
    int GET_SCREEN_HEIGHT();
    //帧数控制
    void delayFrame(Uint32 frameStart);
    //获取纹理大小
    SDL_Rect GetTextureSize(SDL_Texture* texture);
    // 清除窗口
    void Clear();

    // 显示渲染结果
    void Present();

    // 加载图像
    SDL_Texture* LoadImage(const std::string& file);

    // 绘制纹理，允许宽高为默认值
    void Draw(int x, int y, SDL_Texture* tex, int w = -1, int h = -1);

    // 渲染文本
    SDL_Texture* RenderText(const std::string& msg, SDL_Color color, int fontSize);

    // 清理资源
    void CleanUp(std::vector<SDL_Texture*>& textures);

    // 析构函数
    ~Window();
};

#endif // FAO_WINDOW_H
