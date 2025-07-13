//Acople para la brujula
color("red")
translate([0,0,-3])
import("acople1.stl");

//Acople 2;
cube([29.6,3,120]);
translate([0,50.6,0])
    cube([29.6,3,120]);
translate([13.3,0,0])    
    cube([3,50.6,120]);
translate([14.8,0,101])    
    cylinder(r1=0,r2=15,h=15,$fn=500);
translate([14.8,50.6,101])    
    cylinder(r1=0,r2=15,h=15,$fn=500); 
//soporte brujula
color("green")
translate([-15.45,-39.7,117])
    import("soporte_brujula.stl");