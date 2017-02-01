/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/GraphStorage.h>
#include <tulip/memorypool.h>

using namespace tlp;

//=======================================================
void GraphStorage::clear() {
  nbNodes = 0;
  nbEdges = 0;
}
//=======================================================
GraphStorage::GraphStorage() {
  clear();
}
//=======================================================
GraphStorage::~GraphStorage() {
  for (Nodes::iterator i = nodes.begin(); i != nodes.end(); ++i) {
    i->edges.deallocateAll();
  }
}
//=======================================================
/**
 * @brief Return true if n belongs to the graph
 */
bool GraphStorage::isElement(const node n) const {
  return !nodeIds.is_free(n.id);
}
//=======================================================
/**
 * @brief Return true if e belongs to the graph
 */
bool GraphStorage::isElement(const edge e) const {
  return !edgeIds.is_free(e.id);
}
//=======================================================
/**
 * @brief Enables to reserve memory for nbNodes
 */
void GraphStorage::reserveNodes(const size_t nb) {
  if (nb > nbNodes)
    nodes.reserve(nb);
}
//=======================================================
/**
 * @brief Enables to reserve memory for nbEdges
 */
void GraphStorage::reserveEdges(const size_t nb) {
  if (nb > nbEdges)
    edges.reserve(nb);
}
//=======================================================
/**
 * @brief Enables to reserve memory for adjacency nodes
 */
void GraphStorage::reserveAdj(const node n, const size_t nb) {
  EdgeVector &nEdges = nodes[n.id].edges;

  if (nEdges.size() < nb)
    nEdges.resize(nb);
}
//=======================================================
/**
 * @brief Enables to reserve memory for adjacency nodes
 */
void GraphStorage::reserveAdj(const size_t nb) {
  for (unsigned int i = 0; i < nodes.size(); ++i) {
    reserveAdj(node(i), nb);
  }
}
//=======================================================
/**
 * @brief restore adjacency edges of a given node
 */
void GraphStorage::restoreAdj(const node n, const std::vector<edge> &edges) {
  EdgeVector &nEdges = nodes[n.id].edges;
  nEdges.clear();

  for (unsigned int i = 0; i < edges.size(); ++i)
    nEdges.push_back(edges[i]);
}
//=======================================================
/**
 * @brief Return the first node of graph
 */
node GraphStorage::getOneNode() const {
  for (unsigned int i = 0; i < nodes.size(); ++i) {
    if (isElement(node(i)))
      return node(i);
  }

  return node();
}
//=======================================================
/**
 * @brief Return a Tulip Iterator on nodes of the graph
 */
Iterator<node> *GraphStorage::getNodes() const {
  return nodeIds.getIds<node>();
}
//=======================================================
// define a simple class to encapsulate the memento of ids
struct IdsMemento : public GraphStorageIdsMemento {
  IdManagerState nodeIds;
  IdManagerState edgeIds;
  ~IdsMemento() {
  }
};
//=======================================================
/**
 * @brief Return the current state of the ids management
 * must be deleted by the caller
 */
const GraphStorageIdsMemento *GraphStorage::getIdsMemento() {
  IdsMemento *memento = new IdsMemento();
  memento->nodeIds = nodeIds.getState();
  memento->edgeIds = edgeIds.getState();

  return memento;
}
//=======================================================
/**
 * @brief restore a state of the ids management
 */
void GraphStorage::restoreIdsMemento(const GraphStorageIdsMemento *memento) {
  nodeIds.restoreState(static_cast<const IdsMemento *>(memento)->nodeIds);
  edgeIds.restoreState(static_cast<const IdsMemento *>(memento)->edgeIds);
}
//=======================================================
/**
 * @brief Return a Tulip Iterator on edges of the graph
 */
Iterator<edge> *GraphStorage::getEdges() const {
  return edgeIds.getIds<edge>();
}
//=======================================================
// specific iterator classes used to implement
// the get*Nodes & get*Edges methods

// define a class to iterate on graph storage edges
struct EdgeContainerIterator : public Iterator<edge>, public MemoryPool<EdgeContainerIterator> {
  SimpleVector<edge>::iterator it, itEnd;
  EdgeContainerIterator(SimpleVector<edge> &v) : it(v.begin()), itEnd(v.end()) {
  }
  ~EdgeContainerIterator() {
  }
  bool hasNext() {
    return (it != itEnd);
  }
  edge next() {
    assert(hasNext());
    edge tmp = (*it);
    ++it;
    return tmp;
  }
};

