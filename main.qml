import QtQuick.Window 2.15
import QtQuick 2.15
import QtQuick.Controls.Material 2.15

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("OpenCV Video Player")
    Material.theme: Material.Dark

    HomePage{
        id: homePage
        anchors.fill: parent
    }
}
