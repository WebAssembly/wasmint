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

#include <string>
#include <curses.h>
#include <vector>

class Widget {

    WINDOW* win;
    int x_, y_, w_, h_;
    int border_;
    std::string name_;

    std::vector<Widget*> children_;
    bool hidden_ = false;

public:
    Widget(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {
        win = newwin(h, w, y, x);
    }

    void setName(const std::string& name) {
        name_ = name;
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

    void hide() {
        hidden_ = true;
    }

    void show() {
        hidden_ = false;
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

    void addChild(Widget* child) {
        children_.push_back(child);
    }

    void clearChildren() {
        children_.clear();
    }

    void draw() {
        if (hidden_)
            return;

        if (border_ != 0) {
            box(win, 0, 0);
            mvwprintw(win, 0, getWidth() / 2 - name_.size() / 2, name_.c_str());
        }
        render();
        display();
        for (Widget* child : children_) {
            child->draw();
        }
        refresh();
    }

    virtual void render() {
    }

    virtual bool handleCharacter(int c) {
        return false;
    }

    bool doHandleCharacter(int c) {
        for (Widget* child : children_) {
            if (!child->hidden_) {
                if (child->doHandleCharacter(c)) {
                    return true;
                }
            }
        }
        return handleCharacter(c);
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
        mvwprintw(win, y + border_, x + border_, tmp);
    }

    void print(int x, int y, char c, int formatting) {
        char tmp[2];
        tmp[1] = 0;
        tmp[0] = c;
        wattron(win, formatting);
        mvwprintw(win, y + border_, x + border_, tmp);
        wattroff(win, formatting);
    }

    void print(int x, int y, const std::string& str) {
        mvwprintw(win, y + border_, x + border_, str.c_str());
    }

    void print(int x, int y, const std::string& str, int formatting) {
        wattron(win, formatting);
        mvwprintw(win, y + border_, x + border_, str.c_str());
        wattroff(win, formatting);
    }

    void display() {
        wnoutrefresh(win);
    }
};


#endif //WASMINT_WIDGET_H
