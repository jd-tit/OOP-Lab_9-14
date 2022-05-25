//
// Created by jdt on 4/28/2022.
//

#include "../include/undo_action.hpp"

void UndoDelete::doUndo() {
    undoData.repo.insert_at(undoData.element, undoData.index);
}

void UndoModify::doUndo() {
    auto& element = undoData.repo.at(undoData.index);
    element = undoData.element;
}

void UndoAdd::doUndo() {
    undoData.repo.remove_at(undoData.index);
}