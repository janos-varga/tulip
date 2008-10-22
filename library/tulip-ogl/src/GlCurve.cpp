#include <tulip/Size.h>
#include "tulip/Curves.h"
#include "tulip/GlCurve.h"
#include "tulip/GlTextureManager.h"
#include "tulip/GlXMLTools.h"
#include "tulip/GlTools.h"

using namespace std;

namespace tlp {
  GlCurve::GlCurve(const vector<Coord> &points, 
		   const Color &beginFColor,
		   const Color &endFColor ,
		   const float &beginSize,
		   const float &endSize):
    _points(points),
    _beginFillColor(beginFColor),
    _endFillColor(endFColor),
    _beginSize(beginSize),
    _endSize(endSize),
    texture(""){

    assert(points.size() >= 3);

    for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
      boundingBox.check(*it);
  }
  //=====================================================
  GlCurve::GlCurve(const unsigned int nbPoints):
    _points(nbPoints),
    texture(""){
  }
  //=====================================================
  GlCurve::~GlCurve() {
  }
  //=====================================================
  void GlCurve::resizePoints(const unsigned int nbPoints) {
    assert(nbPoints >= 3);
    _points.resize(nbPoints);
  }
  //=====================================================
  const tlp::Coord& GlCurve::point(const unsigned int i) const {
    return _points[i];
  }
  //=====================================================
  tlp::Coord& GlCurve::point(const unsigned int i) {
    return _points[i];
  }
  //=====================================================
  void GlCurve::draw(float lod, Camera *camera) {
    //    cerr << ".";

    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);
    vector<Coord> newPoints(_points.size());
    for(unsigned int i=0; i < _points.size(); ++i) {
      newPoints[i] = _points[i];
    }
    glLineWidth(2);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    tlp::splineLine(newPoints, 
		    _beginFillColor,
		    _endFillColor);
    glPopAttrib();

    if(texture!="") {
      GlTextureManager::getInst().activateTexture(texture);
    }

    tlp::splineQuad(newPoints, 
		    _beginFillColor,
		    _endFillColor,
		    _beginSize,
		    _endSize,
		    newPoints[0] - Coord(1.  , 0.0, 0.),
		    newPoints[3] + Coord(1.  , 0.0, 0.));

    GlTextureManager::getInst().desactivateTexture();

  }
  //=====================================================
  void GlCurve::setTexture(const std::string &texture) {
    this->texture=texture;
  }
  //=====================================================
  void GlCurve::translate(const Coord& mouvement){
    boundingBox.first+=mouvement;
    boundingBox.second+=mouvement;
    
    for(vector<Coord>::iterator it=_points.begin();it!=_points.end();++it) {
      (*it)+=mouvement;
    }
  }
  //=====================================================
  void GlCurve::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::createProperty(rootNode, "type", "GlCurve");
    
    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"points",_points);
    GlXMLTools::getXML(dataNode,"beginFillColor",_beginFillColor);
    GlXMLTools::getXML(dataNode,"endFillColor",_endFillColor);
    GlXMLTools::getXML(dataNode,"beginSize",_beginSize);
    GlXMLTools::getXML(dataNode,"endSize",_endSize);
  }
  //============================================================
  void GlCurve::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {

      GlXMLTools::setWithXML(dataNode,"points",_points);
      GlXMLTools::setWithXML(dataNode,"beginFillColor",_beginFillColor);
      GlXMLTools::setWithXML(dataNode,"endFillColor",_endFillColor);
      GlXMLTools::setWithXML(dataNode,"beginSize",_beginSize);
      GlXMLTools::setWithXML(dataNode,"endSize",_endSize);

      for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
	boundingBox.check(*it);
    }
  }
}
