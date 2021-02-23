set(NRF_802154_SL_ROOT ${ZEPHYR_CURRENT_MODULE_DIR}/sl)

include(${NRF_802154_SL_ROOT}/nrf_802154_sl_files.cmake)

zephyr_library()

zephyr_library_sources(
  ${CMAKE_CURRENT_SOURCE_DIR}/platform/clock/nrf_802154_clock_zephyr.c
  ${CMAKE_CURRENT_SOURCE_DIR}/platform/irq/nrf_802154_irq_zephyr.c
  ${CMAKE_CURRENT_SOURCE_DIR}/platform/gpiote/nrf_802154_gpiote_zephyr.c
)

function(sl_calculate_lib_path lib_path)


endfunction()

if (CONFIG_NRF_802154_SL_OPENSOURCE)
  zephyr_include_directories(${NRF_802154_SL_OPENSOURCE_INCLUDE_DIRS})
  zephyr_library_sources(${NRF_802154_SL_OPENSOURCE_SOURCES})

else () # CONFIG_NRF_802154_SL_OPENSOURCE
  zephyr_include_directories(${NRF_802154_SL_INCLUDE_DIRS} include)
  zephyr_library_sources_ifdef(CONFIG_SOC_SERIES_NRF52X ${NRF_802154_SL_SOURCES_NRF52})
  zephyr_library_sources_ifdef(
    CONFIG_SOC_SERIES_NRF53X
    ${NRF_802154_SL_SOURCES_NRF53}
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/lp_timer/nrf_802154_lp_timer_zephyr.c)

  # Generate library path
  # CMake regex does not support {4}
  string(REGEX REPLACE "_[a-zA-Z][a-zA-Z][a-zA-Z][a-zA-Z]$" "" arch_soc_dir ${CONFIG_SOC})

  # Set floating ABI
  if(CONFIG_FPU)
    if(CONFIG_FP_HARDABI)
      set(float_dir hard-float)
    elseif(CONFIG_FP_SOFTABI)
      set(float_dir softfp-float)
    else()
      assert(0 "Unreachable code")
    endif()
  else()
    set(float_dir soft-float)
  endif()

  set(NRF_802154_SL_LIB_PATH ${NRF_802154_SL_ROOT}/lib/${arch_soc_dir}/${float_dir})

  if(NOT EXISTS ${NRF_802154_SL_LIB_PATH})
    message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_802154_sl lib."
                    "(${NRF_802154_SL_LIB_PATH} doesn't exist.)")
  endif() # NOT EXISTS NRF_802154_SL_LIB_PATH

  zephyr_link_libraries(${NRF_802154_SL_LIB_PATH}/libnrf_802154_sl.a)

endif () # CONFIG_NRF_802154_SL_OPENSOURCE
