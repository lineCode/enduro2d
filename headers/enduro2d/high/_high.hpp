/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../core/_all.hpp"

namespace e2d
{
    class asset;
    class library;
    class starter;

    class text_asset;
    class mesh_asset;
    class image_asset;
    class binary_asset;
    class shader_asset;
    class texture_asset;
    class material_asset;

    template < typename Asset, typename Content >
    class content_asset;

    template < typename T >
    class asset_cache;
    class asset_cache_base;
}