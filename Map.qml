import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

Item {
    width: 800
    height: 500
    visible: true

    Flickable {
        anchors.fill: parent
        visible: true

        Map {
            id: mapObject
            objectName: "mapObject"
            anchors.fill: parent
            plugin: Plugin {
                name: "osm"
//                PluginParameter { name: "osm.mapping.providersrepository.disabled"; value: "true"}
                PluginParameter { name: "osm.mapping.providersrepository.address"; value: "http://123.57.89.45/dedfaf_posts/qt-osm-map-providers" }
//                PluginParameter { name: "osm.mapping.host"; value: "http://123.57.89.45/dedfaf_posts/qt-osm-map-providers" }
                PluginParameter { name: "osm.mapping.highdpi_tiles"; value: true }
            }
            activeMapType: supportedMapTypes[1] // Choose default map provided by Thunderforest
            center: QtPositioning.coordinate(39.731266726835074, 116.17126161716882)
            zoomLevel: 13.95

            function getMapType(index) {
                return supportedMapTypes[index]
            }

            function refreshMap() {
                mapObject.clearData();
            }

            function moveTo (la, lo) {
                mapObject.center = QtPositioning.coordinate(la, lo)
                console.log("moving cam to " + la + ", " + lo)
            }

//            MapQuickItem {
//                id: marker
//                coordinate: QtPositioning.coordinate(39.906217, 116.391276)
//                anchorPoint: Qt.point(sourceItem.width/2,sourceItem.height/2)
//                sourceItem: Rectangle{
//                    width: 14
//                    height: 14
//                    radius: 7
//                    color: "cyan"
//                    border.color: "black"
//                    border.width: 1
//                }
//            }

            function addMarker(la, lo) {
                console.log("adding marker to " + la + ", " + lo);

                // Create a new marker item
                var newMarker = Qt.createQmlObject('import QtQuick 2.12; import QtPositioning 5.12; import QtLocation 5.12; MapQuickItem { id: marker; coordinate: QtPositioning.coordinate(' + la + ', ' + lo + '); anchorPoint: Qt.point(sourceItem.width / 2, sourceItem.height / 2); sourceItem: Rectangle { width: 14; height: 14; radius: 7; color: "cyan"; border.color: "black"; border.width: 1; } }', mapObject);

                // Add the new marker to the map
                mapObject.addMapItem(newMarker);
            }


            MapQuickItem {
                id: selfLocation
                coordinate: QtPositioning.coordinate(39.728756726835074, 116.17026161716882)
                anchorPoint: Qt.point(sourceItem.width/2,sourceItem.height/2)
                sourceItem: Rectangle{
                    width: 14
                    height: 14
                    radius: 7
                    color: "grey"
                    border.color: "black"
                    border.width: 1
                }
            }
        }
    }
}
