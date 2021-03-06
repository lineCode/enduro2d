/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../assets/flipbook_asset.hpp"

namespace e2d
{
    class flipbook_source final {
    public:
        flipbook_source() = default;
        flipbook_source(const flipbook_asset::ptr& flipbook);

        flipbook_source& flipbook(const flipbook_asset::ptr& value) noexcept;
        const flipbook_asset::ptr& flipbook() const noexcept;
    private:
        flipbook_asset::ptr flipbook_;
    };
}

namespace e2d
{
    inline flipbook_source::flipbook_source(const flipbook_asset::ptr& flipbook)
    : flipbook_(flipbook) {}

    inline flipbook_source& flipbook_source::flipbook(const flipbook_asset::ptr& value) noexcept {
        flipbook_ = value;
        return *this;
    }

    inline const flipbook_asset::ptr& flipbook_source::flipbook() const noexcept {
        return flipbook_;
    }
}
