﻿import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("Hello World")

    id: windows

    //背景
    Image{
        source: "qrc:/img/back.jpg"

        MouseArea{

            onClicked: {

                numberAnimation1.stop();
                console.debug("Hello");
            }
        }
    }

    Image {

        id: img1;
        x: 50
        y: windows.height / 2 - 50
        source: "qrc:/img/img.png"

        NumberAnimation on x{

            id : numberAnimation1
            to : windows.width / 2 - 170
            duration: 400
            loops: Animation.Infinite

        }
    }


    Image {

        id: img2;
        x: windows.width - 200
        y: windows.height / 2 - 80
        source: "qrc:/img/img2.png"

        NumberAnimation on x{

            id : numberAnimation2
            to : windows.width / 2
            duration: 400
            loops: Animation.Infinite
        }

        RotationAnimation on rotation {

            to : 360
            from: 0
            duration: 400
            loops: 100
        }
    }


    MouseArea{

        anchors.fill: parent

        onClicked: {

            numberAnimation1.start();
            numberAnimation2.start();
            timer.start();
        }
    }

    Timer{

        id: timer
        interval: 20
        running: false
        repeat: true

        onTriggered: {

            if(img1.x + 171 > img2.x){

                console.debug("爆炸了");
                timer.stop()
                img1.visible = false;
                img2.visible = false;
            }
        }
    }

    Component.onCompleted: {

        numberAnimation1.stop();
        numberAnimation2.stop();
    }

}
