/*
 * This file is part of the Orchid libraries
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


#include "scriptplugin.h"

#include <stem/resourcefactory.h>
#include "scriptedresource.h"

using namespace Orchid;

ScriptPlugin::ScriptPlugin() {
	m_helpers << new ResourceFactoryHelper<ScriptedResource>();
}

ScriptPlugin::~ScriptPlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> ScriptPlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(orchid_scriptedres_extension, ScriptPlugin)
