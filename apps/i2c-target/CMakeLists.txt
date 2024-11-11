# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(BOARD_FLASH_RUNNER jlink)
set(BOARD_DEBUG_RUNNER jlink)

#set(ZEPHYR_MODULES ..)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

target_sources(app PRIVATE src/main.c)
