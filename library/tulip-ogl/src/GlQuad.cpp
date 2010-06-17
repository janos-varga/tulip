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
//*********************************************************************************
//** GlQuad.cpp : Source for a general quad display class
//**
//** author : Delorme Maxime
//** date   : 07/07/05
//*********************************************************************************
#include "tulip/GlQuad.h"

#include "tulip/GlTextureManager.h"
#include "tulip/GlXMLTools.h"
#include "tulip/GlTools.h"

using namespace std;

namespace tlp {

GlQuad::GlQuad():textureName("")
{
  // No default constructor :)
}

void GlQuad::setPosition(const Coord&)
{
  // No setPosition
}

void GlQuad::setColor(const Color& color)
{
  for(int i=0; i < N_QUAD_POINTS; i++) {
    *(this->colors[i]) = color;
  }
}

GlQuad::GlQuad(Coord positions[N_QUAD_POINTS], const Color &color)
{
  for(int i=0; i < N_QUAD_POINTS; i++)
    {
      this->positions[i] = new Coord(positions[i]);
      this->colors[i]    = new Color(color);
      boundingBox.expand(*this->positions[i]);
    }
}

GlQuad::GlQuad(Coord positions[N_QUAD_POINTS], Color colors[N_QUAD_POINTS])
{
  for(int i=0; i < N_QUAD_POINTS; i++) {
    this->positions[i] = new Coord(positions[i]);
    this->colors[i]    = new Color(colors[i]);

    boundingBox.expand(*this->positions[i]);
  }
}

GlQuad::~GlQuad()
{
  for(int i=0; i < N_QUAD_POINTS; i++)
    {
      delete positions[i];
      delete colors[i];
    }
}

void GlQuad::setPosition(int idPosition, const Coord &position)
{
  if (idPosition < 0 || idPosition >= N_QUAD_POINTS)
    return;

  delete this->positions[idPosition];

  this->positions[idPosition] = new Coord(position);
  boundingBox=BoundingBox();
  for(int i=0;i<N_QUAD_POINTS;++i)
    boundingBox.expand(*positions[i]);
}

void GlQuad::setColor(int idColor, const Color &color)
{
  if (idColor < 0 || idColor >= N_QUAD_POINTS)
    return;

  delete this->colors[idColor];

  this->colors[idColor] = new Color(color);
}

Coord* GlQuad::getPosition(int idPosition) const
{
  if (idPosition < 0 || idPosition >= N_QUAD_POINTS)
    return NULL;

  return positions[idPosition];
}

Color* GlQuad::getColor(int idColor) const
{
  if (idColor < 0 || idColor >= N_QUAD_POINTS)
    return NULL;

  return colors[idColor];
}

void GlQuad::setTextureName(const string &name) {
  textureName=name;
}

string GlQuad::getTextureName() const {
  return textureName;
}

void GlQuad::draw(float, Camera *)
{

  if(textureName!="") {
    GlTextureManager::getInst().activateTexture(textureName);
  }

  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  setMaterial(*colors[0]);
  glVertex3f(positions[0]->getX(), positions[0]->getY(), positions[0]->getZ());
  glTexCoord2f(1.0f, 0.0f);
  setMaterial(*colors[1]);
  glVertex3f(positions[1]->getX(), positions[1]->getY(), positions[1]->getZ());
  glTexCoord2f(1.0f, 1.0f);
  setMaterial(*colors[2]);
  glVertex3f(positions[2]->getX(), positions[2]->getY(), positions[2]->getZ());
  glTexCoord2f(0.0f, 1.0f);
  setMaterial(*colors[3]);
  glVertex3f(positions[3]->getX(), positions[3]->getY(), positions[3]->getZ());

  glEnd();
  glEnable(GL_CULL_FACE);

  GlTextureManager::getInst().desactivateTexture();

}
//===========================================================
void GlQuad::translate(const Coord& mouvement) {
  boundingBox.translate(mouvement);

  for(int i=0; i < N_QUAD_POINTS; i++) {
    *positions[i]+=mouvement;
  }
}
//===========================================================
void GlQuad::getXML(xmlNodePtr rootNode) {
  xmlNodePtr dataNode=NULL;

  GlXMLTools::createProperty(rootNode, "type", "GlQuad");

  GlXMLTools::getDataNode(rootNode,dataNode);

  GlXMLTools::getXML(dataNode,"position0",positions[0]);
  GlXMLTools::getXML(dataNode,"position1",positions[1]);
  GlXMLTools::getXML(dataNode,"position2",positions[2]);
  GlXMLTools::getXML(dataNode,"position3",positions[3]);
  GlXMLTools::getXML(dataNode,"color0",colors[0]);
  GlXMLTools::getXML(dataNode,"color1",colors[1]);
  GlXMLTools::getXML(dataNode,"color2",colors[2]);
  GlXMLTools::getXML(dataNode,"color3",colors[3]);


  }
  //============================================================
  void GlQuad::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      for(int i=0; i < N_QUAD_POINTS; i++) {
	positions[i]=new Coord();
	colors[i]=new Color();
      }
      GlXMLTools::setWithXML(dataNode,"position0",*positions[0]);
      GlXMLTools::setWithXML(dataNode,"position1",*positions[1]);
      GlXMLTools::setWithXML(dataNode,"position2",*positions[2]);
      GlXMLTools::setWithXML(dataNode,"position3",*positions[3]);
      GlXMLTools::setWithXML(dataNode,"color0",*colors[0]);
      GlXMLTools::setWithXML(dataNode,"color1",*colors[1]);
      GlXMLTools::setWithXML(dataNode,"color2",*colors[2]);
      GlXMLTools::setWithXML(dataNode,"color3",*colors[3]);

      for(int i=0; i < N_QUAD_POINTS; i++) {
        boundingBox.expand(*this->positions[i]);
      }
    }
  }

}
