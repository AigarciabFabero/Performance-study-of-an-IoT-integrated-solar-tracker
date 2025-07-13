//soporte panel
union(){
difference(){
    cube([140,160,6],center=true);
    //cubo vacio central:
    translate([0,0,1.5])
        cube([130,150,3.01],center=true);
    //tres rectangulos centrales:
    translate([-15,-65,-4])
        cube([30,130,10]);
    translate([35,-65,-4])
        cube([20,130,10]);
    translate([-55,-65,-4])
        cube([20,130,10]);
    //laterales:
    translate([0,0,1.5])
        cube([100,200,3.01],center=true);
    translate([0,0,1.5])
        cube([200,120,3.01],center=true);
    }
}

//Cuando cambie los agujeros, comentar el translate que se ve mejor en el dibujo.

translate([0,-30,-1.5])
union(){
difference(){
    cube([70,20,3],center=true);
    //cilindro 1_central:
    translate([0,-5,-1.51])
        cylinder(d=3.1,h=1.52,$fn=100);
    translate([0,-5,-0.5])
        cylinder(r1=1.6,r2=3,h=2.3,$fn=500);
    //cilindro 2_central:
    translate([0,5,-1.51])
        cylinder(d=3.1,h=6,$fn=100);
    translate([0,5,-0.5])
        cylinder(r1=1.6,r2=3,h=2.3,$fn=500);
    }
}
