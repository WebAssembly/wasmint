/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SDLModule.h"

wasmint::SDLModule::SDLModule() {
    using namespace wasm_module;

    context().name("sdl");

#ifdef WASMINT_HAS_SDL
    addFunction("init", Int32::instance(), {Int32::instance(), Int32::instance(), Int32::instance(), Int32::instance()}, [this](std::vector<Variable> parameters) {
        if (SDL_Init(SDL_INIT_VIDEO) == 0) {
            int32_t windowX = parameters.at(0).int32();
            int32_t windowY = parameters.at(1).int32();
            windowWidth_ = parameters.at(2).int32();
            windowHeight_ = parameters.at(3).int32();

            window_ = SDL_CreateWindow("wasm SDL Window", windowX, windowY, windowWidth_, windowHeight_, SDL_WINDOW_SHOWN);
            if (window_ == nullptr){
                std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
                SDL_Quit();

                return (int32_t) 1;
            }

            ren = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (ren == nullptr){
                SDL_DestroyWindow(window_);
                std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                SDL_Quit();

                return (int32_t) 1;
            }
        } else {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return (int32_t) 1;
        }

        return (int32_t) 0;
    });

    addFunction("load_bmp", Int32::instance(), {Int32::instance()}, [this](std::vector<Variable> parameters) {
        SDL_Surface *bmp = SDL_LoadBMP("/home/teemperor/workspace/wasm/wasmint/examples/pong/ball.bmp");
        if (bmp == nullptr){
            std::cout << "SDL_LoadPNG Error: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(window_);
            SDL_Quit();

            return (int32_t) 0;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
        SDL_FreeSurface(bmp);

        if (tex == nullptr) {
            std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(window_);
            SDL_Quit();

            return (int32_t) 0;
        }

        int32_t index = textureIndex;
        textures_[index] = tex;

        return (int32_t) index;
    });

    addFunction("render", Void::instance(), {Int32::instance(), Int32::instance(), Int32::instance(), Int32::instance(), Int32::instance()}, [this](std::vector<Variable> parameters) {
        SDL_Rect DestR;

        DestR.x = Int32::getValue(parameters.at(1));
        DestR.y = Int32::getValue(parameters.at(2));
        DestR.w = Int32::getValue(parameters.at(3));
        DestR.h = Int32::getValue(parameters.at(4));

        SDL_RenderCopy(ren, textures_.at(Int32::getValue(parameters.at(0))), NULL, &DestR);

        return Variable::Void();
    });

    addFunction("clear", Void::instance(), {Int32::instance(), Int32::instance(), Int32::instance()}, [this](std::vector<Variable> parameters) {
        SDL_RenderClear(ren);
        return Variable::Void();
    });

    addFunction("present", Void::instance(), {Int32::instance(), Int32::instance(), Int32::instance()}, [this](std::vector<Variable> parameters) {
        SDL_RenderPresent(ren);
        return Variable::Void();
    });

    addFunction("key_down", Int32::instance(), {Int32::instance()}, [this](std::vector<Variable> parameters) {
        SDL_PumpEvents();

        const Uint8* state = SDL_GetKeyboardState(NULL);
        SDL_Scancode code = SDL_SCANCODE_0;

        int32_t param = parameters.at(0).int32();

        switch(param) {
            case 0:
                code = SDL_SCANCODE_UP;
                break;
            case 1:
                code = SDL_SCANCODE_DOWN;
                break;
            case 2:
                code = SDL_SCANCODE_ESCAPE;
                break;
            default:
                return (int32_t) 0;
        }

        if (state[code]) {
            return (int32_t) 1;
        } else {
            return (int32_t) 0;
        }
    });



    addFunction("get_window_width", Int32::instance(), {}, [this](std::vector<Variable> parameters) {
        return (int32_t) windowWidth_;
    });

    addFunction("get_window_height", Int32::instance(), {}, [this](std::vector<Variable> parameters) {
        return (int32_t) windowHeight_;
    });

    addFunction("quit", Void::instance(), {}, [this](std::vector<Variable> parameters) {
        quit();

        return Variable::Void();
    });

#endif

}

void wasmint::SDLModule::quit() {
#ifdef WASMINT_HAS_SDL
    for (auto texturePair : textures_) {
        SDL_DestroyTexture(texturePair.second);
    }
    textures_.clear();

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window_);
    SDL_Quit();
#endif
}