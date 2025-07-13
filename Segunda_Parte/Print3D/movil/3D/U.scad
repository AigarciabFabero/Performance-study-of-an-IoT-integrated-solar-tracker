//U
translate([40,-19.6,20])
rotate([0,180,-90])
union(){
    
//Esta estructura es la base(_) de la U:
    
translate([0,3.5,0]){
rotate([90,0,0]){
union(){
difference(){
    cube([49.9,20,3.5]);
    translate([14.6,10,-0.01])
        cylinder(d=3.5,h=7,$fn=3000);
    translate([14.6,10,1.21])
        cylinder(r1=1.55,r2=2.5,h=2.3,$fn=500);
    translate([24.6,10,-0.01])
        cylinder(d=3.5,h=7,$fn=3000);
    translate([24.6,10,1.21])
        cylinder(r1=1.55,r2=2.5,h=2.3,$fn=500);
            }
        }
    }
}

//Estructura lateral 1 usa un inserto de M2.5*3*3.5

translate([49.9,0,-32.2]) 
rotate([0,-90,0]) 
union(){
    translate([32.2,-40,0])
        difference(){
        cube([20,40,3.5]);
            translate([10,0,0.51])
                cylinder(d=3.4,h=3,$fn=100);
            translate([10,0,-0.01])
                cylinder(d=3.1,h=5,$fn=100); 
        }
    translate([42.2,-40,0])
        difference(){
            cylinder(d=20,h=3.5,$fn=1000);
            translate([0,0,-0.01])
                cylinder(d=3.1,h=4,$fn=100);   
        }
}


//Estructura lateral 2 en cruz

rotate([0,90,0])
difference(){
union(){
    translate([-20,-40,0])
        cube([20,40,3.5]);
    translate([-10,-40,0])
        cylinder(d=20,h=3.5,$fn=300); 
}   //Hueco:
    translate([-17,-40,-0.1])
        cylinder(d=3.1,h=5.2,$fn=300);
    translate([-3,-40,-0.1])
        cylinder(d=3.1,h=5.2,$fn=300);
    translate([-10,-33,-0.1])
        cylinder(d=3.1,h=5.2,$fn=300);
    translate([-10,-47,-0.1])
        cylinder(d=3.1,h=5.2,$fn=300);
    translate([-10,-40,-0.1])
        cylinder(d=3.1,h=5.2,$fn=300);
}

}

//Cojinete U
/*
translate([0,26.8,10]) 
rotate([90,0,0]) 
union(){
difference(){
        cylinder(d=20,h=10.7,$fn=3000); 
    
    //cilindro de diametro 13mm
    translate([0,0,8.1])
        cylinder(d=13,h=2.61,$fn=1000);
   //cilindro de diametro 10mm
    translate([0,0,5])
        cylinder(d=10,h=3.11,$fn=1000);
   //Tornillo
    translate([0,0,-0.01])
        cylinder(d=3.5,h=5.1,$fn=1000);
    translate([0,0,3])
        cylinder(d=5,h=2.51,$fn=1000);  
}  
}
*/