import QtQuick 2.0

Rectangle{
    id: piece
    objectName: "Piece"
    property bool highlight: false
    property bool isQueen: false
    property int initX: -1
    property int initY: -1
    property string baseColor: "white"
    property string highlightColor: "lightyellow"
    radius: board.squareSize/2
    width: 2*radius
    height: width
    color: baseColor
    antialiasing: true
    x: initX
    y: initY

    Rectangle{
        //anchors.centerIn: piece
        radius: parent.radius/2
        width: 2*radius
        height: 2*radius
        x: radius
        y: radius
        //z: piece.z + 1
        color: piece.isQueen ? "red": parent.color
        antialiasing: true
        Behavior on opacity {
            animation: opacityAnim
        }
    }

    MouseArea{
        id:pieceMA
        anchors.fill:parent
        onClicked: {
            if( (game.activePlayer === game.blackPlayer && parent.baseColor === "white") ||
                    (game.activePlayer === game.whitePlayer && parent.baseColor === "black"))
                return;
            root.unhighlightAllItems();
            board.selectedPieceCol = parent.x;
            board.selectedPieceRow = parent.y;
            parent.highlight = true;
            parent.color = parent.highlightColor;
            game.calculateMovesForPiece(parent.y,parent.x);
        }
    }

    Behavior on opacity {
        NumberAnimation{
            id: opacityAnim
            duration: 1000
        }
    }
    onOpacityChanged: {
        if(piece.opacity == 0){
            piece.visible = false;
        }
    }
    onYChanged: {
        if( (piece.baseColor === "black" && piece.y == 7) ||
                (piece.baseColor === "white" && piece.y == 0)){
            piece.isQueen = true;
        }
    }
}
