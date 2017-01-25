#pragma once

#include "Def.hpp"
#include <intrin.h>
#include <cassert>
#include <cstdint>
#include <gsl/gsl_assert>

namespace Fancy::Simd
{
    using Floats = __m128;
    using Ints = __m128i;
    using Doubles = __m128d;

    template<typename T>
    void EnsureAligned(T* p)
    {
        Ensures((reinterpret_cast<std::uintptr_t>(p) & 0x7u) == 0);
    }

    inline Floats __vectorcall MakeFloats(float x, float y, float z, float w) noexcept
    {
        return _mm_set_ps(w, z, y, x);
    }

    inline Floats MakeFloats(float x) noexcept
    {
        return _mm_set_ps1(x);
    }

    inline Ints MakeInts(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t w) noexcept
    {
        return _mm_set_epi32(static_cast<int>(w), static_cast<int>(z), 
            static_cast<int>(y), static_cast<int>(x));
    }

    inline Ints MakeInts(std::int32_t x) noexcept
    {
        return _mm_set1_epi32(x);
    }

    inline Doubles MakeDoubles(double x, double y) noexcept
    {
        return _mm_set_pd(x, y);
    }

    //no-op
    inline Floats AsFloats(Ints ints) noexcept
    {
        return _mm_castsi128_ps(ints);
    }

    //no-op
    inline Ints AsInts(Floats floats) noexcept
    {
        return _mm_castps_si128(floats);
    }

    template<std::uint32_t u0, std::uint32_t u1, std::uint32_t u2, std::uint32_t u3>
    inline Floats __vectorcall PermuteFloats(Floats floats) noexcept
    {
        static_assert(u0 <= 3 && u1 <= 3 && u2 <= 3 && u3 <= 3, "Indexes should be less or equal than 3.");
        return _mm_permute_ps(floats, static_cast<int/*std::int32_t*/>(u3 << 6 | u2 << 4 | u1 << 2 | u0));
    }

    inline Floats __vectorcall DupX(Floats floats) noexcept
    {
        return PermuteFloats<0, 0, 0, 0>(floats);
    }

    inline Floats __vectorcall DupY(Floats floats) noexcept
    {
        return PermuteFloats<1, 1, 1, 1>(floats);
    }

    inline Floats __vectorcall DupZ(Floats floats) noexcept
    {
        return PermuteFloats<2, 2, 2, 2>(floats);
    }

    inline Floats __vectorcall DupW(Floats floats) noexcept
    {
        return PermuteFloats<3, 3, 3, 3>(floats);
    }

    template<std::uint32_t u0, std::uint32_t u1, std::uint32_t u2, std::uint32_t u3>
    inline Ints __vectorcall PermuteInts(Ints ints) noexcept
    {
        static_assert(u0 <= 3 && u1 <= 3 && u2 <= 3 && u3 <= 3, "Indexes should be less or equal than 3.");
        return _mm_shuffle_epi32(ints, static_cast<int/*std::int32_t*/>(u3 << 6 | u2 << 4 | u1 << 2 | u0));
    }

    inline Floats ZeroFloats() noexcept
    {
        return _mm_setzero_ps();
    }

    inline Ints ZeroInts() noexcept
    {
        return _mm_setzero_si128();
    }

    inline Doubles ZeroDoubles() noexcept
    {
        return _mm_setzero_pd();
    }

    DefConstInlineVar(Floats, UintToFloatBias,
        MakeFloats(32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f));

    DefConstInlineVar(Ints, SignBitMask, MakeInts(0x80000000, 0x80000000, 0x80000000, 0x80000000));

    DefConstInlineVar(Ints, NumBitMask, MakeInts(0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF));

    DefConstInlineVar(Ints, ZeroedInts, ZeroInts());

    DefConstInlineVar(Floats, ZeroedFloats, ZeroFloats());

    DefConstInlineVar(Doubles, ZeroedDoubles, ZeroDoubles());

