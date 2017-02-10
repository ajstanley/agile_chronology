// new window script

var NewWin;
NewWin = null;
function OpenWindow(Page,Size) {
    if (NewWin!=null) {
        if (!NewWin.closed) NewWin.close();
    }		
    NewWin = window.open(Page,'Printable',Size);
}