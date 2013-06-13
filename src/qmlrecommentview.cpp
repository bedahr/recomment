/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "qmlrecommentview.h"
#include "qmlapplicationviewer/qmlapplicationviewer.h"
#include "settings.h"
#include "recomment.h"
#include "offer.h"
#include "attributemodel.h"
#include <QMetaObject>
#include <QDeclarativeImageProvider>
#include <QDeclarativeEngine>
#include <QGraphicsObject>
#include <QDeclarativeContext>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>
#include <QDebug>
#include <QUuid>

class QMLRecommentImageProvider: public QDeclarativeImageProvider
{
public:
    QMLRecommentImageProvider()
        : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
    {}

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        QPixmap out = m_pixmapCache.value(id);
        if (size)
            *size = out.size();

        if (requestedSize.isValid())
            out = out.scaled(requestedSize,Qt::IgnoreAspectRatio);
        return out;
    }

    QString add(const QPixmap& pixmap) {
        QString id = QUuid::createUuid().toString();
        m_pixmapCache.insert(id, pixmap);
        return id;
    }

    void remove(const QString& id) {
        m_pixmapCache.remove(id);
    }

private:
    QHash<QString, QPixmap> m_pixmapCache;
};


QMLRecommentView::QMLRecommentView(Recomment *recomment, bool voiceControlled, QObject *parent) :
    RecommentView(voiceControlled, parent),
    viewer(new QmlApplicationViewer()),
    viewerImageCache(new QMLRecommentImageProvider),
    attributeModel(new AttributeModel(recomment->getAttributeFactory(), this)),
    state(Unconnected),
    skipNonEssentialUIUpdates(false)
{
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->rootContext()->setContextProperty("recommentView", this);
    viewer->rootContext()->setContextProperty("recomment", recomment);
    viewer->rootContext()->setContextProperty("attributeModel", attributeModel);
    viewer->setMainQmlFile(QLatin1String("app/native/res/qml/recomment/main.qml"));
    viewer->engine()->addImageProvider(QLatin1String("RecommentImages"), viewerImageCache);

    connect(connectButton(), SIGNAL(clicked()),
                                 this, SLOT(connectClicked()));
    connect(disconnectButton(), SIGNAL(clicked()),
                                 this, SIGNAL(disconnectFromServer()));

    restoreConfiguration();
}

QMLRecommentView::~QMLRecommentView()
{
    delete viewer;
    delete attributeModel;
    //delete viewerImageCache; deleted by engine
}

void QMLRecommentView::pauseUpdates()
{
    qDebug() << "Pause";
    skipNonEssentialUIUpdates = true;
}

void QMLRecommentView::resumeUpdates()
{
    qDebug() << "Resume";
    skipNonEssentialUIUpdates = false;
}

void QMLRecommentView::connectClicked()
{
    storeConfiguration();
    emit connectToServer();
}

void QMLRecommentView::startRecordingRequested()
{
    if (state != Active)
        return;
    emit startRecording();
}

void QMLRecommentView::commitRecordingRequested()
{
    if (state != Active)
        return;
    emit commitRecording();
}

void QMLRecommentView::displayStatus(const QString& status)
{
    //qDebug() << "Display status" << status;
    QObject *lbStatus = viewer->rootObject()->findChild<QObject*>("lbStatus");
    lbStatus->setProperty("text", status);
}

void QMLRecommentView::displayError(const QString& error)
{
    qDebug() << "Error: " << error;
    QMetaObject::invokeMethod(viewer->rootObject()->findChild<QObject*>("errorDialog"),
                              "show", Q_ARG(QVariant, error));
}

void QMLRecommentView::displayExecutedAction(const QString& action)
{
    QMetaObject::invokeMethod(viewer->rootObject()->findChild<QObject*>("recognitionResultBanner"),
                              "recognized", Q_ARG(QVariant, action));
}

void QMLRecommentView::displayConnectionState(ConnectionState state_)
{
    state = state_;
    //qDebug() << "State changed: " << state;

    QObject *cb = connectButton();
    switch (state) {
    case Unconnected:
        cb->setProperty("text", tr("Connect"));
        break;
    case Connecting:
        cb->setProperty("text", tr("Connecting..."));
        break;
    default:
        cb->setProperty("text", tr("Connected"));
    }


    QObject *db = disconnectButton();
    switch (state) {
    case ConnectedWaiting:
        db->setProperty("text", tr("Disconnect"));
        break;
    case Connected:
    case Active:
        db->setProperty("text", tr("Disconnect"));
        break;
    case Disconnecting:
        db->setProperty("text", tr("Disconnecting..."));
        break;
    default:
        db->setProperty("text", tr("Disconnected"));
        break;
    }

    viewer->rootObject()->setProperty("state",
        ((state == Unconnected) || (state == Connecting)) ? "disconnected" :
                        ((state == Active) ? "activated" : "connected"));
}

