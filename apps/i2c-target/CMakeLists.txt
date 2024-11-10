# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(BOARD_FLASH_RUNNER jlink)
set(BOARD_DEBUG_RUNNER jlink)

#set(ZEPHYR_MODULES ..)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(i2c-target LANGUAGES C)

target_sources(i2c-target PRIVATE src/main.c)
