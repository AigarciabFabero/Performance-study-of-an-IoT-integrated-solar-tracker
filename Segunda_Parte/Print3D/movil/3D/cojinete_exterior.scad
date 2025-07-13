//relé exterior
union(){
difference(){
    //hexagono
    translate([0,0,0])
        cylinder(d=150,h=5,$fn=6);
    //cilindro superior
    translate([0,0,2])
        cylinder(d=65,h=7,$fn=1000);
    //cilindro inferior
    translate([0,0,-1])
        cylinder(d=59,h=10,$fn=1000);
    
    cilindro_hexagono(30,50); 
}
}

module cilindro_hexagono(x1,y1){
    translate([x1,y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
    translate([-x1,y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
    translate([x1,-y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
    translate([-x1,-y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
}