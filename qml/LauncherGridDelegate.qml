import QtQuick 2.9
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import MeuiKit 1.0 as Meui

Item {
    id: control

    property bool dragStarted: false
    property var dragItemIndex: index

    Drag.active: iconMouseArea.drag.active
    Drag.dragType: Drag.Automatic
    Drag.supportedActions: Qt.MoveAction
    Drag.hotSpot.x: icon.width / 2
    Drag.hotSpot.y: icon.height / 2

    Drag.onDragStarted:  {
        dragStarted = true
    }

    Drag.onDragFinished: {
        dragStarted = false
    }

    DropArea {
        anchors.fill: icon
        enabled: true

        onContainsDragChanged: {
            if (drag.source)
                launcherModel.move(drag.source.dragItemIndex, control.dragItemIndex)
        }
    }

//    Rectangle {
//        anchors.fill: parent
//        radius: Meui.Theme.bigRadius
//        color: "black"
//        opacity: 0.5
//    }

    Image {
        id: icon

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            bottom: label.top
            margins: Meui.Units.largeSpacing * 2
        }

        property real size: height

        source: "image://icontheme/" + model.iconName
        sourceSize.width: height
        sourceSize.height: height
        width: height
        height: width
        cache: true

        visible: !dragStarted

        ColorOverlay {
            id: colorOverlay
            anchors.fill: icon
            source: icon
            color: "#000000"
            opacity: 0.5
            visible: iconMouseArea.pressed
        }
    }

    Menu {
        id: _itemMenu

        modal: true

        MenuItem {
            text: qsTr("Open")
            onTriggered: launcherModel.launch(model.appId)
        }

        MenuItem {
            text: qsTr("Uninstall")
            onTriggered: {}
        }
    }

    MouseArea {
        id: iconMouseArea
        anchors.fill: icon
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.axis: Drag.XAndYAxis

        onClicked: {
            if (mouse.button == Qt.LeftButton)
                launcherModel.launch(model.appId)
            else if (mouse.button == Qt.RightButton)
                _itemMenu.popup()
        }

        onPositionChanged: {
            if (pressed) {
                if (mouse.source !== Qt.MouseEventSynthesizedByQt) {
                    drag.target = icon
                    icon.grabToImage(function(result) {
                        control.Drag.imageSource = result.url
                    })
                } else {
                    drag.target = null
                }
            }
        }

        onReleased: {
            drag.target = null
        }
    }

    TextMetrics {
        id: fontMetrics
        font.family: label.font.family
        text: label.text
    }

    Label {
        id: label

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            margins: Meui.Units.smallSpacing
        }

        visible: !dragStarted

        text: model.name
        elide: Text.ElideRight
        textFormat: Text.PlainText
        wrapMode: "WordWrap"
        horizontalAlignment: Text.AlignHCenter
        width: parent.width - 2 * Meui.Units.smallSpacing
        height: fontMetrics.height
        color: "white"
    }
}