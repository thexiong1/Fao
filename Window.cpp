#include "Window.h"

SDL_Color Window::HexToSDLColor(const std::string &hex) {
    if (hex.length() != 8) {
        throw std::invalid_argument("十六进制颜色格式无效。预期格式：RRGGBBAA");
    }

    Uint8 r = static_cast<Uint8>(std::stoul(hex.substr(0, 2), nullptr, 16));
    Uint8 g = static_cast<Uint8>(std::stoul(hex.substr(2, 2), nullptr, 16));
    Uint8 b = static_cast<Uint8>(std::stoul(hex.substr(4, 2), nullptr, 16));
    Uint8 a = static_cast<Uint8>(std::stoul(hex.substr(6, 2), nullptr, 16));

    return {r, g, b, a};
}

// 初始化 SDL、TTF 并创建窗口和渲染器
void Window::Init(const std::string& title,const std::string& hexColor) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("SDL 初始化失败: " + std::string(SDL_GetError()));
    }
    if (TTF_Init() != 0) {
        throw std::runtime_error("TTF 初始化失败: " + std::string(TTF_GetError()));
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        throw std::runtime_error("创建窗口失败: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        throw std::runtime_error("创建渲染器失败: " + std::string(SDL_GetError()));
    }
    // 解析16进制颜色并设置背景颜色
    SDL_Color backgroundColor = HexToSDLColor(hexColor);
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
}

// 清除窗口
void Window::Clear() {
    SDL_RenderClear(renderer);
}

// 显示渲染结果
void Window::Present() {
    SDL_RenderPresent(renderer);
}

// 加载图像
SDL_Texture* Window::LoadImage(const std::string& file) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file.c_str());
    if (!texture) {
        throw std::runtime_error("无法加载图像: " + file + " " + IMG_GetError());
    }
    return texture;
}

// 绘制纹理，允许宽高为默认值
void Window::Draw(int x, int y, SDL_Texture* tex, int w, int h) {
    SDL_Rect destRect {x, y, w, h};

    // 如果宽度或高度为-1，获取纹理的实际大小
    if (w == -1 || h == -1) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(tex, nullptr, nullptr, &textureWidth, &textureHeight);
        if (w == -1) destRect.w = textureWidth;
        if (h == -1) destRect.h = textureHeight;
    }

    SDL_RenderCopy(renderer, tex, nullptr, &destRect);
}

// 渲染文本
SDL_Texture* Window::RenderText(const std::string& msg, SDL_Color color, int fontSize) {
    const char* fontFile = "../assets/font/LXGWWenKaiGBFusion-Regular.ttf";  // 字体文件路径
    TTF_Font* font = TTF_OpenFont(fontFile, fontSize);  // 打开字体文件
    if (!font) {
        throw std::runtime_error("加载字体失败: " + std::string(fontFile) + " " + TTF_GetError());
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, msg.c_str(), color);  // 渲染UTF-8文本
    if (!surface) {
        TTF_CloseFont(font);  // 确保在抛出异常前关闭字体文件
        throw std::runtime_error("无法渲染文本: " + std::string(TTF_GetError()));
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);  // 将渲染后的文本转换为纹理
    SDL_FreeSurface(surface);  // 释放表面资源
    TTF_CloseFont(font);  // 关闭字体文件

    return texture;
}

// 清理资源
void Window::CleanUp(std::vector<SDL_Texture*>& textures) {
    for (auto& texture : textures) {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
    textures.clear(); // 清空向量
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

// 资源管理类清理
Window::~Window() {
    // 确保退出时清理资源
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();
}

// 获取窗口当前的宽度
int Window::GET_SCREEN_WIDTH() {
    int width = 0;  // 用于存储宽度的临时变量
    SDL_GetWindowSize(window, &width, nullptr);  // 获取窗口宽度
    return width;  // 返回获取到的宽度
}

// 获取窗口当前的高度
int Window::GET_SCREEN_HEIGHT() {
    int height = 0;  // 用于存储高度的临时变量
    SDL_GetWindowSize(window, nullptr, &height);  // 获取窗口高度
    return height;  // 返回获取到的高
}

SDL_Rect Window::GetTextureSize(SDL_Texture *texture) {
    SDL_Rect rect = {0, 0, 0, 0}; // 初始化为 (0, 0, 0, 0)
    if (texture == nullptr) {
        throw std::invalid_argument("Texture is null.");
    }
    if (SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h) != 0) {
        throw std::runtime_error("Failed to query texture size: " + std::string(SDL_GetError()));
    }
    return rect; // 返回 SDL_Rect
}

void Window::delayFrame(Uint32 frameStart) {
    int frameTime = SDL_GetTicks() - frameStart;
    if (1000 / 60 > frameTime) {
        SDL_Delay(1000 / 60 - frameTime);
    }
}

