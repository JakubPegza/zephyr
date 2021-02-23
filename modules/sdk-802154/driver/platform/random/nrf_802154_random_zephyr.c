/*
 * Copyright (c) 2019 - 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Based on code written in 2016-2018 by David Blackman and Sebastiano Vigna
 * (vigna@acm.org)
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * See <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include "platform/nrf_802154_random.h"

#include <drivers/entropy.h>

static uint64_t s[2];

static inline uint64_t rotl(const uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

static uint64_t next(void)
{
	const uint64_t s0 = s[0];
	uint64_t s1 = s[1];
	const uint64_t result = s0 + s1;

	s1 ^= s0;
	s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
	s[1] = rotl(s1, 37);

	return result;
}

void nrf_802154_random_init(void)
{
	const struct device *dev;
	int err;

	dev = device_get_binding(DT_CHOSEN_ZEPHYR_ENTROPY_LABEL);
	__ASSERT_NO_MSG(dev != NULL);

	err = entropy_get_entropy(dev, (uint8_t *)&s, sizeof(s));
	__ASSERT_NO_MSG(err == 0);
}

void nrf_802154_random_deinit(void)
{
	/* Intentionally empty */
}

uint32_t nrf_802154_random_get(void)
{
	return (uint32_t)next();
}
