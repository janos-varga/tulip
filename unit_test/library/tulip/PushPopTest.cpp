/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/ForEach.h>
#include "PushPopTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ConnectedTest.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( PushPopTest );

//==========================================================
void PushPopTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void PushPopTest::tearDown() {
  //graph->pop();
  delete graph;
}
//==========================================================
void PushPopTest::build(unsigned int nbNodes, unsigned int edgeRatio) {
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD  = nbNodes;
  unsigned int EDGE_RATIO = edgeRatio;

  for (unsigned int i=0; i<NB_ADD; ++i)
    nodes.push_back(graph->addNode());

  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;

  for (unsigned int i=0; i< NB_EDGES; ++i)
    edges.push_back(graph->addEdge(nodes[rand()%NB_ADD], nodes[rand()%NB_ADD]));
}
//==========================================================
void PushPopTest::testAddDel() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  graph->push();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  edge e1 = graph->addEdge(n2, n3);
  CPPUNIT_ASSERT(graph->existEdge(n2, n3) == e1);

  graph->reverse(e0);
  CPPUNIT_ASSERT(graph->existEdge(n1, n0) == e0);

  graph->delNode(n0);
  CPPUNIT_ASSERT(graph->isElement(n0) == false);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);

  vector<node> nodes;
  vector<pair <node, node> > ends;
  vector<edge> edges;

  graph->addNodes(3, nodes);
  CPPUNIT_ASSERT(nodes.size() == 3);
  ends.push_back(pair<node, node>(nodes[0], nodes[1]));
  ends.push_back(pair<node, node>(nodes[0], nodes[2]));
  ends.push_back(pair<node, node>(nodes[1], nodes[2]));
  graph->addEdges(ends, edges);
  CPPUNIT_ASSERT(edges.size() == 3);

  graph->pop();

  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);
  CPPUNIT_ASSERT(graph->isElement(nodes[0]) == false);
  CPPUNIT_ASSERT(graph->isElement(nodes[1]) == false);
  CPPUNIT_ASSERT(graph->isElement(nodes[2]) == false);

  graph->unpop();
  CPPUNIT_ASSERT(graph->existEdge(n2, n3) == e1);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);
  CPPUNIT_ASSERT(graph->isElement(nodes[0]));
  CPPUNIT_ASSERT(graph->isElement(nodes[1]));
  CPPUNIT_ASSERT(graph->isElement(nodes[2]));
  CPPUNIT_ASSERT(graph->existEdge(nodes[0], nodes[1]) == edges[0]);
  CPPUNIT_ASSERT(graph->existEdge(nodes[0], nodes[2]) == edges[1]);
  CPPUNIT_ASSERT(graph->existEdge(nodes[1], nodes[2]) == edges[2]);

  graph->reverse(e1);
  CPPUNIT_ASSERT(graph->existEdge(n3, n2) == e1);

  StlIterator<edge, std::vector<edge>::const_iterator> ite(edges.begin(),
      edges.end());
  graph->delEdges(&ite);
  CPPUNIT_ASSERT(graph->isElement(edges[0]) == false);
  CPPUNIT_ASSERT(graph->isElement(edges[1]) == false);
  CPPUNIT_ASSERT(graph->isElement(edges[2]) == false);

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == false);
  CPPUNIT_ASSERT(graph->isElement(nodes[0]) == false);
  CPPUNIT_ASSERT(graph->isElement(nodes[1]) == false);
  CPPUNIT_ASSERT(graph->isElement(nodes[2]) == false);

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e1) == true);
  CPPUNIT_ASSERT(graph->isElement(n2) == true);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);
  CPPUNIT_ASSERT(graph->existEdge(n3, n2) == e1);
  CPPUNIT_ASSERT(graph->isElement(nodes[0]));
  CPPUNIT_ASSERT(graph->isElement(nodes[1]));
  CPPUNIT_ASSERT(graph->isElement(nodes[2]));
  CPPUNIT_ASSERT(graph->isElement(edges[0]) == false);
  CPPUNIT_ASSERT(graph->isElement(edges[1]) == false);
  CPPUNIT_ASSERT(graph->isElement(edges[2]) == false);

  graph->delEdge(e1);
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == true);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);

  graph->delNode(n2);
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == false);

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);

}

