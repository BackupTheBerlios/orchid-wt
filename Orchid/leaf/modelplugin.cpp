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


#include "modelplugin.h"

#include "modelplugin.moc"

#include <stem/resourcefactory.h>
#include <leaf/xmlmodelresource.h>

using namespace Orchid;

ModelPlugin::ModelPlugin() {
	m_helpers << new ResourceFactoryHelper<ModelResource>();
	m_helpers << new ResourceFactoryHelper<XmlModelResource>();
}

ModelPlugin::~ModelPlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> ModelPlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(orchid_modelres_extension, ModelPlugin)
