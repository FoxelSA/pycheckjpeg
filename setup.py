#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
  pycheckjpeg - A python module to check integrity of JPEG files

  Copyright (c) 2014 FOXEL SA - http://foxel.ch
  Please read <http://foxel.ch/license> for more information.


  Author(s):

       Kevin Velickovic <k.velickovic@foxel.ch>


  This file is part of the FOXEL project <http://foxel.ch>.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Additional Terms:

       You are required to preserve legal notices and author attributions in
       that material or in the Appropriate Legal Notices displayed by works
       containing it.

       You are required to attribute the work as explained in the "Usage and
       Attribution" section of <http://foxel.ch/license>.
"""

from distutils.core import setup, Extension

setup(
    name="pycheckjpeg", 
    version="0.1",
    ext_modules = [Extension("pycheckjpeg", ["src/pycheckjpeg.c"], libraries=['jpeg'])],
    scripts=['scripts/pycheckjpeg']
    )
