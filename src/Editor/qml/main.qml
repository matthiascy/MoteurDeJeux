import QtQuick 2.7
import QtQuick.Controls 2.2
import Engine 1.0

ApplicationWindow {
  id: root
  width: 800
  height: 600
  visible: true
  title: "MoteurDeJeux"

  Viewport {
    id: engineViewport
    visible: true
    focus: true
    anchors.fill: parent

    MouseArea {
      anchors.fill: parent
      onPressed: engineViewport.onMousePress(mouseX, mouseY)
      onReleased: engineViewport.onMouseRelease(mouseX, mouseY)
    }
  }

  Rectangle {
    id: tips
    width: text.implicitWidth + 20
    height: text.implicitHeight + 40
    color: "#AA333333"
    radius: 10
    anchors.left: parent.left
    anchors.leftMargin: 15
    anchors.top: parent.top
    anchors.topMargin: 15

    Text {
      id: text
      anchors.centerIn: parent
      font.pointSize: 9
      color: "white"
      text: "FPS：" + engineViewport.fps +  "\n\n角速度：" +
            engineViewport.angularSpeed.toFixed(3) + "°/s" + "\n\n旋转轴：" +
            "(" + engineViewport.rotateAxis.x.toFixed(3) + "，" + engineViewport.rotateAxis.y.toFixed(3) + "，"
            + engineViewport.rotateAxis.z.toFixed(3) + ")"
    }
  }

  Rectangle {
    id: openglVersion
    width: 100
    height: 20
    color: "#FFFFFFFF"
        radius: 10
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5

        Text {
          id: versionText
          anchors.centerIn: parent
          font.pointSize: 9
          color: "black"
          text: "OpenGL: " + OpenGLInfo.majorVersion + '.' + OpenGLInfo.minorVersion + '.' + OpenGLInfo.profile
        }
  }
}
