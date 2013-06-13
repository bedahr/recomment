import QtQuick 1.1

AnimatedItem {
    id: outerAnim
    property alias mode : itSpeakControl.state

    AnimatedItem {
        id: thankYou
        anchors.fill: parent
        visible: true
        z: 4
        Rectangle {
        x: -10
        y: -10
        width: parent.width + 20
        height: parent.height + 40

        color: "white"
        border.width: 1
        border.color: "black"

        Text {
            anchors.centerIn: parent
            font.pointSize: 42

            text: qsTr("Vielen Dank für Ihre Teilnahme")
        }

        Button {
            width:150
            height: 40
            anchors.right: parent.right
            anchors.bottom:parent.bottom
            anchors.rightMargin: 20
            anchors.bottomMargin: 20
            text: qsTr("Zurück zur Auswahl")
            onClicked: thankYou.state = "hidden"
        }
        }
    }

    Item {
        id: itSpeakControl
        objectName: "itSpeakControl"
        anchors.centerIn: parent
        state: "pushToTalk"
        width: Math.max(btSpeak.width, lbSpeak.width)
        height: Math.max(btSpeak.height, lbSpeak.height) + currentItem.height + 20 /* margins */

        onStateChanged: console.debug("State has changed: " + state)
        states: [
            State {
                name: "pushToTalk"
                PropertyChanges {
                    target: btSpeak
                    visible: recommentView.voiceControlled ? true : false
                }
                PropertyChanges {
                    target: lbSpeak
                    visible: false
                }
            },
            State {
                name: "vad"
                PropertyChanges {
                    target: btSpeak
                    visible: false
                }
                PropertyChanges {
                    target: lbSpeak
                    visible: recommentView.voiceControlled ? true : false
                }
            }
        ]
        Offer {
            id: currentItem
            objectName: "currentRecommendation"
            width: 920
            height: recommentView.voiceControlled ? 395 : 550
            anchors.top: parent.top
            anchors.topMargin: recommentView.voiceControlled ? -40 : 20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: btSpeak
            objectName: "btSpeak"
            text: isPressed ? qsTr("Bitte sprechen") : qsTr("Drücken und halten um zu sprechen") //qsTr("Press and hold to speak")
            width: 620
            height: 70
            font.pointSize: 18
            anchors.top: currentItem.bottom
            anchors.topMargin: 25
            onPressed: {
                recommentView.startRecordingRequested();
            }
            onReleased: {
                commitTimer.restart()
            }

            Timer {
                id: commitTimer
                interval: 500
                repeat: false
                onTriggered: recommentView.commitRecordingRequested();
            }

            bottomClickMargin: 40
            topClickMargin: 40
        }
        Text {
            id: lbSpeak
            objectName: "lbSpeak"
            anchors.top: currentItem.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Please speak...")
            font.pointSize: 18
        }
    }

    ProgressBar {
        id: pbVUMeter
        objectName: "pbVUMeter"
        visible: recommentView.voiceControlled
        width: 620
        height: 50
        anchors.top: itSpeakControl.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: -30
    }

    Text {
        id: recognitionResultBanner
        objectName: "recognitionResultBanner"
        anchors.centerIn: pbVUMeter
        font.pointSize: 14
        visible: recommentView.voiceControlled
        Timer {
            id: resultTimer
            interval: 3500
            repeat: false
            onTriggered: recognitionResultBanner.text = ""
        }

        function recognized(result) {
            text = result
            resultTimer.restart()
        }
    }

    Button {
        id: btFinished
        width:420
        height: 40
        text: qsTr("Diese Kamera auswählen") // qsTr("Choose this camera")
        anchors.top: pbVUMeter.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 18
        anchors.topMargin: recommentView.voiceControlled ? 20 : -50
        onClicked: thankYou.state = "shown"
    }
}