void QMLRecommentView::displayMicrophoneLevel(int level, int min, int max)
{
    if (skipNonEssentialUIUpdates)
        return;

    QObject *pbVUMeter = viewer->rootObject()->findChild<QObject*>("pbVUMeter");
    if (min != -1)
        pbVUMeter->setProperty("minimumValue", min);
    if (max != -1)
        pbVUMeter->setProperty("maximumValue", max);
    pbVUMeter->setProperty("value", level);
}

void QMLRecommentView::displayListening()
{
    speakLabel()->setProperty("text", tr("Listening..."));
}

void QMLRecommentView::displayRecognizing()
{
    QMetaObject::invokeMethod(viewer->rootObject()->findChild<QObject*>("currentRecommendation"),
                                      "displayRecognizing");
    speakLabel()->setProperty("text", tr("Please speak"));
}

void QMLRecommentView::restoreConfiguration()
{
    qDebug() << "Restoring configuration";
    QObject *rootObject = viewer->rootObject();
    rootObject->findChild<QObject*>("cbAutoConnect")->setProperty("checked", Settings::autoConnect());
    rootObject->findChild<QObject*>("teHost")->setProperty("text", Settings::host());
    rootObject->findChild<QObject*>("tePort")->setProperty("text", QString::number(Settings::port()));
    rootObject->findChild<QObject*>("teUserName")->setProperty("text", Settings::user());
    rootObject->findChild<QObject*>("tePassword")->setProperty("text", Settings::password());
    rootObject->findChild<QObject*>("cbPushToTalk")->setProperty("checked", !Settings::voiceActivityDetection());

    qDebug() << "Restoring configuration: Done";
}

void QMLRecommentView::storeConfiguration()
{
    qDebug() << "Storing configuration";
    QObject *rootObject = viewer->rootObject();
    Settings::setAutoConnect(rootObject->findChild<QObject*>("cbAutoConnect")->property("checked").toBool());
    Settings::setHost(rootObject->findChild<QObject*>("teHost")->property("text").toString());
    Settings::setPort(rootObject->findChild<QObject*>("tePort")->property("text").toInt());
    Settings::setUser(rootObject->findChild<QObject*>("teUserName")->property("text").toString());
    Settings::setPassword(rootObject->findChild<QObject*>("tePassword")->property("text").toString());
    Settings::setVoiceActivityDetection(!rootObject->findChild<QObject*>("cbPushToTalk")->property("checked").toBool());
    Settings::store();

    emit configurationChanged();
}

void QMLRecommentView::show()
{
#ifndef Q_OS_BLACKBERRY
    viewer->showNormal();
#else
    viewer->showFullScreen();
#endif
}

QObject* QMLRecommentView::connectButton()
{
    return viewer->rootObject()->findChild<QObject*>("btConnect");
}

QObject* QMLRecommentView::disconnectButton()
{
    return viewer->rootObject()->findChild<QObject*>("btDisconnect");
}

QObject* QMLRecommentView::speakButton()
{
    return viewer->rootObject()->findChild<QObject*>("btSpeak");
}

QObject* QMLRecommentView::speakLabel()
{
    return viewer->rootObject()->findChild<QObject*>("lbSpeak");
}

void QMLRecommentView::displayRecommendation(const Offer* offer, const QString& explanation)
{
    //qDebug() << "Displaying recommendation";
    static QString oldId;
    if (!oldId.isNull()) {
        //qDebug() << "Removing: " << oldId;
        viewerImageCache->remove(oldId);
    }

    oldId = viewerImageCache->add(offer->getImage());
    attributeModel->setAttributes(offer->getAttributeNames(), offer->getAttributes());
    //QVariantMap attributes;
    //foreach (const QString& key, offer.getAttributes().keys())
      //  attributes.insert(key, offer.getAttribute(key)->toString());
    QMetaObject::invokeMethod(viewer->rootObject()->findChild<QObject*>("currentRecommendation"),
                              "recommend",
                              Q_ARG(QVariant, QVariant::fromValue(offer->getName())),
                              Q_ARG(QVariant,
                                    QVariant::fromValue(QLatin1String("image://RecommentImages/")+oldId)),
                              Q_ARG(QVariant, QVariant::fromValue(attributeModel))
                              );
    displayExecutedAction(explanation);
}

void QMLRecommentView::displayNoMatch(const QString& description)
{
    QMetaObject::invokeMethod(viewer->rootObject()->findChild<QObject*>("noMatchDialog"),
                              "show", Q_ARG(QVariant, description));
}
