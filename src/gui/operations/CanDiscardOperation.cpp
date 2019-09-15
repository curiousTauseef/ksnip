/*
 * Copyright (C) 2019 Damir Porobic <damir.porobic@gmx.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "CanDiscardOperation.h"

CanDiscardOperation::CanDiscardOperation(QWidget *parent, const QImage &image, bool isUnsaved)
{
	mParent = parent;
	mImage = image;
	mIsUnsaved = isUnsaved;
	mConfig = KsnipConfig::instance();
}

bool CanDiscardOperation::execute()
{
	if (mConfig->promptSaveBeforeExit() && mIsUnsaved) {
		auto saveBeforeDiscardResponse = getSaveBeforeDiscard();

		if (saveBeforeDiscardResponse == MessageBoxResponse::Yes) {
			return saveImage();
		} else if (saveBeforeDiscardResponse == MessageBoxResponse::Cancel) {
			return false;
		}
	}

	return true;
}

bool CanDiscardOperation::saveImage() const
{
	auto saveOperation = SaveOperation(mParent, mImage, mConfig->useInstantSave());
	return saveOperation.execute();
}

MessageBoxResponse CanDiscardOperation::getSaveBeforeDiscard() const
{
	return MessageBoxHelper::yesNoCancel(QCoreApplication::translate("CanDiscardOperation", "Warning - ") + QApplication::applicationName(),
		                               QCoreApplication::translate("CanDiscardOperation", "The capture has been modified.\nDo you want to save it?"));
}
