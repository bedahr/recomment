import QtQuick 1.1

Rectangle {
    id: button
    property color buttonColor: "white"
    property alias textColor: btnText.color
    property alias text: btnText.text
    property alias isPressed: buttonMouseArea.pressed
    property alias font: btnText.font
    property int topClickMargin
    property int bottomClickMargin
    property string image : ""

    signal clicked()
    signal pressed()
    signal released()

    border.color: Qt.darker(buttonColor, 1.5)
    border.width: 1


    height: btnText.height + 20
    color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) :
                                     buttonColor


    gradient : Gradient {
        GradientStop { position: 0.1; color: Qt.darker(button.color, 0.5) }
        GradientStop { position: 1.0; color: Qt.darker(button.color, 1.5) }
    }

    MouseArea{
        property bool isHovering : false
        anchors.topMargin: -topClickMargin
        anchors.bottomMargin: -bottomClickMargin
        id: buttonMouseArea
        onClicked: button.clicked()
        onPressed: {
            parent.color = Qt.darker(buttonColor, 2.5)
            button.pressed()
        }
        onReleased: {
            parent.color = Qt.darker(buttonColor, 1.5)
            button.released()
        }
        hoverEnabled: true
        onEntered: {
            parent.color =  Qt.darker(buttonColor, 1.5)

            parent.border.color =  Qt.darker(buttonColor,1.5)
        }
        onExited:  {
            isHovering = false
            parent.color = buttonColor
            parent.border.color = Qt.darker(buttonColor, 1.5)
        }
        anchors.fill: parent
    }

    Image {
        id: img
        visible: image != ""
        anchors.centerIn: parent
        source: image
    }

    Text {
        id: btnText
        anchors.centerIn: parent
        color: "black"
        visible: image == ""
    }

}
