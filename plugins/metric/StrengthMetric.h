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
#ifndef _STRENGTHMETRIC_H
#define _STRENGTHMETRIC_H
#include <tulip/tuliphash.h>
#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/** This plugin is an implementation of the Strength Parameter.
 *  algorithm first published in :
 *
 *  Y. Chiricota. F.Jourdan, an G.Melancon\n
 *  "Software component capture using graph clustering", \n
 *  "IWPC", \n
 *   2002.
 *
 *  <b>HISTORY</b>
 *
 *  - 26/02/2002 Verson 0.0.1: First implementation.
 *
 *  - 27/09/2005 Version 0.0.2: Fix bar normalisation of the parameter (error in the paper).
 *
 *  \note This algorithm works on general graphs. 
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class StrengthMetric:public tlp::DoubleAlgorithm { 
public:
  StrengthMetric(const tlp::PropertyContext &);
  ~StrengthMetric();
  double getNodeValue(const tlp::node n);
  double getEdgeValue(const tlp::edge n);
  bool run();
private:
  double e(const TLP_HASH_SET<tlp::node> &U);
  double e(TLP_HASH_SET<tlp::node> &U, TLP_HASH_SET<tlp::node> &V);
  double s(TLP_HASH_SET<tlp::node> &U, TLP_HASH_SET<tlp::node> &V);
  double s(const TLP_HASH_SET<tlp::node> &U);
};
/*@}*/
#endif