//==========================================================
void PushPopTest::testSetEnds() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  graph->push();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  graph->setEnds(e0, n2, n3);
  CPPUNIT_ASSERT(graph->existEdge(n2, n3) == e0);

  graph->reverse(e0);
  CPPUNIT_ASSERT(graph->existEdge(n3, n2) == e0);

  graph->pop();

  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);

  graph->unpop();

  CPPUNIT_ASSERT(graph->existEdge(n3, n2) == e0);

  graph->delEdge(e0);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);
  CPPUNIT_ASSERT(graph->existEdge(n3, n2).isValid() == false);

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);

  Graph* g1 = graph->addSubGraph();
  g1->addNode(n0);
  g1->addNode(n1);
  g1->addEdge(e0);

  graph->push();

  g1->reverse(e0);
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n1, n0) == e0);

  n2 = g1->addNode();
  g1->setTarget(e0, n2);
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n1, n2) == e0);

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n1, n2) == e0);

  g1->setSource(e0, n0);
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n0, n2) == e0);

  g1->delEdge(e0);

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n0, n2) == e0);

  g1->addEdge(e0);
  CPPUNIT_ASSERT(g1->isElement(e0));
  CPPUNIT_ASSERT(g1->existEdge(n0, n2) == e0);

  g1->setEnds(e0, n2, n1);
  CPPUNIT_ASSERT(g1->existEdge(n2, n1) == e0);
  CPPUNIT_ASSERT(graph->existEdge(n2, n1) == e0);

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);
}

//==========================================================
void PushPopTest::testSetValue() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  DoubleProperty* prop = graph->getProperty<DoubleProperty>("prop");

  prop->setNodeValue(n1, 1.0);
  prop->setEdgeValue(e0, 1.0);

  graph->push();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n0, n2);

  prop->setNodeValue(n2, 2.0);
  prop->setEdgeValue(e1, 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 2.0);

  prop->setNodeValue(n1, 2.0);
  prop->setEdgeValue(e0, 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 2.0);

  graph->push();
  prop->setAllNodeValue(3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 3.0);
  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 2.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 3.0);

  prop->setAllEdgeValue(3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 3.0);

  graph->delNode(n1);
  CPPUNIT_ASSERT(graph->isElement(n1) == false);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 2.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 3.0);

  prop->setNodeValue(n0, 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 2.0);
  prop->setEdgeValue(e0, 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 2.0);
}

//==========================================================
void PushPopTest::testSetEltValue() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  DoubleVectorProperty* prop =
    graph->getProperty<DoubleVectorProperty>("prop");

  vector<double> vv;
  vv.push_back(1.0);
  vv.push_back(1.0);

  prop->setNodeValue(n1, vv);
  prop->setEdgeValue(e0, vv);

  graph->push();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n0, n2);

  CPPUNIT_ASSERT(prop->getEdgeValue(e1).size() == 0);

  prop->setNodeValue(n2, vv);
  prop->setEdgeValue(e1, vv);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 0) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 1) == 1.0);

  prop->setNodeEltValue(n1, 1, 2.0);
  prop->setEdgeEltValue(e0, 1, 2.0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 1) == 2.0);

  graph->push();
  vector<double> dv;
  dv.push_back(3.0);
  prop->setAllNodeValue(dv);
  prop->pushBackNodeEltValue(n1, 3.0);
  prop->pushBackNodeEltValue(n2, 3.0);
  prop->pushBackNodeEltValue(n2, 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 1);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n0, 0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1).size() == 2);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2).size() == 3);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 2) == 3.0);

  prop->setNodeEltValue(n0, 0, 4.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 1);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n0, 0) == 4.0);

  prop->popBackNodeEltValue(n0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 0);
  prop->pushBackNodeEltValue(n0, 3.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 1) == 1.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 1);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n0, 0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1).size() == 2);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2).size() == 3);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 2) == 3.0);

  prop->setAllEdgeValue(dv);
  prop->pushBackEdgeEltValue(e1, 3.0);
  prop->pushBackEdgeEltValue(e1, 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0).size() == 1);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 0) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1).size() == 3);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 1) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 2) == 3.0);

  prop->setEdgeEltValue(e0, 0, 4.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0).size() == 1);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 0) == 4.0);

  graph->delNode(n1);
  CPPUNIT_ASSERT(graph->isElement(n1) == false);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 1) == 1.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeEltValue(n0, 0) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 0) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 1) == 3.0);

  prop->pushBackNodeEltValue(n0, 3.0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n0, 1) == 3.0);
  prop->pushBackEdgeEltValue(e0, 3.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 1) == 3.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 1) == 2.0);
}

