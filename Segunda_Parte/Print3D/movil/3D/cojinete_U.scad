//Cojinete U

union(){
difference(){
    cylinder(d=20,h=10.7,$fn=3000); 
    
    //cilindro de diametro 13mm
    translate([0,0,8.1])
        cylinder(d=13.15,h=2.61,$fn=1000);
   //cilindro de diametro 10mm
    translate([0,0,5])
        cylinder(d=10.15,h=3.11,$fn=1000);
   //Tornillo
    translate([0,0,-0.01])
        cylinder(d=4.1,h=4,$fn=1000);
    translate([0,0,0])
        cylinder(d=4.4,h=1.4,$fn=1000);  
}  
}