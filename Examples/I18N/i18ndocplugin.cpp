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


#include "i18ndocplugin.h"

#include "i18ndocplugin.moc"

#include <stem/resourcefactory.h>
#include "i18ndocsample.h"

using namespace Orchid;

I18nDocPlugin::I18nDocPlugin() {
	m_helpers << new ResourceFactoryHelper<I18nDocSample>();
}

I18nDocPlugin::~I18nDocPlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> I18nDocPlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(i18n_document_sample_extension, I18nDocPlugin)
