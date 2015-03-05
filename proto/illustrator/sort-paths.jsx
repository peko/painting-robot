for(var s in app.selection) {
    var p = app.selection[s];
    
    if(p.typename!="CompoundPathItem" || p.pathItems.length == 0) continue;
    
    $.writeln(p.name, " ", p.pathItems.length);
    var l = []
    for(var i=0; i<p.pathItems.length; i++) {
        l[i]= [p.pathItems[i].length, i];
    }
    l.sort(function(a,b) {return b[0]-a[0]});
    
    var lr = activeDocument.layers.add();
    lr.name = "sorted";
    var cp = lr.compoundPathItems.add();
    for(var i=0; i<p.pathItems.length>>1; i++) {
        p.pathItems[l[i][1]].duplicate(cp);
    }
};

$.gc();