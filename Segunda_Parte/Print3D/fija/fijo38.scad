//Soporte fijo de 38º de inclinación
difference(){
union(){
translate([0,1.85,-2.36])
difference(){
rotate([38,0,0])
    translate([200,0,0])
        cube([130,150,3]);
rotate([38,0,0])
    translate([215,15,-0.1])
        cube([100,120,3.2]);    
}
//Base
translate([200,0,-3])
difference(){
    cube([130,120.34,3]);
    translate([15,15,-0.01])
        cube([100,90,3.02]);
    translate([65,112.84,0])
        cylinder(d=4,h=10,$fn=100);
    translate([5,60,0])
        cylinder(d=4,h=10,$fn=100);
    translate([125,60,0])
        cylinder(d=4,h=10,$fn=100);
}

translate([200.01,110.34,-3])
union(){
//Soporte izq  
cube([14.98,10,95.34]);
//Soporte drch
translate([115,0,0])
    cube([15,10,95.35]);
}

//Laminas laterales de sujecion
//superiores
translate([330,114.34,-3])
    cube([2,6,95.34]);
translate([+198.01,114.34,-3])
    cube([2,6,95.34]);
//inferiores:
translate([330,-3,-3])
    cube([2,10,6]);
translate([198,-3,-3])
    cube([2,10,6]);

//Cubo sin más
translate([200,-3,-3])
    cube([130,3,6]);
}
//triángulos 1
translate([200,114.53,99])
rotate([-22,0,0])
rotate([0,90,0])
cylinder(r=15,h=15,$fn=3);
//triángulo 2
translate([314.99,114.53,99])
rotate([-22,0,0])
rotate([0,90,0])
cylinder(r=15,h=15.03,$fn=3);
}