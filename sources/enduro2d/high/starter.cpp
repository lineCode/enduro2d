/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/starter.hpp>

#include <enduro2d/high/world.hpp>
#include <enduro2d/high/library.hpp>

#include <enduro2d/high/assets/atlas_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>
#include <enduro2d/high/assets/flipbook_asset.hpp>
#include <enduro2d/high/assets/image_asset.hpp>
#include <enduro2d/high/assets/material_asset.hpp>
#include <enduro2d/high/assets/mesh_asset.hpp>
#include <enduro2d/high/assets/model_asset.hpp>
#include <enduro2d/high/assets/shader_asset.hpp>
#include <enduro2d/high/assets/shape_asset.hpp>
#include <enduro2d/high/assets/sprite_asset.hpp>
#include <enduro2d/high/assets/text_asset.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>

#include <enduro2d/high/systems/flipbook_system.hpp>
#include <enduro2d/high/systems/render_system.hpp>

namespace
{
    using namespace e2d;

    template < typename Module, typename... Args >
    Module& safe_module_initialize(Args&&... args) {
        return modules::is_initialized<Module>()
            ? modules::instance<Module>()
            : modules::initialize<Module>(std::forward<Args>(args)...);
    }

    class starter_application final : public application {
    public:
        starter_application(high_application_uptr application)
        : high_application_(std::move(application)) {}

        bool initialize() final {
            ecs::registry_filler(the<world>().registry())
                .system<flipbook_system>(world::priority_update)
                .system<render_system>(world::priority_render);
            return !high_application_ || high_application_->initialize();
        }

        void shutdown() noexcept final {
            if ( high_application_ ) {
                high_application_->shutdown();
            }
        }

        bool frame_tick() final {
            the<world>().registry().process_systems_in_range(
                world::priority_update_section_begin,
                world::priority_update_section_end);
            return !the<window>().should_close()
                || (high_application_ && !high_application_->on_should_close());
        }

        void frame_render() final {
            the<world>().registry().process_systems_in_range(
                world::priority_render_section_begin,
                world::priority_render_section_end);
        }
    private:
        high_application_uptr high_application_;
    };
}

namespace e2d
{
    //
    // high_application
    //

    bool high_application::initialize() {
        return true;
    }

    void high_application::shutdown() noexcept {
    }

    bool high_application::on_should_close() {
        return true;
    }

    //
    // starter::parameters
    //

    starter::parameters::parameters(const engine::parameters& engine_params)
    : engine_params_(engine_params) {}

    starter::parameters& starter::parameters::library_root(const url& value) {
        library_root_ = value;
        return *this;
    }

    starter::parameters& starter::parameters::engine_params(const engine::parameters& value) {
        engine_params_ = value;
        return *this;
    }

    url& starter::parameters::library_root() noexcept {
        return library_root_;
    }

    engine::parameters& starter::parameters::engine_params() noexcept {
        return engine_params_;
    }

    const url& starter::parameters::library_root() const noexcept {
        return library_root_;
    }

    const engine::parameters& starter::parameters::engine_params() const noexcept {
        return engine_params_;
    }

    //
    // starter
    //

    starter::starter(int argc, char *argv[], const parameters& params) {
        safe_module_initialize<engine>(argc, argv, params.engine_params());
        safe_module_initialize<library>(params.library_root());
        safe_module_initialize<asset_cache<atlas_asset>>(the<library>());
        safe_module_initialize<asset_cache<binary_asset>>(the<library>());
        safe_module_initialize<asset_cache<flipbook_asset>>(the<library>());
        safe_module_initialize<asset_cache<image_asset>>(the<library>());
        safe_module_initialize<asset_cache<material_asset>>(the<library>());
        safe_module_initialize<asset_cache<mesh_asset>>(the<library>());
        safe_module_initialize<asset_cache<model_asset>>(the<library>());
        safe_module_initialize<asset_cache<shader_asset>>(the<library>());
        safe_module_initialize<asset_cache<shape_asset>>(the<library>());
        safe_module_initialize<asset_cache<sprite_asset>>(the<library>());
        safe_module_initialize<asset_cache<text_asset>>(the<library>());
        safe_module_initialize<asset_cache<texture_asset>>(the<library>());
        safe_module_initialize<asset_factory>()
            .register_asset<atlas_asset>("atlas")
            .register_asset<binary_asset>("binary")
            .register_asset<flipbook_asset>("flipbook")
            .register_asset<image_asset>("image")
            .register_asset<material_asset>("material")
            .register_asset<mesh_asset>("mesh")
            .register_asset<model_asset>("model")
            .register_asset<shader_asset>("shader")
            .register_asset<shape_asset>("shape")
            .register_asset<sprite_asset>("sprite")
            .register_asset<text_asset>("text")
            .register_asset<texture_asset>("texture");
        safe_module_initialize<world>();
    }

    starter::~starter() noexcept {
        modules::shutdown<world>();
        modules::shutdown<asset_factory>();
        modules::shutdown<asset_cache<texture_asset>>();
        modules::shutdown<asset_cache<text_asset>>();
        modules::shutdown<asset_cache<sprite_asset>>();
        modules::shutdown<asset_cache<shape_asset>>();
        modules::shutdown<asset_cache<shader_asset>>();
        modules::shutdown<asset_cache<model_asset>>();
        modules::shutdown<asset_cache<mesh_asset>>();
        modules::shutdown<asset_cache<material_asset>>();
        modules::shutdown<asset_cache<image_asset>>();
        modules::shutdown<asset_cache<flipbook_asset>>();
        modules::shutdown<asset_cache<binary_asset>>();
        modules::shutdown<asset_cache<atlas_asset>>();
        modules::shutdown<library>();
        modules::shutdown<engine>();
    }

    bool starter::start(high_application_uptr app) {
        return the<engine>().start(
            std::make_unique<starter_application>(
                std::move(app)));
    }
}