// define some values for further template specializations
// IO_IN => in nodes/edges
// IO_OUT => out nodes/edges
// IO_INOUT => inout nodes/edges
// IO_ prefix is needed on windows platform
// to avoid compilation failure
enum IO_TYPE { IO_IN = 0, IO_OUT = 1, IO_INOUT = 2 };

// define a template class to iterate on in or out edges of a given node
template <IO_TYPE io_type> struct IOEdgeContainerIterator : public Iterator<edge>, public MemoryPool<IOEdgeContainerIterator<io_type>> {
  node n;
  edge curEdge;
  std::set<edge> loop;
  const std::vector<std::pair<node, node>> &edges;
  SimpleVector<edge>::iterator it, itEnd;

  void prepareNext() {
    for (; it != itEnd; ++it) {
      curEdge = *it;
      // note that io_type value may only be IO_IN which is null
      // or IO_OUT which is define to 1
      node curNode = io_type ? edges[curEdge.id].first : edges[curEdge.id].second;

      if (curNode != n)
        continue;

      curNode = io_type ? edges[curEdge.id].second : edges[curEdge.id].first;

      if (curNode == n) {
        if (loop.find(curEdge) == loop.end()) {
          loop.insert(curEdge);
          ++it;
          return;
        }
      } else {
        ++it;
        return;
      }
    }

    // set curEdge as invalid
    curEdge = edge();
  }

  IOEdgeContainerIterator(node n, SimpleVector<edge> &v, std::vector<std::pair<node, node>> &edges)
      : n(n), edges(edges), it(v.begin()), itEnd(v.end()) {
    prepareNext();
  }

  ~IOEdgeContainerIterator() {
  }

  edge next() {
    // check hasNext()
    assert(curEdge.isValid());
    // we are already pointing to the next
    edge tmp = curEdge;
    // anticipating the next iteration
    prepareNext();
    return tmp;
  }

  bool hasNext() {
    return (curEdge.isValid());
  }
};

// define a class to iterate on in or out nodes of a given node
template <IO_TYPE io_type> struct IONodesIterator : public Iterator<node>, public MemoryPool<IONodesIterator<io_type>> {
  node n;
  const std::vector<std::pair<node, node>> &edges;
  Iterator<edge> *it;

  IONodesIterator(node n, SimpleVector<edge> &nEdges, std::vector<std::pair<node, node>> &edges) : n(n), edges(edges) {
    if (io_type == IO_INOUT)
      it = new EdgeContainerIterator(nEdges);
    else
      it = new IOEdgeContainerIterator<io_type>(n, nEdges, edges);
  }

  ~IONodesIterator() {
    delete it;
  }

  bool hasNext() {
    return (it->hasNext());
  }

  node next() {
    // check hasNext()
    assert(it->hasNext());
    const std::pair<node, node> &ends = edges[it->next()];

    // out nodes
    if (io_type == IO_OUT)
      return ends.second;

    // in nodes
    if (io_type == IO_IN)
      return ends.first;

    // inout nodes
    return (ends.first == n) ? ends.second : ends.first;
  }
};
//=======================================================
Iterator<edge> *GraphStorage::getInOutEdges(const node n) const {
  return new EdgeContainerIterator(nodes[n.id].edges);
}
//=======================================================
void GraphStorage::getInOutEdges(const node n, std::vector<edge> &edges) const {
  SimpleVector<edge> &nEdges = nodes[n.id].edges;
  unsigned int nbEdges = nodes[n.id].edges.size();
  edges.resize(nbEdges);
  if (nbEdges)
    memcpy(edges.data(), &nEdges[0], nEdges.size() * sizeof(edge));
}
//=======================================================
bool GraphStorage::getEdges(const node src, const node tgt, bool directed, std::vector<edge> &vEdges, bool onlyFirst) const {
  SimpleVector<edge>::const_iterator it = nodes[src.id].edges.begin();
  edge previous;
  bool result = false;

  while (it != nodes[src.id].edges.end()) {
    edge e = (*it);

    // loops appear twice
    if (e != previous) {
      const std::pair<node, node> &eEnds = edges[e.id];

      if ((eEnds.second == tgt && eEnds.first == src) || (!directed && eEnds.first == tgt && eEnds.second == src)) {
        vEdges.push_back(e);

        if (onlyFirst)
          return true;

        result = true;
      }
    }

    previous = e;
    ++it;
  }

  return result;
}
//=======================================================
Iterator<edge> *GraphStorage::getOutEdges(const node n) const {
  return new IOEdgeContainerIterator<IO_OUT>(n, nodes[n.id].edges, edges);
}
//=======================================================
Iterator<edge> *GraphStorage::getInEdges(const node n) const {
  return new IOEdgeContainerIterator<IO_IN>(n, nodes[n.id].edges, edges);
}
//=======================================================
Iterator<node> *GraphStorage::getInOutNodes(const node n) const {
  return new IONodesIterator<IO_INOUT>(n, nodes[n.id].edges, edges);
}
//=======================================================
Iterator<node> *GraphStorage::getInNodes(const node n) const {
  return new IONodesIterator<IO_IN>(n, nodes[n.id].edges, edges);
}
//=======================================================
Iterator<node> *GraphStorage::getOutNodes(const node n) const {
  return new IONodesIterator<IO_OUT>(n, nodes[n.id].edges, edges);
}
//=======================================================
/**
 * @brief Return the degree of a node
 */
