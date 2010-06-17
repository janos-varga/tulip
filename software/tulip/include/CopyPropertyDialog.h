/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef COPYPROPERTYDIALOG_H
#define COPYPROPERTYDIALOG_H
#include <string>
#include <vector>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif

#include "CopyPropertyDialogData.h"
#include "tulip/Iterator.h"

class CopyPropertyDialog : public QDialog, public Ui::CopyPropertyDialogData
{ 
  Q_OBJECT;

public:
  CopyPropertyDialog(QWidget *parent = 0);
  void setProperties(std::string& srcProp,
		     std::vector<std::string>& localProps,
		     std::vector<std::string>& inheritedProps);
  enum destType {NEW, LOCAL, INHERITED};
  std::string getDestinationProperty(destType&);
  
public slots:  
  void setNew( bool );
  void setLocal( bool );
  void setInherited( bool );
};

#endif // COPYPROPERTYDIALOG_H
