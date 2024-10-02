import QtQuick 2.12
import QtQuick.Window 2.13
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Extras.Private 1.0


Window {
	id : root
	width: 400
	maximumWidth : width
	minimumWidth : width
    height: 400
	maximumHeight : height
	minimumHeight : height
	title:"lampu"
	color : "#94CEF2"
    visible: true
    flags: Qt.Dialog
	
	Rectangle{
		width : parent.width 
		height : parent.height/2
		color : "red"
	
	}
	Rectangle{
		y : parent.height/2
		width : parent.width 
		height : parent.height/2
		color : "white"
	
	}
	
	
	Text{
	x:10
	y:5
	width: 400
	text : "Voice Switch"
	color : "white"
	font.pixelSize : 24
	verticalAlignment: Text.AlignVCenter
	}
	
	Button{
		anchors.verticalCenter: parent.verticalCenter
		anchors.horizontalCenter: parent.horizontalCenter
		width : 100
		height : 100
		text : "ngomong"
		onClicked :{
			backend.button("push")
			
		}
	}
	
	
	Text{
	id : hasil
	y:300
	anchors.horizontalCenter: parent.horizontalCenter

	text : "hasil"
	color : "red"
	font.pixelSize : 24
	verticalAlignment: Text.AlignVCenter
	}
	
	
	Timer{
		id:tmgauge
		interval: 500
		repeat: true
		running: true
		onTriggered: {
			hasil.text = backend.test_message()
			
		}
	}
	
	
	
}