unsigned int GraphStorage::deg(const node n) const {
  assert(isElement(n));
  return nodes[n.id].edges.size();
}

//=======================================================
/**
 * @brief Return the out degree of a node
 */
unsigned int GraphStorage::outdeg(const node n) const {
  assert(isElement(n));
  return nodes[n.id].outDegree;
}

//=======================================================
/**
 * @brief Return the in degree of a node
 */
unsigned int GraphStorage::indeg(const node n) const {
  assert(isElement(n));
  const EdgeContainer &ctnr = nodes[n.id];
  return ctnr.edges.size() - ctnr.outDegree;
}
//=======================================================
/**
 * @brief Return the number of edges in the graph
 */
unsigned int GraphStorage::numberOfEdges() const {
  return nbEdges;
}
//=======================================================
/**
 * @brief Return the number of nodes in the graph
 */
unsigned int GraphStorage::numberOfNodes() const {
  return nbNodes;
}
//=======================================================
/**
 * @brief Return the extremities of an edge (src, target)
 */
const std::pair<node, node> &GraphStorage::ends(const edge e) const {
  assert(isElement(e));
  return edges[e.id];
}
//=======================================================
/**
 * @brief return the first extremity (considered as source if the graph is directed) of an edge
 */
node GraphStorage::source(const edge e) const {
  return edges[e.id].first;
}
//=======================================================
/**
 * @brief return the second extremity (considered as target if the graph is directed) of an edge
 * @complexity o(1)
 */
node GraphStorage::target(const edge e) const {
  return edges[e.id].second;
}
//=======================================================
/**
 * @brief return the opposite node of n through edge e
 */
node GraphStorage::opposite(const edge e, const node n) const {
  const std::pair<node, node> &eEnds = edges[e.id];
  assert((eEnds.first == n) || (eEnds.second == n));
  return (eEnds.first == n) ? eEnds.second : eEnds.first;
}

//=======================================================
/**
 * @brief Reconnect the edge e to have the new given ends
 */
void GraphStorage::setEnds(const edge e, const node newSrc, const node newTgt) {
  assert(isElement(e));
  std::pair<node, node> &eEnds = edges[e.id];
  node src = eEnds.first;
  node tgt = eEnds.second;

  // nothing to do if same ends
  if (src == newSrc && tgt == newTgt)
    return;

  node nSrc = newSrc;
  node nTgt = newTgt;

  if (newSrc.isValid() && src != newSrc) {
    assert(isElement(newSrc));
    eEnds.first = newSrc;
    EdgeContainer &sCtnr = nodes[src.id];
    EdgeContainer &nCtnr = nodes[newSrc.id];
    sCtnr.outDegree -= 1;
    nCtnr.outDegree += 1;
    nCtnr.edges.push_back(e);
    removeFromEdgeContainer(sCtnr, e);
  } else
    nSrc = src;

  if (newTgt.isValid() && tgt != newTgt) {
    assert(isElement(newTgt));
    eEnds.second = newTgt;
    nodes[newTgt.id].edges.push_back(e);
    removeFromEdgeContainer(nodes[tgt.id], e);
  } else
    nTgt = tgt;
}
//=======================================================
/**
 * @brief change the source of an edge
 */
void GraphStorage::setSource(const edge e, const node n) {
  setEnds(e, n, node());
}
//=======================================================
/**
 * @brief change the target of an edge
 */
void GraphStorage::setTarget(const edge e, const node n) {
  setEnds(e, node(), n);
}
//=======================================================
/**
 * @brief Reverse an edge e, source become target and target become soure
 */
