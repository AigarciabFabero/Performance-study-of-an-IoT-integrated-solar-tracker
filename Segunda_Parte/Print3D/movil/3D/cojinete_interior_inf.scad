//rele interior inferior
rotate([0,180,0])
translate([0,0,-5])
difference(){
union(){
difference(){
    solido();
    translate([13,-13,-1])
        cilindro(0,0,-1,8,8);
    cilindro(20,0,-1,3.1,8);
    cilindro(0,20,-1,3.1,8);
    cilindro(-20,0,-1,3.1,8);
    cilindro(0,-20,-1,3.1,8);  
}
}   
    //Cruz 3.1mm
    cilindro(0,0,-0.01,3.1,7.2);
    cilindro(0,7,-0.1,3.1,7.2);
    cilindro(0,-7,-0.1,3.1,7.2);
    cilindro(7,0,-0.1,3.1,7.2);
    cilindro(-7,0,-0.1,3.1,7.2);
    //cruz 5.5mm
    cilindro(0,0,-0.01,5.5,4.0);
    cilindro(0,7,-0.01,5.5,3);
    cilindro(0,-7,-0.01,5.5,3);
    cilindro(7,0,-0.01,5.5,3);
    cilindro(-7,0,-0.01,5.5,3);
}
module solido(){
translate([0,0,0]){
    cylinder(d=49.8,h=3,$fn=1000);
}
translate([0,0,3]){
    cylinder(d=56,h=2,$fn=1000);
}
}

module cilindro(x,y,z,diametro,altura){
translate([x,y,z])
    cylinder(d=diametro,h=altura,$fn=300);
}
