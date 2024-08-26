import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

Item {
    width: 1024
    height: 512
    visible: true

    Flickable {
        anchors.fill: parent
        visible: true

        Map {
            anchors.fill: parent
            plugin: Plugin {
                name: "osm"
                PluginParameter { name: "osm.mapping.providersrepository.address"; value: "http://123.57.89.45/dedfaf_posts/qt-osm-map-providers" }
                PluginParameter { name: "osm.mapping.highdpi_tiles"; value: false }
            }
            activeMapType: supportedMapTypes[0] // Cycle map provided by Thunderforest
            center: QtPositioning.coordinate(39.731266726835074, 116.17126161716882)
            zoomLevel: 14
        }
    }

}