void GraphStorage::reverse(const edge e) {
  assert(isElement(e));
  std::pair<node, node> &eEnds = edges[e.id];
  node src = eEnds.first;
  node tgt = eEnds.second;
  eEnds.first = tgt;
  eEnds.second = src;
  nodes[src.id].outDegree -= 1;
  nodes[tgt.id].outDegree += 1;
}
//=======================================================
/**
 *  \brief Set the ordering of edges around n according to their order in v.
 */
void GraphStorage::setEdgeOrder(const node n, const std::vector<edge> &v) {
  if (v.empty())
    return;

  MutableContainer<int> isEle;
  isEle.setAll(0);

  for (std::vector<edge>::const_iterator it = v.begin(); it != v.end(); ++it) {
    isEle.add(it->id, 1);
  }

  std::vector<edge>::const_iterator it2 = v.begin();
  EdgeVector &currentOrder = nodes[n.id].edges;

  for (unsigned int i = 0; i < currentOrder.size(); ++i) {
    if (isEle.get(currentOrder[i].id) > 0) {
      isEle.add(currentOrder[i].id, -1);
      currentOrder[i] = *it2;
      ++it2;
    }
  }
}
//=======================================================
/**
 * \brief swap to edge in the ordered adjacency vector
 * \warning the two edges must be element of star(v)
 * @complexity o(1)
 */
void GraphStorage::swapEdgeOrder(const node n, const edge e1, const edge e2) {
  if (e1 == e2)
    return;

  EdgeVector &adjacency = nodes[n.id].edges;
  unsigned int e1Pos = UINT_MAX, e2Pos = UINT_MAX;

  for (unsigned int i = 0; i < deg(n); ++i) {
    if (adjacency[i] == e1)
      e1Pos = i;

    if (adjacency[i] == e2)
      e2Pos = i;

    if (e1Pos != UINT_MAX && e2Pos != UINT_MAX)
      break;
  }

  assert(e1Pos != UINT_MAX && e2Pos != UINT_MAX);
  adjacency[e1Pos] = e2;
  adjacency[e2Pos] = e1;
}
//=======================================================
/**
 * @brief restore the given node in the structure and return it
 */
node GraphStorage::restoreNode(const node n) {
  if (nodes.size() <= n.id)
    nodes.resize(n.id + 1);
  else {
    EdgeContainer &ctnr = nodes[n.id];
    ctnr.edges.clear();
    ctnr.outDegree = 0;
  }

  ++nbNodes;

  return n;
}
//=======================================================
/**
 * @brief Add a new node in the structure and return it
 */
node GraphStorage::addNode() {
  return restoreNode(node(nodeIds.get()));
}
//=======================================================
/**
 * @brief Add nb new nodes in the structure
 * and return them in addedNodes
 */
void GraphStorage::addNodes(unsigned int nb, std::vector<node> *addedNodes) {
  if (addedNodes)
    addedNodes->clear();

  if (nb == 0)
    return;

  if (addedNodes)
    addedNodes->reserve(nb);

  unsigned int first = nodeIds.getFirstOfRange(nb);
  unsigned int last = first + nb - 1;
  nodes.reserve(last + 1);

  if (nodes.size() <= first)
    nodes.resize(first);

  unsigned int nodesSize = nodes.size();

  for (; first <= last; ++first) {
    if (nodesSize <= first) {
      nodes.push_back(EdgeContainer());
      ++nodesSize;
    } else {
      EdgeContainer &ctnr = nodes[first];
      ctnr.edges.clear();
      ctnr.outDegree = 0;
    }

    if (addedNodes)
      addedNodes->push_back(node(first));
  }

  nbNodes += nb;
}
//=======================================================
/**
 * @brief remove a node from the nodes structure only
 */
void GraphStorage::removeFromNodes(const node n) {
  nodes[n.id].edges.clear();
  nodeIds.free(n.id);
  nbNodes--;
}
//=======================================================
/**
 * @brief Delete a node and all its adjacent edges in the graph
 */
void GraphStorage::delNode(const node n) {
  assert(isElement(n));
  std::set<edge> loops;
  bool haveLoops = false;

  const EdgeVector &edges = nodes[n.id].edges;

  for (EdgeVector::const_iterator i = edges.begin(); i != edges.end(); ++i) {
    const std::pair<node, node> &iEnds = ends(*i);
    node src = iEnds.first;
    node tgt = iEnds.second;

    if (src != tgt) {
      if (src != n)
        nodes[src.id].outDegree -= 1;

      removeFromEdges(*i, n);
    } else {
      loops.insert(*i);
      haveLoops = true;
    }
  }

  if (haveLoops) {
    std::set<edge>::const_iterator it;

    for (it = loops.begin(); it != loops.end(); ++it) {
      removeFromEdges(*it, n);
    }
  }

  removeFromNodes(n);
}
//=======================================================
/**
 * @brief restore the given edge between src and tgt and return it
 */
