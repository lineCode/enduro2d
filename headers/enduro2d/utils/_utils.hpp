/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../base/_all.hpp"
#include "../math/_all.hpp"

namespace e2d
{
    class buffer;
    class color;
    class color32;
    class image;
    class mesh;
    class shape;
    class input_stream;
    class output_stream;
    class input_sequence;
    class output_sequence;
    class url;

    template < typename T >
    class module;

    template < typename T >
    class intrusive_ptr;

    template < typename T, typename Tag >
    class intrusive_list;

    template < typename Char >
    class basic_string_hash;
}

namespace e2d
{
    using str   = basic_string<char>;
    using wstr  = basic_string<wchar_t>;
    using str16 = basic_string<char16_t>;
    using str32 = basic_string<char32_t>;

    using str_view   = basic_string_view<char>;
    using wstr_view  = basic_string_view<wchar_t>;
    using str16_view = basic_string_view<char16_t>;
    using str32_view = basic_string_view<char32_t>;

    using str_hash = basic_string_hash<char>;
    using wstr_hash = basic_string_hash<wchar_t>;
    using str16_hash = basic_string_hash<char16_t>;
    using str32_hash = basic_string_hash<char32_t>;

    struct seconds_tag {};
    struct milliseconds_tag {};
    struct microseconds_tag {};

    template < typename T >
    using seconds = unit<T, seconds_tag>;
    template < typename T >
    using milliseconds = unit<T, milliseconds_tag>;
    template < typename T >
    using microseconds = unit<T, microseconds_tag>;
}

namespace e2d
{
    class noncopyable {
    public:
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };

    class null_disposer {
    public:
        template < typename... Args >
        void operator()(Args&&... args) const noexcept {
            E2D_UNUSED(std::forward<Args>(args)...);
        }
    };
}

namespace e2d { namespace utils
{
    //
    // sdbm_hash
    //

    namespace impl
    {
        // Inspired by:
        // http://www.cse.yorku.ca/~oz/hash.html

        template < typename Char >
        u32 sdbm_hash_impl(u32 init, const Char* str) noexcept {
            while ( Char c = *str++ ) {
                init = c + (init << 6u) + (init << 16u) - init;
            }
            return init;
        }

        template < typename Char >
        u32 sdbm_hash_impl(u32 init, const Char* begin, const Char* const end) noexcept {
            while ( begin != end ) {
                init = (*begin++) + (init << 6u) + (init << 16u) - init;
            }
            return init;
        }
    }

    template < typename Char >
    u32 sdbm_hash(const Char* str) noexcept {
        E2D_ASSERT(str);
        return impl::sdbm_hash_impl(0u, str);
    }

    template < typename Char >
    u32 sdbm_hash(const Char* begin, const Char* const end) noexcept {
        E2D_ASSERT(begin <= end);
        return impl::sdbm_hash_impl(0u, begin, end);
    }

    template < typename Char >
    u32 sdbm_hash(u32 init, const Char* str) noexcept {
        E2D_ASSERT(str);
        return impl::sdbm_hash_impl(init, str);
    }

    template < typename Char >
    u32 sdbm_hash(u32 init, const Char* begin, const Char* const end) noexcept {
        E2D_ASSERT(begin <= end);
        return impl::sdbm_hash_impl(init, begin, end);
    }

    //
    // enum_to_underlying
    //

    template < typename E
             , typename = std::enable_if<std::is_enum<E>::value> >
    constexpr std::underlying_type_t<E> enum_to_underlying(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}}
