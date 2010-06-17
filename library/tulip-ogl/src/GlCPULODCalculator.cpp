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
#include "tulip/GlCPULODCalculator.h"

#include <tulip/Matrix.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "tulip/Camera.h"
#include "tulip/GlEntity.h"
#include "tulip/GlTools.h"
#include "tulip/GlScene.h"

#include <iostream>

using namespace std;

namespace tlp {

  GlCPULODCalculator::~GlCPULODCalculator() {
  }

  void GlCPULODCalculator::beginNewCamera(Camera* camera) {
    // add a new layerLODUnit add set camera pointer
    layersLODVector.push_back(LayerLODUnit());
    currentLayerLODUnit=&layersLODVector.back();
    currentLayerLODUnit->camera=(unsigned long)camera;
  }

  void GlCPULODCalculator::addSimpleEntityBoundingBox(unsigned long entity,const BoundingBox& bb) {
    assert(bb.isValid());
    // check if we have to render simple entities
    if((renderingEntitiesFlag & RenderingSimpleEntities) !=0)
      currentLayerLODUnit->simpleEntitiesLODVector.push_back(SimpleEntityLODUnit(entity,bb));
  }
  void GlCPULODCalculator::addNodeBoundingBox(unsigned int id,const BoundingBox& bb) {
    assert(bb.isValid());
    // check if we have to render nodes
    if((renderingEntitiesFlag & RenderingNodes) !=0)
      currentLayerLODUnit->nodesLODVector.push_back(ComplexEntityLODUnit(id,bb));
  }
  void GlCPULODCalculator::addEdgeBoundingBox(unsigned int id,const BoundingBox& bb) {
    assert(bb.isValid());
    // check if we have to render edges
    if((renderingEntitiesFlag & RenderingEdges) !=0)
      currentLayerLODUnit->edgesLODVector.push_back(ComplexEntityLODUnit(id,bb));
  }

  void GlCPULODCalculator::reserveMemoryForNodes(unsigned int numberOfNodes) {
    if((renderingEntitiesFlag & RenderingNodes) !=0)
      currentLayerLODUnit->nodesLODVector.reserve(numberOfNodes);
  }

  void GlCPULODCalculator::reserveMemoryForEdges(unsigned int numberOfEdges) {
    if((renderingEntitiesFlag & RenderingEdges) !=0)
      currentLayerLODUnit->edgesLODVector.reserve(numberOfEdges);
  }

  void GlCPULODCalculator::compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport) {

    for(vector<LayerLODUnit>::iterator it=layersLODVector.begin();it!=layersLODVector.end();++it){
      Camera *camera=(Camera*)((*it).camera);

      Matrix<float,4> transformMatrix;
      camera->getTransformMatrix(globalViewport,transformMatrix);

      Coord eye;
      if(camera->is3D()) {
        eye=camera->getEyes() + ( camera->getEyes() - camera->getCenter() ) / (float)camera->getZoomFactor();
        computeFor3DCamera(&(*it),eye,transformMatrix,globalViewport,currentViewport);
      }else{
        computeFor2DCamera(&(*it),globalViewport,currentViewport);
      }

      glMatrixMode(GL_MODELVIEW);
    }
  }

  void GlCPULODCalculator::computeFor3DCamera(LayerLODUnit *layerLODUnit,
                                              const Coord &eye,
                                              const Matrix<float, 4> transformMatrix,
                                              const Vector<int,4>& globalViewport,
                                              const Vector<int,4>& currentViewport) {
    size_t nb=layerLODUnit->simpleEntitiesLODVector.size();

#ifdef _OPENMP
    omp_set_num_threads(omp_get_num_procs());
    omp_set_nested(true);
    omp_set_dynamic(false);
#endif

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(size_t i=0;i<nb;++i){
      layerLODUnit->simpleEntitiesLODVector[i].lod=calculateAABBSize(layerLODUnit->simpleEntitiesLODVector[i].boundingBox,eye,transformMatrix,globalViewport,currentViewport);
    }
    nb=layerLODUnit->nodesLODVector.size();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(size_t i=0;i<nb;++i){
      layerLODUnit->nodesLODVector[i].lod=calculateAABBSize(layerLODUnit->nodesLODVector[i].boundingBox,eye,transformMatrix,globalViewport,currentViewport);
    }
    nb=layerLODUnit->edgesLODVector.size();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(size_t i=0;i<nb;++i){
      layerLODUnit->edgesLODVector[i].lod=calculateAABBSize(layerLODUnit->edgesLODVector[i].boundingBox,eye,transformMatrix,globalViewport,currentViewport);
    }
  }

  void GlCPULODCalculator::computeFor2DCamera(LayerLODUnit *layerLODUnit,
      const Vector<int,4>& globalViewport,
      const Vector<int,4>& currentViewport) {

    for(vector<SimpleEntityLODUnit>::iterator it=layerLODUnit->simpleEntitiesLODVector.begin();it!=layerLODUnit->simpleEntitiesLODVector.end();++it){
      (*it).lod=calculate2DLod((*it).boundingBox,globalViewport,currentViewport);
    }
    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->nodesLODVector.begin();it!=layerLODUnit->nodesLODVector.end();++it){
      (*it).lod=calculate2DLod((*it).boundingBox,globalViewport,currentViewport);
    }
    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->edgesLODVector.begin();it!=layerLODUnit->edgesLODVector.end();++it){
      (*it).lod=calculate2DLod((*it).boundingBox,globalViewport,currentViewport);
    }
  }

}