//==========================================================
void PushPopTest::testVectorValue() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  DoubleVectorProperty* prop =
    graph->getProperty<DoubleVectorProperty>("prop");

  vector<double> vv;
  vv.push_back(1.0);
  vv.push_back(1.0);

  prop->setNodeValue(n1, vv);
  prop->setEdgeValue(e0, vv);

  graph->push();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n0, n2);

  prop->setNodeValue(n2, vv);
  prop->setEdgeValue(e1, vv);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).size() == 0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 0) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 1) == 1.0);

  prop->setNodeEltValue(n1, 1, 2.0);
  prop->setEdgeEltValue(e0, 1, 2.0);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e0, 1) == 2.0);

  graph->push();
  prop->resizeNodeValue(n1, 2);
  prop->resizeNodeValue(n2, 3, 3.0);
  prop->resizeEdgeValue(e1, 3, 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1).size() == 2);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2).size() == 3);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 2) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1).size() == 3);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 2) == 3.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n1).size() == 2);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2).size() == 2);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1).size() == 2);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 1) == 1.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n1).size() == 2);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2).size() == 3);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 2) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1).size() == 3);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 2) == 3.0);

  prop->popBackNodeEltValue(n1);
  prop->popBackNodeEltValue(n2);
  prop->popBackEdgeEltValue(e1);
  CPPUNIT_ASSERT(prop->getNodeValue(n1).size() == 1);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n1, 0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2).size() == 2);
  CPPUNIT_ASSERT(prop->getNodeEltValue(n2, 1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1).size() == 2);
  CPPUNIT_ASSERT(prop->getEdgeEltValue(e1, 1) == 1.0);
}

//==========================================================
void PushPopTest::testCopyProperty() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);
  edge e1 = graph->addEdge(n1, n0);

  DoubleProperty* prop = graph->getProperty<DoubleProperty>("prop");

  prop->setNodeValue(n0, 1.0);
  prop->setEdgeValue(e0, 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 0.0);

  DoubleProperty p(graph);

  p.setNodeValue(n1, 1.0);
  p.setEdgeValue(e1, 1.0);
  CPPUNIT_ASSERT(p.getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(p.getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(p.getEdgeValue(e0) == 0.0);
  CPPUNIT_ASSERT(p.getEdgeValue(e1) == 1.0);

  graph->push();

  *prop = p;
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 1.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 0.0);

  graph->push();
  prop->setAllNodeValue(0.0);
  prop->setAllEdgeValue(0.0);
  prop->setNodeValue(n1, 1.0);
  prop->setEdgeValue(e1, 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 1.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 0.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 1.0);
}

void PushPopTest::testSubgraph() {
  Graph *g1, *g2, *g3, *g4;

  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g2->addNode(n0);
  g2->addNode(n1);
  g2->addEdge(e0);

  graph->push();

  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  Iterator<Graph *> *it= g2->getSubGraphs();
  Graph *a,*b;
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->unpop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  node n2 = g3->addNode();
  node n3 = g3->addNode();
  edge e = g2->addEdge(n2,n3);

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g3->isElement(n3));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  graph->pop();

  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(!g2->isElement(n2));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(!graph->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(n3));

  graph->unpop();

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g3->isElement(n3));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  g2->delNode(n3);

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(!g3->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));

  graph->delNode(n0);

  CPPUNIT_ASSERT(!g2->isElement(n0));
  CPPUNIT_ASSERT(!g2->isElement(e0));
  CPPUNIT_ASSERT(!graph->isElement(n0));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  graph->pop();

  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(!graph->isElement(n3));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n0));
  CPPUNIT_ASSERT(g2->isElement(e0));
  CPPUNIT_ASSERT(graph->isElement(n0));
  CPPUNIT_ASSERT(graph->isElement(e0));

  graph->unpop();

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(!g3->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(!g2->isElement(n0));
  CPPUNIT_ASSERT(!g2->isElement(e0));
  CPPUNIT_ASSERT(!graph->isElement(n0));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  g3->addNode(n1);
  e = g3->addEdge(n1, n2);
  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n1));
  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n1));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n1));
  CPPUNIT_ASSERT(graph->isElement(n2));

  graph->pop();
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n1));
  CPPUNIT_ASSERT(!g2->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n1));
  CPPUNIT_ASSERT(!graph->isElement(n2));

  graph->unpop();
  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n1));
  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n1));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n1));
  CPPUNIT_ASSERT(graph->isElement(n2));

  graph->push();

  g2->delSubGraph(g3);
  g2->delSubGraph(g4);

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n1));
  CPPUNIT_ASSERT(g3->isElement(n2));

  g2->delSubGraph(g4);
  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(!it->hasNext());
  CPPUNIT_ASSERT(a == g3);
  delete it;

  graph->push();
  graph->delSubGraph(g2);
  it= graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g1 && b==g3) || (a==g3 && b==g1));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();
  it= graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g1 && b==g2) || (a==g2 && b==g1));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
}

