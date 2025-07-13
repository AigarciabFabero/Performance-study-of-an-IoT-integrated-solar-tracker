//cilindro
difference(){
    cylinder(d=20,h=70,$fn=1000);
    //cilindro 1_central hueco:
    translate([0,0,5])
        cylinder(d=15,h=55,$fn=1000);
    //agujeros tornillo parte superior agarre soporte_panel:
    translate([-5,0,59.99])
        cylinder(d=3.1,h=10,$fn=100);
    translate([5,0,59.99])
        cylinder(d=3.1,h=10,$fn=100);
    //Agujeros insertos M3*4*4.2:
    translate([-5,0,65.01])
        cylinder(d=4.1,h=5,$fn=100);
    translate([-5,0,68.51])
        cylinder(d=4.4,h=1.5,$fn=100);    
    translate([5,0,65.01])
        cylinder(d=4.1,h=5,$fn=100);
    translate([5,0,68.51])
        cylinder(d=4.4,h=1.5,$fn=100);
    //Agujeros insertos base 
    translate([0,5,-0.01])
        cylinder(d=4.1,h=4,$fn=100);
    translate([0,5,-0.01])
        cylinder(d=4.4,h=1.5,$fn=100);  
    translate([0,-5,-0.01])
        cylinder(d=4.1,h=4,$fn=100);
    translate([0,-5,-0.01])
        cylinder(d=4.4,h=1.5,$fn=100);         

    
}