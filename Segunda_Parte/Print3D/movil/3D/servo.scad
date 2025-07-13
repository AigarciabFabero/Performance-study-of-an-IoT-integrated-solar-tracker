//Servomotor

union(){
cube([44.5,22.7,26.3]);  
    
//cilindros laterales:
    
translate([7.53,-1.4,4.5])
    cylinder(d=2.8,h=17.3,$fn=1000);
translate([37.3,-1.4,4.5])
    cylinder(d=2.8,h=17.3,$fn=1000);
translate([7.53,24.1,4.5])
    cylinder(d=2.8,h=17.3,$fn=1000);
translate([37.3,24.1,4.5])
    cylinder(d=2.8,h=17.3,$fn=1000);
    
//cilindros inferiores:
    
translate([11.13,11.35,-3.9])
    cylinder(d=13,h=3.9,$fn=1000);
translate([11.13,11.35,-7])
    cylinder(d=10,h=3.1,$fn=1000);
    
//cilindro superior:
    
translate([11.13,11.35,26.3])
    cylinder(d=20,h=4.6,$fn=1000);
}

