// Copyright (C) 2020-2025 Parabola Research Limited
// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Assert.h"

#include <type_traits>

#if defined(_MSC_VER)
	#include <intrin.h>

	static inline int __builtin_clz(unsigned int x)
	{
		unsigned long index;
		return _BitScanReverse(&index, x) ? 31 - index : 32;
	}

	static inline int __builtin_ctz(unsigned int x)
	{
		unsigned long index;
		return _BitScanForward(&index, x) ? index : 32;
	}
#endif

namespace Bungee {

template <bool floor = false>
static inline int log2(int x)
{
	BUNGEE_ASSERT1(x > 0);
	BUNGEE_ASSERT1(floor || !(x & (x << 1)));

	int y;
	if constexpr (floor)
		y = __builtin_clz(1) - __builtin_clz(x);
	else
		y = __builtin_ctz(x);

	BUNGEE_ASSERT1(floor ? (1 << y <= x && x < 2 << y) : (x == 1 << y));
	return y;
}

template <int x>
constexpr int log2(std::integral_constant<int, x>)
{
	static_assert(x > 0);
	static_assert(!(x & (x - 1)));

	if constexpr (x == 1)
		return 0;
	else
		return 1 + log2(std::integral_constant<int, x / 2>{});
}

} // namespace Bungee
