/*
 * This file is part of the Orchid example plugins
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "gallerydemo.h"

GalleryDemo::GalleryDemo() {
	setTitle("Hardcoded Gallery Demo");
	insertFile("flower.jpg", ":/flower.jpg");
	insertFile("flower2.jpg", ":/flower2.jpg");
	insertFile("forest.jpg", ":/forest.jpg");
	insertFile("geranium.jpg", ":/geranium.jpg");
	insertFile("tree.jpg", ":/tree.jpg");
}
