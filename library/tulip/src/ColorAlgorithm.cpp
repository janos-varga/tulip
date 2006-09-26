//-*-c++-*-

#include "tulip/Types.h"
#include "tulip/ColorAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/ColorProperty.h"

tlp::ColorAlgorithm::ColorAlgorithm (const tlp::PropertyContext &context):tlp::PropertyAlgorithm<tlp::ColorType , tlp::ColorType>(context) {
  colorResult = (tlp::ColorProperty *)context.propertyProxy;
}

tlp::ColorAlgorithm::~ColorAlgorithm() {
}
