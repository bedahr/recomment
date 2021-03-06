import QtQuick 1.1

Item {
    function escapeBrackets(input) {
        return input.replace(" \(.*\)", "")
    }

    height: recommentView.voiceControlled ? 40 : 60
    Text {
        anchors.right: data.left
        anchors.rightMargin: 10
        anchors.top: parent.top
        id: teKey
        text: "<b>"+key+":</b>"
        font.pointSize: recommentView.voiceControlled ? 16 : 14
    }
    Item {
        id: data
        anchors.left: parent.left
        anchors.top: teKey.top
        anchors.leftMargin: Math.max(recommentView.voiceControlled ? 320 : 270, 10 + teKey.width)

        Button {
            id: btChange
            width: 80
            height: 25
            text: qsTr("Ändern") //qsTr("Change")
            visible: !recommentView.voiceControlled && !isCountable
            anchors.left: parent.left
            anchors.top: parent.top
            onClicked: recomment.critique(escapeBrackets(key) + " ÄNDERN")
            //topClickMargin: 20
        }
        Button {
            id: btLower
            width: 80
            height: 25
            visible: (!recommentView.voiceControlled && isCountable) ? true : false
            anchors.left: parent.left
            anchors.top: parent.top
            //topClickMargin: 20
            buttonColor: "white"
            image: "/usr/share/icons/oxygen/22x22/actions/list-remove.png"
            onClicked: recomment.critique("WENIGER " + escapeBrackets(key))
        }
        Text {
            id: teValue
            anchors.top: parent.top
            anchors.left: parent.left

            anchors.leftMargin: (!recommentView.voiceControlled ? (isCountable ?
                                                                       ((btRaise.anchors.leftMargin - btLower.width) / 2) + btLower.width - (width / 2)
                                                                     : btChange.width  + 5)
                                                                : 0)
            text: value
            font.pointSize: recommentView.voiceControlled ? 16 : 14
        }
        Rectangle {
            id: bar
            visible: false //isCountable ? true : false
            width: recommentView.voiceControlled ? 150 : 299
            anchors.top: teValue.bottom
            anchors.topMargin: recommentView.voiceControlled ? 0 : 4
            anchors.left: recommentView.voiceControlled ? teValue.left : parent.left
            height: 5
            border.color: "gray"
            border.width: 1

            Rectangle {
                id: indicator
                anchors.left: bar.left
                anchors.top: bar.top
                height: bar.height
                width: 5
                color: "gray"
                anchors.leftMargin: bar.width * ((realValue - minValue) / (maxValue - minValue)) - 2.5

            }
        }

        Button {
            id: btRaise
            width: 80
            height: 25
            visible: (!recommentView.voiceControlled && isCountable) ? true : false
            anchors.left: parent.left
            anchors.leftMargin: 220
            anchors.top: parent.top
            buttonColor: "white"
            image: "/usr/share/icons/oxygen/22x22/actions/list-add.png"
            onClicked: recomment.critique("MEHR " + escapeBrackets(key))
            //topClickMargin: 20
        }
    }
}
