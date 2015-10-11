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

    context().name("\"sdl\"");

#ifdef WASMINT_HAS_SDL
    addFunction("\"init\"", Int32::instance(), {}, [this](std::vector<Variable> parameters) {
        if (SDL_Init(SDL_INIT_VIDEO) == 0) {
            window_ = SDL_CreateWindow("SDL Window", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
            if (window_ == nullptr){
                std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
                SDL_Quit();

                Variable variable(Int32::instance());
                Int32::setValue(variable, 1);
                return variable;
            }

            ren = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (ren == nullptr){
                SDL_DestroyWindow(window_);
                std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                SDL_Quit();
                Variable variable(Int32::instance());
                Int32::setValue(variable, 1);
                return variable;
            }
        } else {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            abort();
        }
        Variable variable(Int32::instance());
        Int32::setValue(variable, 0);
        return variable;
    });

    addFunction("\"load_bmp\"", Int32::instance(), {}, [this](std::vector<Variable> parameters) {
        SDL_Surface *bmp = SDL_LoadBMP("/home/teemperor/workspace/wasm/wasmint/examples/pong/ball.bmp");
        if (bmp == nullptr){
            std::cout << "SDL_LoadPNG Error: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(window_);
            SDL_Quit();

            Variable variable(Int32::instance());
            Int32::setValue(variable, 1);
            return variable;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
        SDL_FreeSurface(bmp);

        if (tex == nullptr) {
            std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(window_);
            SDL_Quit();

            Variable variable(Int32::instance());
            Int32::setValue(variable, 1);
            return variable;
        }

        int index = textureIndex++;
        textures_[index] = tex;

        Variable variable(Int32::instance());
        Int32::setValue(variable, 0);
        return variable;
    });

    addFunction("\"render\"", Void::instance(), {Int32::instance(), Int32::instance(), Int32::instance()}, [this](std::vector<Variable> parameters) {
        SDL_Rect DestR;

        DestR.x = Int32::getValue(parameters.at(1));
        DestR.y = Int32::getValue(parameters.at(2));
        DestR.w = 20;
        DestR.h = 20;

        std::cout << "x " << DestR.x << std::endl;
        std::cout << "y " << DestR.y << std::endl;

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, textures_.at(Int32::getValue(parameters.at(0))), NULL, &DestR);
        SDL_RenderPresent(ren);

        Variable variable(Void::instance());
        return variable;
    });

    addFunction("\"quit\"", Void::instance(), {}, [this](std::vector<Variable> parameters) {
        for (auto texturePair : textures_) {
            SDL_DestroyTexture(texturePair.second);
        }
        textures_.clear();

        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(window_);
        SDL_Quit();

        Variable variable(Void::instance());
        return variable;
    });

#endif

}