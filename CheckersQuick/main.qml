import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import CheckersGame 1.0
import CheckersBoard 1.0
Window {
    id: root
    visible: true
    width: 800
    height: width
    title: "Checkers"

    CheckersGame{
        id:game
    }
    Item{
        id: buttons
        anchors.centerIn: parent
        Keys.forwardTo: board
        ColumnLayout{
            anchors.fill: parent
            Button{
                id:playButton
                text:"Play"
                //anchors.centerIn:parent
                onClicked: {
                    game.newGame();
                    root.resetBoard();
                    buttons.visible = false;
                    board.visible = true;
                }
            }
            Button{
                id: exitButton
                text: "Exit"
                onClicked:{ root.close() }
            }
        }
    }

    function unhighlightAllItems()
    {
        for( var r=0; r < board.rows; r++){
            for(var c=0; c<board.columns; c++){
                var item = board.childAt(r,c);
                if(item && item.highlight){
                    item.color = item.baseColor
                    item.highlight = false;
                }
            }
        }
    }

    function resetBoard()
    {   console.log("RESET: "+board.data.length);
        for( var child in board.data)
        {
            console.log("child: "+child.objectName);
            if(child.objectName === "Piece")
            {
                //console.log("piece: "+ child.initY, " "+ chil.initX);
                child.x = child.initX;
                child.y = child.initY;
                child.opacity = 1;
                child.visible = true;
            }
        }
    }
    Grid{
        id:board
        columns: 8
        rows: columns
        visible: false
        property int piecesCount: 12
        property int squareSize: 1
        property int selectedPieceRow: -1
        property int selectedPieceCol: -1
        scale:Math.min(parent.width / width, parent.height / height)
        anchors.centerIn: parent
        height: width
        ParallelAnimation{
            id: moveAnimation
            property Rectangle targetPiece: Rectangle{}
            property int xEnd: -1
            property int yEnd: -1
            property int duration: 1000

            NumberAnimation {
                target: moveAnimation.targetPiece
                property: "y"
                to: moveAnimation.yEnd
                duration: moveAnimation.duration
            }
            NumberAnimation {
                target: moveAnimation.targetPiece
                property: "x"
                to: moveAnimation.xEnd
                duration: moveAnimation.duration
            }
        }

        Connections{
            target: game.board
            onPieceMoved: {
                root.unhighlightAllItems();
                var piece = board.childAt(scol, srow);
                if(!piece)
                    return;
                var animation = moveAnimation;
                animation.targetPiece = piece;
                animation.xEnd = tcol; animation.yEnd = trow;
                animation.restart();
            }
            onPieceCaptured: {
                var piece = board.childAt(col,row);
                piece.opacity = 0;
                //piece.visible = false;
            }
        }

        Connections{
            target: game
            onAvailableMovesCalculated:{
                if(!availablePos || availablePos.lenght < 2)
                    return;
                for(var i = 0; i < availablePos.length; i+=2)
                {
                    var square = board.childAt(availablePos[i+1], availablePos[i]);
                    square.highlight = true;
                    square.color = square.highlightColor;
                }
            }           
        }

        Keys.enabled: true
        Keys.onEscapePressed: {
            board.visible=false;
            buttons.visible = true;
        }

        //Squares
        Repeater{
            model: board.rows * board.columns
            Rectangle{
                id:square
                objectName: "Square"
                readonly property int col: index % board.columns
                readonly property int row: Math.floor(index/board.rows)
                property string baseColor: (row+col)%2 ? "gray":"snow"
                property string highlightColor: "palegreen"
                property bool highlight: false

                width: board.squareSize
                height: board.squareSize
                color: baseColor

                MouseArea{
                    id: squareMA
                    anchors.fill: parent
                    onClicked: {
                        if(!parent.highlight){
                            return;
                        }
                        game.movePiece(board.selectedPieceRow, board.selectedPieceCol, parent.row, parent.col);
                    }
                }
            }
        }

        //White pieces
        Repeater{
            model:board.piecesCount
            Piece{
                baseColor:"white"
                highlightColor: "lightyellow"
                initX: [0,2,4,6][index%4] + (y+1)%2
                initY: [5,6,7][Math.floor(index/4)]
                z: 10
            }
        }
        //Black pieces
        Repeater{
            model:board.piecesCount
            Piece {
                baseColor:"black"
                highlightColor: "gold"
                initX: [0,2,4,6][index%4] + (y+1)%2
                initY: Math.floor(index/4)
                z: 10
            }
        }
    }
}
