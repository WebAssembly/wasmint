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


#ifndef WASMINT_INSTRUCTIONVIEWER_H
#define WASMINT_INSTRUCTIONVIEWER_H


class InstructionViewer : public Widget {

    const wasm_module::Instruction* instruction_ = nullptr;
    const wasm_module::Instruction* selectedInstruction_ = nullptr;

    int renderInstruction(const wasm_module::Instruction* instruction, int x, int y) {
        int height = 1;

        if (instruction == selectedInstruction_)
            print(x, y, instruction->dataString(), A_REVERSE);
        else
            print(x, y, instruction->dataString());

        for (const wasm_module::Instruction* child : instruction->children()) {
            int h = renderInstruction(child, x + 4, y + 1);
            height += h;
            y += h;
        }
        return height;
    }

    void goHigher() {
        if (selectedInstruction_->hasParent())
            selectedInstruction_ = selectedInstruction_->parent();
    }

    void goDeeper() {
        if (!selectedInstruction_->children().empty())
            selectedInstruction_ = selectedInstruction_->children().at(0);
        else
            goDown();

    }

    void goDown() {
        if (selectedInstruction_->hasParent()) {
            std::size_t index = selectedInstruction_->parent()->getChildIndex(selectedInstruction_);
            if (index + 1 < selectedInstruction_->parent()->children().size()) {
                selectedInstruction_ = selectedInstruction_->parent()->children().at(index + 1);
            } else {
                goHigher();
                goDown();
            }
        }
    }

    void goUp() {
        if (selectedInstruction_->hasParent()) {
            std::size_t index = selectedInstruction_->parent()->getChildIndex(selectedInstruction_);
            if (index > 0) {
                selectedInstruction_ = selectedInstruction_->parent()->children().at(index - 1);
            } else {
                goHigher();
            }
        }
    }

public:
    InstructionViewer() : Widget(68, 2, 60, 20) {
        setBorder(1);
        setName("Instruction");
    }

    void setInstruction(const wasm_module::Instruction* instruction) {
        instruction_ = instruction;
        selectedInstruction_ = instruction;
    }

    virtual void render() override {
        renderInstruction(instruction_, 0, 0);
        refresh();
    }

    virtual bool handleCharacter(int c) override {
        if (c == KEY_LEFT) {
            goHigher();
            return true;
        }
        if (c == KEY_RIGHT) {
            goDeeper();
            return true;
        }
        if (c == KEY_DOWN) {
            goDown();
            return true;
        }
        if (c == KEY_UP) {
            goUp();
            return true;
        }
        return false;
    }

};


#endif //WASMINT_INSTRUCTIONVIEWER_H
