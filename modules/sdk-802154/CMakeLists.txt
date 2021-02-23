# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

add_subdirectory_ifdef(CONFIG_NRF_802154_RADIO_DRIVER  sl)
add_subdirectory_ifdef(CONFIG_NRF_802154_RADIO_DRIVER  driver)
add_subdirectory_ifdef(CONFIG_NRF_802154_SERIALIZATION serialization)
