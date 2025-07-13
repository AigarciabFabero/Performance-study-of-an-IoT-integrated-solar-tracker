//Base 
union(){
//Base cuadrada: 
difference(){
    cube([200,200,4],center=true);
    //agujeros esquinas
    cilindro_esquinas(85);
    //agujeros hexagono
    cilindro_hexagono(30,50);
    translate([0,0,-3])
    cylinder(d=10,h=15,$fn=500);
    cilindros_soporte_base(3.4,4.03);
}

//Fin
}
//Modulos
module cilindro_esquinas(x){
    translate([x,x,-6])
        cylinder(d=4,h=20,$fn=500);
    translate([x,-x,-6])
        cylinder(d=4,h=20,$fn=500);
    translate([-x,x,-6])
        cylinder(d=4,h=20,$fn=500);
    translate([-x,-x,-6])
        cylinder(d=4,h=20,$fn=500);
}
module cilindro_hexagono(x1,y1){
    translate([x1,y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
    translate([x1,y1,-3.1])
        cylinder(r1=5.8,r2=1.6,h=1.7,$fn=500);
    translate([-x1,y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
    translate([-x1,y1,-3.1])
        cylinder(r1=5.8,r2=1.6,h=1.7,$fn=500);
    translate([x1,-y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
    translate([x1,-y1,-3.1])
        cylinder(r1=5.8,r2=1.6,h=1.7,$fn=500);
    translate([-x1,-y1,-6])
        cylinder(d=3.1,h=50,$fn=500);
    translate([-x1,-y1,-3.1])
        cylinder(r1=5.8,r2=1.6,h=1.7,$fn=500);
}
module cilindros_centrales(d1,d2,p1,p2){
translate([0,0,-p2+0.5])
    cylinder(d=d1,h=p1,$fn=500);
translate([0,0,0.5])
    cylinder(d=d2,h=p2,$fn=500);
}

//Soporte_servo_base:
module cilindros_soporte_base(diam,alt){
    translate([-11.13,-14.15,-2.01])
        union(){
        cilindro(-6,14.5,-0.01,diam,alt); 
        cilindro(22,-6,-0.01,diam,alt); 
        cilindro(22,34.3,-0.01,diam,alt); 
        }
module cilindro(x,y,z,diam,alt){
    translate([x,y,z])
        cylinder(d=diam,h=alt,$fn=1000);
}
}