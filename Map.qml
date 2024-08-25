import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

Item {
    width: 512
    height: 512
    visible: true

    Flickable {
        anchors.fill: parent
        width: 512
        height: 512
        visible: true

        Map {
            anchors.fill: parent
            plugin: Plugin {
                name: "osm"
            }
            center: QtPositioning.coordinate(39.731266726835074, 116.17126161716882)
            zoomLevel: 14
        }
    }

}

