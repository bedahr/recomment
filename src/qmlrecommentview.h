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

#ifndef QMLRECOMMENTVIEW_H
#define QMLRECOMMENTVIEW_H

#include "recommentview.h"
#include <QVariant>
#include <QStringList>

class QmlApplicationViewer;
class Recomment;
class QMLRecommentImageProvider;
class AttributeModel;

class QMLRecommentView : public RecommentView
{
    Q_OBJECT

signals:
    void publicCommandsChanged();

public:
    QMLRecommentView(Recomment *recomment, bool voiceControlled, QObject *parent=0);
    ~QMLRecommentView();

public slots:
    void show();

    void displayStatus(const QString& status);
    void displayError(const QString& error);
    void displayConnectionState(ConnectionState state);

    void storeConfiguration();
    void restoreConfiguration();

    void startRecordingRequested();
    void commitRecordingRequested();

    void displayExecutedAction(const QString& action);

    void displayMicrophoneLevel(int level, int min, int max);
    void displayListening();
    void displayRecognizing();

    void pauseUpdates();
    void resumeUpdates();

    void displayRecommendation(const Offer* offer, const QString &explanation);
    void displayNoMatch(const QString& description);

private:
    QmlApplicationViewer *viewer;
    QMLRecommentImageProvider *viewerImageCache;
    AttributeModel *attributeModel;
    ConnectionState state;
    bool skipNonEssentialUIUpdates;

    QStringList currentLanguagePublicCommands;

    QObject* connectButton();
    QObject* disconnectButton();
    QObject* speakButton();
    QObject* speakLabel();

private slots:
    void connectClicked();
};

#endif // QMLRECOMMENTVIEW_H
