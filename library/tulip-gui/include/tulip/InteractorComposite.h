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
#ifndef INTERACTORCOMPOSITE_H
#define INTERACTORCOMPOSITE_H

#include <tulip/Interactor.h>
#include <QtCore/QList>

namespace tlp {

/**
  @class InteractorComposite represent a handler item inside an InteractorComposite

  @see InteractorComposite for details.
  */
class TLP_QT_SCOPE InteractorComponent: public QObject {
  Q_OBJECT

  View* _view;

public:
  virtual void init();
  virtual bool eventFilter(QObject*, QEvent*);
  virtual void clear() {}

  void setView(View* view);
  View* view() const;

  virtual void viewChanged(View *) {}
};

/**
  @class InteractorComposite is a subclass of Interactor that allows building Interactors using component classes focused specifically on input handling.

  The composite behaves like a regular interactor. Event handling is made using InteractorComponent subclasses.
  Basically, an InteractorComposite is a list of InteractorComponent. Each of them are meant to be pushed in the list using the push_back and push_front methods.
  When an event is caught by the InteractorComposite, it will iterate over all components (in th order provided by the list) and run eventFilter on each one of them until one of the component returns true.

  When subclassing InteractorComposite, you should push your components in the construct() method (take care of the order they are pushed in).
  Once the setView() method is called (after construct()), the init() method will be run on every components.
  */
class TLP_QT_SCOPE InteractorComposite : public tlp::Interactor {
  Q_OBJECT
  QAction* _action;
  tlp::View* _view;
  QObject* _lastTarget;

protected:
  QList<tlp::InteractorComponent*> _components;
  /**
    @brief backup a QObject into the InteractorComposite.
    This can be used to store the last target the interactor was installed on. This can be used when uninstall is called.
    */
  void setLastTarget(QObject*);

  /**
    @return The last target the interactor was installed on.
    @note If the lastTarget is destroyed before uninstall was called, the lastTarget() method will return NULL.
    */
  QObject* lastTarget() const;

protected slots:
  void lastTargetDestroyed();

public:
  typedef QList<InteractorComponent*>::iterator iterator;
  typedef QList<InteractorComponent*>::const_iterator const_iterator;

  /**
    @brief Default constructor
    @param icon The icon set on the interactor's action
    @param text The text set on the interactor's action
    */
  InteractorComposite(const QIcon& icon, const QString& text="");
  virtual ~InteractorComposite();

  virtual tlp::View* view() const;
  virtual QAction* action() const;
  virtual QCursor cursor() const;

  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  iterator begin();
  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  iterator end();
  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  const_iterator begin() const;
  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  const_iterator end() const;

  /**
    @brief Pushs an InteractorComponent at the end of the list
    */
  void push_back(InteractorComponent* component);

  /**
    @brief Pushs an InteractorComponent at the beggining of the list
    */
  void push_front(InteractorComponent* component);

public slots:
  virtual void undoIsDone();
  virtual void setView(tlp::View* view);
  virtual void install(QObject* target);
  virtual void uninstall();
};

}
#endif // INTERACTORCOMPOSITE_H