edge GraphStorage::restoreEdge(const node src, const node tgt, const edge e, bool updateEndsEdge) {
  if (edges.size() <= e.id)
    edges.resize(e.id + 1);

  edges[e.id].first = src;
  edges[e.id].second = tgt;

  nodes[src.id].outDegree += 1;

  if (updateEndsEdge) {
    nodes[src.id].edges.push_back(e);
    nodes[tgt.id].edges.push_back(e);
  }

  ++nbEdges;

  return e;
}
//=======================================================
/**
 * @brief Add a new edge between src and tgt and return it
 */
edge GraphStorage::addEdge(const node src, const node tgt) {
  return restoreEdge(src, tgt, edge(edgeIds.get()));
}
//=======================================================
/**
 * @brief Add edges in the structure and returns them
 * in the addedEdges vector
 */
void GraphStorage::addEdges(const std::vector<std::pair<node, node>> &ends, std::vector<edge> *addedEdges) {

  if (addedEdges)
    addedEdges->clear();

  unsigned int nb = ends.size();

  if (nb == 0)
    return;

  if (addedEdges)
    addedEdges->reserve(nb);

  unsigned int first = edgeIds.getFirstOfRange(nb);
  edges.reserve(first + nb);

  if (edges.size() < first) {
    edges.resize(first);
  }

  unsigned int edgesSize = edges.size();
  std::vector<std::pair<node, node>>::const_iterator it = ends.begin();
  std::vector<std::pair<node, node>>::const_iterator end = ends.end();

  for (; it != end; ++it, ++first) {
    if (edgesSize == first) {
      edges.push_back(*it);
      ++edgesSize;
    } else
      edges[first] = *it;

    node src = (*it).first;
    node tgt = (*it).second;
    assert(isElement(src));
    assert(isElement(tgt));
    EdgeContainer &ctnr = nodes[src.id];
    ctnr.outDegree += 1;
    edge e(first);
    ctnr.edges.push_back(e);
    nodes[tgt.id].edges.push_back(e);

    if (addedEdges)
      addedEdges->push_back(e);
  }

  nbEdges += nb;
}
//=======================================================
/**
 * @brief Delete an edge in the graph
 */
void GraphStorage::delEdge(const edge e) {
  unsigned srcId = source(e).id;
  nodes[srcId].outDegree -= 1;
  removeFromEdges(e);
}
//=======================================================
/**
 * @brief Delete all edges in the graph
 */
void GraphStorage::delAllEdges() {
  nbEdges = 0;
  edges.clear();

  // loop on nodes to clear adjacency edges
  for (std::vector<EdgeContainer>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
    (*it).edges.clear();
  }
}
//=======================================================
/**
 * @brief Delete all nodes in the graph
 */
void GraphStorage::delAllNodes() {
  delAllEdges();
  nbNodes = 0;
  nodes.clear();
}

// member functions below do not belong to the public API
// they are just needed by the current implementation
//=======================================================
/**
 * @brief remove an edge from an EdgeContainer
 */
void GraphStorage::removeFromEdgeContainer(EdgeContainer &c, const edge e) {
  bool copy = false;
  EdgeVector::iterator previous = c.edges.begin();

  for (EdgeVector::iterator i = previous; i != c.edges.end(); ++i) {
    edge e1 = *i;

    if (copy)
      *previous = e1;

    previous = i;

    if (e1 == e)
      copy = true;
  }

  if (copy)
    c.edges.pop_back();
}
//=======================================================
/**
 * @brief remove an edge from the edges structure
 * and from the EdgeContainer of its ends
 * except for the end node in argument if it is valid
 */
void GraphStorage::removeFromEdges(const edge e, node end) {
  edgeIds.free(e.id);
  nbEdges--;
  std::pair<node, node> &eEnds = edges[e.id];
  // remove from source's edges
  node n = eEnds.first;

  if (n != end)
    removeFromEdgeContainer(nodes[n.id], e);

  // remove from target's edges
  n = eEnds.second;

  if (n != end)
    removeFromEdgeContainer(nodes[n.id], e);
}
