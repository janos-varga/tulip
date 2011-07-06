/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -v -c TulipAgentCommunicator -p ../tulip_perspective/include/TulipAgentCommunicator.h:../tulip_perspective/src/TulipAgentCommunicator.cpp resources/org.labri.TulipAgentService.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef TULIPAGENTCOMMUNICATOR_H_1308571708
#define TULIPAGENTCOMMUNICATOR_H_1308571708

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.labri.Tulip
 */
class TulipAgentCommunicator: public QDBusAbstractInterface {
  Q_OBJECT
public:
  static inline const char *staticInterfaceName() {
    return "org.labri.Tulip";
  }

public:
  TulipAgentCommunicator(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

  ~TulipAgentCommunicator();

  Q_PROPERTY(qlonglong pid READ pid)
  inline qlonglong pid() const {
    return qvariant_cast< qlonglong >(property("pid"));
  }

public Q_SLOTS: // METHODS
  inline QDBusPendingReply<> AddPluginRepository(const QString &url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
    return asyncCallWithArgumentList(QLatin1String("AddPluginRepository"), argumentList);
  }

  inline QDBusPendingReply<> CreatePerspective(const QString &name) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(name);
    return asyncCallWithArgumentList(QLatin1String("CreatePerspective"), argumentList);
  }

  inline QDBusPendingReply<> CreatePerspective(const QString &name, const QVariantMap &parameters) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(name) << qVariantFromValue(parameters);
    return asyncCallWithArgumentList(QLatin1String("CreatePerspective"), argumentList);
  }

  inline QDBusPendingReply<> EnableCrashHandling(const QString &folder, qlonglong pid) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(folder) << qVariantFromValue(pid);
    return asyncCallWithArgumentList(QLatin1String("EnableCrashHandling"), argumentList);
  }

  inline QDBusPendingReply<QStringList> GetCompatiblePerspectives(const QString &file) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(file);
    return asyncCallWithArgumentList(QLatin1String("GetCompatiblePerspectives"), argumentList);
  }

  inline QDBusPendingReply<> OpenProject(const QString &file) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(file);
    return asyncCallWithArgumentList(QLatin1String("OpenProject"), argumentList);
  }

  inline QDBusPendingReply<> OpenProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(file) << qVariantFromValue(perspective) << qVariantFromValue(parameters);
    return asyncCallWithArgumentList(QLatin1String("OpenProjectWith"), argumentList);
  }

  inline QDBusPendingReply<> RemovePluginRepository(const QString &url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
    return asyncCallWithArgumentList(QLatin1String("RemovePluginRepository"), argumentList);
  }

  inline QDBusPendingReply<> ShowAboutPage() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowAboutPage"), argumentList);
  }

  inline QDBusPendingReply<> ShowOpenProjectWindow() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowOpenProjectWindow"), argumentList);
  }

  inline QDBusPendingReply<> ShowPluginsCenter() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowPluginsCenter"), argumentList);
  }

  inline QDBusPendingReply<> ShowWelcomeScreen() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowWelcomeScreen"), argumentList);
  }

Q_SIGNALS: // SIGNALS
  void Terminate();
};

namespace org {
namespace labri {
typedef ::TulipAgentCommunicator Tulip;
}
}
#endif