void PushPopTest::testTests() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);
  edge e1 = graph->addEdge(n0, n2);

  node n3 = graph->addNode();

  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));

  graph->push();
  vector<edge> addedEdges;
  ConnectedTest::makeConnected(graph, addedEdges);
  CPPUNIT_ASSERT(ConnectedTest::isConnected(graph));

  graph->pop();
  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));
}

void PushPopTest::testAddDelProps() {
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));

  BooleanProperty* bProp = graph->getProperty<BooleanProperty>("boolean");
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  graph->push();

  DoubleProperty* dProp = graph->getProperty<DoubleProperty>("double");
  CPPUNIT_ASSERT(graph->existProperty("double"));
  node n = graph->addNode();
  dProp->setNodeValue(n, 123456789.);

  graph->pop();

  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(!graph->isElement(n));

  graph->unpop();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double") == dProp);
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->isElement(n));
  CPPUNIT_ASSERT(dProp->getNodeValue(n) == 123456789.);

  graph->delLocalProperty("boolean");
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  graph->getProperty<DoubleProperty>("double");
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->isElement(n));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double")->getNodeValue(n) != 123456789.);

  graph->push();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  graph->pop();
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->isElement(n));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double")->getNodeValue(n) != 123456789.);

  graph->pop();
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->getProperty<BooleanProperty>("boolean") == bProp);
}

// this class will capture
// everything that will happen to our properties
class PropertyObserverForTest :public PropertyObserver, public Observable {
public:
  std::set<PropertyInterface*> properties;

  PropertyObserverForTest() {}

  void reset() {
    properties.clear();
  }

  unsigned int nbProperties() {
    return properties.size();
  }

  bool found(PropertyInterface *prop) {
    return properties.find(prop) != properties.end();
  }

  virtual void destroy(PropertyInterface* prop) {
    properties.insert(prop);
  }

  virtual void treatEvent(const Event& evt) {
    PropertyInterface* prop = dynamic_cast<PropertyInterface *>(evt.sender());

    if (prop && evt.type() == Event::TLP_DELETE)
      destroy(prop);
  }

};

void PushPopTest::testObserveDelProps() {
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));

  BooleanProperty* bProp = graph->getProperty<BooleanProperty>("boolean");
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  PropertyObserverForTest pObserver;
  pObserver.reset();
  bProp->addPropertyObserver(&pObserver);
  CPPUNIT_ASSERT(pObserver.nbProperties() == 0);

  graph->push();

  graph->delLocalProperty("boolean");
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(pObserver.nbProperties() == 1);
  CPPUNIT_ASSERT(pObserver.found(bProp));

  DoubleProperty* dProp = graph->getProperty<DoubleProperty>("double");
  dProp->addPropertyObserver(&pObserver);
  pObserver.reset();
  CPPUNIT_ASSERT(pObserver.nbProperties() == 0);
  graph->pop();
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(pObserver.nbProperties() == 1);
  CPPUNIT_ASSERT(pObserver.found(dProp));
  pObserver.reset();

  graph->unpop();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double") == dProp);
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(pObserver.nbProperties() == 1);
  CPPUNIT_ASSERT(pObserver.found(bProp));

  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(pObserver.nbProperties() == 2);
  CPPUNIT_ASSERT(pObserver.found(dProp));

  graph->pop();
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->getProperty<BooleanProperty>("boolean") == bProp);
  CPPUNIT_ASSERT(!graph->existProperty("double"));
}