    inline void Store(Floats floats, float* p)
    {
        _mm_storeu_ps(p, floats);
    }

    inline void StoreAligned(Floats floats, float* p)
    {
        EnsureAligned(p);
        _mm_store_ps(p, floats);
    }

    inline float First(Floats floats) noexcept
    {
        return _mm_cvtss_f32(floats);
    }

    namespace Internal
    {
        inline std::int32_t CastToInt(float f) noexcept
        {
            return *reinterpret_cast<std::int32_t*>(reinterpret_cast<char*>(&f));
        }
    }

    inline std::int32_t First(Ints ints) noexcept
    {
        return Internal::CastToInt(First(AsFloats(ints)));
    }

    inline float Second(Floats floats) noexcept
    {
        return First(PermuteFloats<1, 0, 0, 0>(floats));
    }

    inline std::int32_t Second(Ints ints) noexcept
    {
        return Internal::CastToInt(Second(AsFloats(ints)));
    }

    inline float Third(Floats floats)
    {
        return First(PermuteFloats<2, 0, 0, 0>(floats));
    }

    inline std::int32_t Third(Ints ints) noexcept
    {
        return Internal::CastToInt(Third(AsFloats(ints)));
    }

    inline float Fourth(Floats floats) noexcept
    {
        return First(PermuteFloats<3, 0, 0, 0>(floats));
    }

    inline std::int32_t Fourth(Ints ints) noexcept
    {
        return Internal::CastToInt(Fourth(AsFloats(ints)));
    }

    inline Floats SetFirst(Floats floats, float f) noexcept
    {
        return _mm_move_ss(floats, _mm_set_ss(f));
    }

    inline Floats SetSecond(Floats floats, float f)
    {
        const auto temp = MakeFloats(f);
        return _mm_insert_ps(floats, temp, 0x10);
    }

    inline Floats SetThird(Floats floats, float f) noexcept
    {
        const auto temp = MakeFloats(f);
        return _mm_insert_ps(floats, temp, 0x20);
    }

    inline Floats SetFourth(Floats floats, float f) noexcept
    {
        const auto temp = MakeFloats(f);
        return _mm_insert_ps(floats, temp, 0x30);
    }

    inline Floats Min(Floats lhs, Floats rhs) noexcept
    {
        return _mm_min_ps(lhs, rhs);
    }

    inline Floats Max(Floats lhs, Floats rhs) noexcept
    {
        return _mm_max_ps(lhs, rhs);
    }

    inline Floats Less(Floats lhs, Floats rhs) noexcept
    {
        return _mm_cmple_ps(lhs, rhs);
    }

    inline Floats Greater(Floats lhs, Floats rhs) noexcept
    {
        return _mm_cmpge_ps(lhs, rhs);
    }

    inline Floats Equal(Floats lhs, Floats rhs) noexcept
    {
        return _mm_cmpeq_ps(lhs, rhs);
    }

    inline Floats NotEqual(Floats lhs, Floats rhs) noexcept
    {
        return _mm_cmpneq_ps(lhs, rhs);
    }

    inline std::uint8_t __vectorcall Msbs(Floats floats) noexcept
    {
        return static_cast<std::uint8_t>(_mm_movemask_ps(floats));
    }

    inline Floats __vectorcall Add(Floats lhs, Floats rhs) noexcept
    {
        return _mm_add_ps(lhs, rhs);
    }

    inline Ints __vectorcall Add(Ints lhs, Ints rhs) noexcept
    {
        return _mm_add_epi32(lhs, rhs);
    }

    inline Doubles __vectorcall Add(Doubles lhs, Doubles rhs) noexcept
    {
        return _mm_add_pd(lhs, rhs);
    }

    inline Floats __vectorcall Sub(Floats lhs, Floats rhs) noexcept
    {
        return _mm_sub_ps(lhs, rhs);
    }

