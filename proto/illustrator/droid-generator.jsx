function log(a){
    $.writeln(a);
}

for(var i=0; i<activeDocument.symbols.length; i++) {   
    log(activeDocument.symbolItems.add(activeDocument.symbols[i]));
}
