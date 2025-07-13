//Arandelas

cilindro(0,0,0,20,0.9);
cilindro(0,25,0,20,0.9);
cilindro(0,50,0,20,0.9);
cilindro(0,75,0,20,0.9);

cilindro(25,0,0,20,0.7);
cilindro(25,25,0,20,0.7);
cilindro(25,50,0,20,0.7);
cilindro(25,75,0,20,0.7);

cilindro(50,0,0,20,0.5);
cilindro(50,25,0,20,0.5);
cilindro(50,50,0,20,0.5);
cilindro(50,75,0,20,0.5);

translate([-25,0,0])
difference(){
    cylinder(d=7,h=6,$fn=500);
    translate([0,0,2])
    cylinder(d=3.4,h=4.01,$fn=500);
    translate([0,0,4])
    cylinder(d=3.6,h=2.01,$fn=500);
}


module cilindro(x,y,z,diam,alt){
    translate([x,y,z])
        difference(){
        cylinder(d=diam,h=alt,$fn=500);
        translate([0,0,-0.01])
            cylinder(d=3.1,h=2,$fn=500);
        }
}