    inline Ints __vectorcall Sub(Ints lhs, Ints rhs) noexcept
    {
        return _mm_sub_epi32(lhs, rhs);
    }

    inline Doubles __vectorcall Sub(Doubles lhs, Doubles rhs) noexcept
    {
        return _mm_sub_pd(lhs, rhs);
    }

    inline Floats __vectorcall Mul(Floats lhs, Floats rhs) noexcept
    {
        return _mm_mul_ps(lhs, rhs);
    }

    inline Ints __vectorcall Mul(Ints lhs, Ints rhs) noexcept
    {
        return _mm_mul_epi32(lhs, rhs);
    }

    inline Doubles __vectorcall Mul(Doubles lhs, Doubles rhs) noexcept
    {
        return _mm_mul_pd(lhs, rhs);
    }

    inline Floats __vectorcall Div(Floats lhs, Floats rhs) noexcept
    {
        return _mm_div_ps(lhs, rhs);
    }

    inline Floats __vectorcall Sqrt(Floats floats) noexcept
    {
        return _mm_sqrt_ps(floats);
    }

    inline Floats __vectorcall Neg(Floats floats) noexcept
    {
        return Sub(ZeroFloats(), floats);
    }

    inline Floats __vectorcall And(Floats lhs, Floats rhs) noexcept
    {
        return _mm_and_ps(lhs, rhs);
    }

    inline Ints __vectorcall And(Ints lhs, Ints rhs) noexcept
    {
        return _mm_and_si128(lhs, rhs);
    }

    inline Doubles __vectorcall And(Doubles lhs, Doubles rhs) noexcept
    {
        return _mm_and_pd(lhs, rhs);
    }

    inline Floats __vectorcall NotAnd(Floats lhs, Floats rhs) noexcept
    {
        return _mm_andnot_ps(lhs, rhs);
    }

    inline Floats __vectorcall Or(Floats lhs, Floats rhs) noexcept
    {
        return _mm_or_ps(lhs, rhs);
    }
    
    namespace Internal
    {
        inline constexpr std::uint8_t CastToUint8(bool b) noexcept
        {
            return b ? 1 : 0;
        }
    }

    template<bool b1, bool b2, bool b3, bool b4>
    inline Floats __vectorcall Dot(Floats lhs, Floats rhs) noexcept
    {
        return _mm_dp_ps(lhs, rhs, 0xF | ((Internal::CastToUint8(b1) | Internal::CastToUint8(b2) << 1
            | Internal::CastToUint8(b3) << 2 | Internal::CastToUint8(b4) << 3) << 4));
    }

    inline Floats FloatsFromMemory(const float* p) noexcept
    {
        return _mm_loadu_ps(p);
    }

    inline Floats FloatsFromAlignedMemory(const float* p) noexcept
    {
        return _mm_load_ps(p);
    }

    inline Floats FloatsFromInts(Ints ints) noexcept
    {
        return _mm_cvtepi32_ps(ints);
    }

    inline Floats FloatsFromUints(__m128i uints) noexcept
    {
        auto sMask = And(uints, SignBitMask);
        auto lowBits = And(uints, NumBitMask);
        return Add(AsFloats(lowBits), And(AsFloats(_mm_srai_epi32(sMask, 31)), UintToFloatBias));
    }

    inline Ints IntsFromAlignedMemory(const std::int32_t* mem) noexcept
    {
        assert((reinterpret_cast<std::uintptr_t>(mem) & 7u) == 0);
        return _mm_load_si128(reinterpret_cast<const __m128i*>(mem));
    }

    inline Floats FloatsFromAlignedMemory(const std::int32_t* mem) noexcept
    {
        assert((reinterpret_cast<std::uintptr_t>(mem) & 7u) == 0);
        return FloatsFromInts(IntsFromAlignedMemory(mem));
    }

    /*inline Ints IntsFromMemory(const std::int32_t* mem) noexcept
    {
        return 
    }*/
}