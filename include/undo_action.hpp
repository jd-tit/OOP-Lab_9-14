#pragma once

#include <utility>

#include "domain.h"
#include "repo.h"

struct UndoData {
    Course element;
    size_t index;
    Repo<Course>& repo;
};

class UndoAction {
public:
    virtual void doUndo() = 0;

    explicit UndoAction(UndoData data) : undoData(std::move(data)) {}
    virtual ~UndoAction() = default;
    UndoData undoData;
};


class UndoAdd : public UndoAction {
public:
    using UndoAction::UndoAction;
    void doUndo() override;
    ~UndoAdd() override = default;
};

class UndoModify : public UndoAction {
public:
    using UndoAction::UndoAction;
    void doUndo() override;
    ~UndoModify() override = default;
};

class UndoDelete : public UndoAction {
public:
    using UndoAction::UndoAction;
    void doUndo() override;
    ~UndoDelete() override = default;
};


