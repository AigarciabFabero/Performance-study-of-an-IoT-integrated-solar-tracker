
//rele interior superior
union(){
difference(){
    //estructura solida:
    solida();
    //cilindro central:
    translate([-13,-13,0])
        cilindro(0,0,-1,8,8);
    
    //resto cilindros:
    
    cilindro(0,20,-1,3.1,8);
    cilindro(0,20,2.01,5.6,4);
    cilindro(-20,0,-1,3.1,8);
    cilindro(-20,0,2.01,5.6,4);
    cilindro(0,-20,-1,3.1,8);
    cilindro(0,-20,2.01,5.6,4);
    
    //cilindros interior soporte_servo_2:
    cilindro(20,0,-1,3.1,8);
    translate([0,0,-0.01])
    difference()
        cilindro(0,0,0,3.1,8);
            translate([0,0,2.01])
            cilindro(0,0,0,4.2,4);
    
}
}   
module solida(){
translate([0,0,0]){
    cylinder(d=49.8,h=3,$fn=1000);
}
translate([0,0,3]){
    cylinder(d=56,h=3,$fn=1000);
}
}

module cilindro(x,y,z,diametro,altura){
translate([x,y,z])
    cylinder(d=diametro,h=altura,$fn=1000);
}


