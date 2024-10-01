#include "window.h"
#include "Button.h"
#include <iostream>


int main(int argc, char **argv)
{

    Window app;  // 实例化 Window 类
    try {
        app.Init(u8"我是标题","000040FF");  // 初始化窗口和渲染器
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::vector<SDL_Texture*> textures;
    SDL_Texture *leftBorder = nullptr, *rightBorder = nullptr, *topBorder = nullptr, *scrTopLeft = nullptr, *text = nullptr,
            *retButton = nullptr,*upButton = nullptr,*dowButton = nullptr;

    try {
        // 加载纹理
        leftBorder = app.LoadImage("../assets/side/gnl-bk.bmp");
        rightBorder = app.LoadImage("../assets/side/gnl1-bk.bmp");
        topBorder = app.LoadImage("../assets/side/map-tit.bmp");
        scrTopLeft = app.LoadImage("../assets/side/t-img1.bmp");
        upButton = app.LoadImage("../assets/button/anniu1.png");
        dowButton = app.LoadImage("../assets/button/anniu2.png");
        retButton = app.LoadImage("../assets/button/fanhui.png");

        textures.push_back(leftBorder);
        textures.push_back(rightBorder);
        textures.push_back(topBorder);
        textures.push_back(scrTopLeft);
        textures.push_back(retButton);
        textures.push_back(upButton);
        textures.push_back(dowButton);

        // 渲染文本
        SDL_Color color = {255, 255, 255};
        text = app.RenderText(u8"官府", color, 18);
        textures.push_back(text);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        app.CleanUp(textures);
        return 2;
    }

    // 创建 ImageButton 实例viewport.getWorldWidth()/2f+45, viewport.getWorldHeight()/5f
    ImageButton button(app.GET_SCREEN_WIDTH()/1.5, app.GET_SCREEN_HEIGHT()/1.3, 64, 28, retButton);

    //float startX = Gdx.graphics.getWidth() / 2f - 100;
    //        float startY = Gdx.graphics.getHeight() * 8.8f / 10 - 40;
    ImageTextButton textButtonOne(40,app.GET_SCREEN_HEIGHT()/9,0,0,dowButton,upButton, nullptr,
                                  u8"技能学习",{255,255,255},16);





    // 主循环
    SDL_Event e;
    bool quit = false;


    while (!quit) {
        // 清屏并绘制纹理和文本
        app.Clear();
        while (SDL_PollEvent(&e) != 0) {
            // 用户请求退出
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // 处理按钮事件
            button.handleEvent(e);
            textButtonOne.handleEvent(e);
        }
        button.render(app);  // 渲染按钮
        textButtonOne.render(app);
        app.Draw(0, 24, leftBorder, 24, app.GET_SCREEN_HEIGHT() - 24);
        app.Draw(app.GET_SCREEN_WIDTH() - 24, 24, rightBorder, 24, app.GET_SCREEN_HEIGHT() - 24);
        app.Draw(0, 0, topBorder, app.GET_SCREEN_WIDTH(), 24);
        app.Draw(0, 0, scrTopLeft, app.GET_SCREEN_WIDTH() / 4, 24);
        app.Draw((app.GET_SCREEN_WIDTH()-app.GetTextureSize(text).w)/2, (app.GET_SCREEN_HEIGHT()-app.GetTextureSize(text).h)/15, text);

        app.Present();
        // 控制帧率

        app.delayFrame(SDL_GetTicks());
    }

    // 清理资源
    app.CleanUp(textures);
    return 0;
}
