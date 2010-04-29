//-*-c++-*-
#ifndef MOUSEMOVESELECTION_H
#define MOUSEMOVESELECTION_H

//#include <tulip/GlRect.h>
#include <tulip/GlCircle.h>
#include <tulip/GlRect.h>
#include <tulip/GlComposite.h>
#include <tulip/Observable.h>
#include <tulip/InteractorComponent.h>
#include <tulip/GlComplexPolygon.h>

/** \addtogroup Mouse_interactor */
/*@{*/

namespace tlp {

  /// This interactor allows to move/rotate/stretch the current selection layout
  class TLP_QT_SCOPE MouseSelectionEditor:public InteractorComponent {

  public:

    MouseSelectionEditor();
    ~MouseSelectionEditor();
    bool compute(GlMainWidget *glMainWidget);
    bool draw(GlMainWidget *);
    bool eventFilter(QObject *, QEvent *);
    InteractorComponent *clone() { return new MouseSelectionEditor(); }

  private:
    enum EditOperation { NONE=0, ROTATE_Z, ROTATE_XY, STRETCH_X, STRETCH_Y, STRETCH_XY, TRANSLATE, ALIGN_TOP, ALIGN_BOTTOM, ALIGN_LEFT, ALIGN_RIGHT, ALIGN_VERTICALLY, ALIGN_HORIZONTALLY};
    enum OperationTarget { COORD = 0, SIZE, COORD_AND_SIZE};

    Graph *_graph;
    GlMainWidget *glMainWidget;
    LayoutProperty *_layout;
    BooleanProperty *_selection;
    DoubleProperty *_rotation;
    SizeProperty *_sizes;
    LayoutProperty *_copyLayout;
    SizeProperty *_copySizes;
    DoubleProperty *_copyRotation;

    void initProxies(GlMainWidget *glMainWidget);
    void initEdition();
    void undoEdition();
    void stopEdition();

    EditOperation operation;
    OperationTarget mode;

    Coord editCenter;
    Coord editPosition;
    Coord editLayoutCenter;

    //    int x,y;
    //    double centerX, centerY;
    Coord ffdCenter;

    GlLayer *layer;
    GlComposite *composite;
    GlCircle _controls[8];
    GlComplexPolygon _advControls[6];
    GlRect centerRect;
    GlRect advRect;
    Coord _layoutCenter;

    bool computeFFD(GlMainWidget*);
    void getOperation(GlEntity *select);
    void mMouseTranslate(double, double, GlMainWidget*);
    void mMouseRotate(double, double, GlMainWidget*);
    void mMouseStretchAxis(double, double, GlMainWidget*);
    void mAlign(EditOperation operation,GlMainWidget*);


  };

}
/*@}*/

#endif
