# Copyright (c) 2021 Alistair Bell <alistair@alistairbell.xyz> 
#
# HolyC compiler is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


# C Compiler flags for the sources.
CONFIG_CC_FLAGS = -std=c89 -Wextra -Wall -g -Wno-unused-parameter -Wno-implicit-fallthrough
# Final linking flags, -static for no shared libraries.
CONFIG_LD_FLAGS =
# Specify any other libraries, -lmusl for musl based binary.
CONFIG_LD_LIBS  =
