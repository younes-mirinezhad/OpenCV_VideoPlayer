import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0
import QtLocation 5.12
import QtQuick.Controls.Material 2.15
import app.OpenCV_Player_ViewPort 1.0
import app.OpenCV_VideoPlayer 1.0

Pane {
    anchors.fill: parent

    OpenCV_VideoPlayer{
        id: openCV_Player
        viewPort: openCV_ViewPort
        onVideoFinished: openCV_Player.setPlaybackState(OpenCV_VideoPlayer.StoppedState)
    }
    FileDialog {
        id: selection_VideoFile
        title: "Select Video File"
        nameFilters: [ "Video files (*.mp4 *.MP4 *.Mp4)" ]
        onAccepted:{
            videoUrl_txb.text=file
            openCV_Player.videoUrl=file
            currentFile=file
        }
    }

    ColumnLayout{
        anchors.fill: parent

        RowLayout{
            Label{
                text: "Video File:"
                Layout.preferredWidth: 75
            }
            TextField{
                id: videoUrl_txb
                Layout.fillWidth: true
                readOnly: true
            }
            Button{
                text: "Select"
                Layout.preferredWidth: 75
                Layout.preferredHeight: 40
                onClicked: {
                    if(openCV_Player.playbackState === OpenCV_VideoPlayer.PlayingState)
                        openCV_Player.setPlaybackState(OpenCV_VideoPlayer.PausedState)
                    selection_VideoFile.open()
                }
            }
        }
        RowLayout{
            Item {
                id: video_box
                Layout.fillWidth: true
                Layout.fillHeight: true

                Item {
                    id: vidRatio_box
                    anchors.centerIn: parent
                    property real _orgVidRatio: openCV_Player.inputResulation.width/openCV_Player.inputResulation.height
                    property real _parentRatio: video_box.width/video_box.height

                    width: _orgVidRatio>=_parentRatio ? video_box.width : video_box.height*_orgVidRatio
                    height: _orgVidRatio>=_parentRatio ? video_box.width/_orgVidRatio : video_box.height

                    OpenCV_Player_ViewPort{
                        id: openCV_ViewPort
                        anchors.fill: parent
                    }
                }
            }
        }
        RowLayout{
            Label{
                id: ctrl_frameNumber
                text: "Frame: " + openCV_Player.frameNumber + " / " + openCV_Player.videoFrameCount
                Layout.preferredWidth: 100
            }
            Item {Layout.fillWidth: true}
            Rectangle{
                id: ctrl_Back
                width: 20
                height: 20
                radius: 5

                Image {
                    anchors.fill: parent
                    source: "qrc:/Icons/Previous.png"
                }
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    ToolTip.text: "Previous frame"
                    onEntered: ToolTip.visible = true
                    onExited: ToolTip.visible = false
                    onClicked: {openCV_Player.setFrameNumber(openCV_Player.frameNumber - 1)}
                }
            }
            Rectangle{
                id: ctrl_Play
                width: 20
                height: 20
                radius: 5

                Image {
                    anchors.fill: parent
                    source: openCV_Player.playbackState === OpenCV_VideoPlayer.PlayingState ? "qrc:/Icons/pause.png" : "qrc:/Icons/play.png"
                }
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    ToolTip.text: "Play/Pause"
                    onEntered: ToolTip.visible = true
                    onExited: ToolTip.visible = false
                    onClicked: {
                        if(openCV_Player.playbackState !== OpenCV_VideoPlayer.PlayingState)
                            openCV_Player.setPlaybackState(OpenCV_VideoPlayer.PlayingState)
                        else
                            openCV_Player.setPlaybackState(OpenCV_VideoPlayer.PausedState)
                    }
                }
            }
            Rectangle{
                id: ctrl_Next
                width: 20
                height: 20
                radius: 5

                Image {
                    anchors.fill: parent
                    source: "qrc:/Icons/Next.png"
                }
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    ToolTip.text: "Next frame"
                    onEntered: ToolTip.visible = true
                    onExited: ToolTip.visible = false
                    onClicked: {openCV_Player.setFrameNumber(openCV_Player.frameNumber+1)}
                }
            }
            Item {Layout.fillWidth: true}
        }
        RowLayout{
            Item {
                id: ctrl_pb
                Layout.fillWidth: true
                Layout.preferredHeight: 7

                Rectangle {
                    anchors.fill: parent
                    color: "lightGray"

                    Rectangle {
                        anchors{
                            top: parent.top
                            bottom: parent.bottom
                            left: parent.left
                        }
                        color: "#757575"
                        width: openCV_Player.frameNumber > 0 ?
                                   parent.width*openCV_Player.frameNumber/openCV_Player.videoFrameCount : 0
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var _fNum = openCV_Player.videoFrameCount*mouse.x/width|0
                            openCV_Player.setFrameNumber(_fNum)
                        }
                    }
                }
            }
        }
    }
}
