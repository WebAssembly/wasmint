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


#ifndef WASMINT_WIDGET_H
#define WASMINT_WIDGET_H

#include <curses.h>

class Widget {

    WINDOW* win;
    int x_, y_, w_, h_;
    int border_;

public:
    Widget(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {
        win = newwin(h, w, y, x);
    }

    int getX() const {
        return x_ + border_;
    }

    int getY() const {
        return y_ + border_;
    }

    int getWidth() const {
        return w_ - border_ * 2;
    }

    int getHeight() const {
        return h_ - border_ * 2;
    }

    virtual void setX(int x) {
        x_ = x;
    }

    virtual void setY(int y) {
        y_ = y;
    }

    virtual void setWidth(int w) {
        w_ = w;
    }

    virtual void setHeight(int h) {
        h_ = h;
    }

    virtual void render() {

    }

    virtual bool handleCharacter(int c) {

    }

    void setBorder(int border) {
        border_ = border;
    }

    WINDOW* window() {
        return win;
    }

    void print(int x, int y, char c) {
        char tmp[2];
        tmp[1] = 0;
        tmp[0] = c;
        mvprintw(y, x, tmp);
    }

    void print(int x, int y, const std::string& str) {
        mvprintw(y, x, str.c_str());
    }

};


#endif //WASMINT_WIDGET_H
