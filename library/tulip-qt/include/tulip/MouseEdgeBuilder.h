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
#ifndef MOUSEADDEDGE_H
#define MOUSEADDEDGE_H
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Coord.h>
#include <tulip/InteractorComponent.h>

namespace tlp {

/** \addtogroup Mouse_interactor */
/*@{*/
class TLP_QT_SCOPE MouseEdgeBuilder:public InteractorComponent
{
 private:
  node source;
  bool started;
  Coord startPos,curPos;
  std::vector<Coord> bends;

 public:
  MouseEdgeBuilder();
  ~MouseEdgeBuilder(){}
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseEdgeBuilder(); }
};
/*@}*/

}
#endif