void PushPopTest::testAddSubgraphProp() {
  Graph *sg;

  Iterator<Graph *> *it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->push();
  sg = graph->addSubGraph();

  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT(sg == it->next());
  delete it;

  sg->push();
  DoubleProperty* prop = sg->getLocalProperty<DoubleProperty>("test");
  CPPUNIT_ASSERT(sg->existProperty("test"));

  sg->pop();
  CPPUNIT_ASSERT(!sg->existProperty("test"));

  sg->unpop();
  CPPUNIT_ASSERT(sg->existProperty("test"));
  CPPUNIT_ASSERT(sg->getLocalProperty<DoubleProperty>("test") == prop);

  sg->pop();
  CPPUNIT_ASSERT(!sg->existProperty("test"));
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT(sg == it->next());
  delete it;

  graph->pop();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->unpop();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT(sg == it->next());
  delete it;

  sg->unpop();
  CPPUNIT_ASSERT(sg->existProperty("test"));
  CPPUNIT_ASSERT(sg->getLocalProperty<DoubleProperty>("test") == prop);

  sg->pop();
  CPPUNIT_ASSERT(!sg->existProperty("test"));
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT(sg == it->next());
  delete it;

  graph->pop();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
  CPPUNIT_ASSERT(!graph->canPop());
  CPPUNIT_ASSERT(graph->canUnpop());

  graph->push();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
  CPPUNIT_ASSERT(graph->canPop());
  CPPUNIT_ASSERT(!graph->canUnpop());
}

void PushPopTest::testMetaNode() {
  node n0 = graph->addNode();
  LayoutProperty* layout = graph->getProperty<LayoutProperty>("viewLayout");
  Coord coord0(-1, -1, -1);
  layout->setNodeValue(n0, coord0);

  set<node> tmp;
  tmp.insert(n0);

  graph->push();

  // create clone graph
  Graph* clone = tlp::newCloneSubGraph(graph, "groups");

  // create meta node
  node metaNode = clone->createMetaNode(tmp);
  Color mColor(255, 255, 255, 127);
  ColorProperty* color = graph->getProperty<ColorProperty>("viewColor");
  color->setNodeValue(metaNode, mColor);

  CPPUNIT_ASSERT(graph->isElement(metaNode));
  CPPUNIT_ASSERT(graph->isMetaNode(metaNode));
  CPPUNIT_ASSERT(coord0 == layout->getNodeValue(metaNode));
  CPPUNIT_ASSERT(mColor == color->getNodeValue(metaNode));

  graph->pop();
  CPPUNIT_ASSERT(!graph->isElement(metaNode));
  CPPUNIT_ASSERT(coord0 != layout->getNodeValue(metaNode));

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(metaNode));
  CPPUNIT_ASSERT(graph->isMetaNode(metaNode));
  CPPUNIT_ASSERT(coord0 == layout->getNodeValue(metaNode));
  CPPUNIT_ASSERT(mColor == color->getNodeValue(metaNode));
}

//==========================================================
CppUnit::Test * PushPopTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Push/Pop" );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Add/Del operations (Simple Graph)",
                         &PushPopTest::testAddDel) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "SetNode/EdgeValue operations",
                         &PushPopTest::testSetValue) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "SetNode/EdgeEltValue operations",
                         &PushPopTest::testSetEltValue) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Vector Value operations",
                         &PushPopTest::testVectorValue) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "SetEnds operations",
                         &PushPopTest::testSetEnds) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Copy Prop operations",
                         &PushPopTest::testCopyProperty) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "addSubGraph operations",
                         &PushPopTest::testSubgraph) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "addSubGraphProp operations",
                         &PushPopTest::testAddSubgraphProp) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Tests operations",
                         &PushPopTest::testTests) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Properties deletion operations",
                         &PushPopTest::testObserveDelProps) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Properties operations",
                         &PushPopTest::testAddDelProps) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "MetaNode operations",
                         &PushPopTest::testMetaNode) );
  return suiteOfTests;
}
//==========================================================
