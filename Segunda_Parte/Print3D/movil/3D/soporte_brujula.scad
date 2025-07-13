//Soporte brujula

difference(){
union(){
cube([78.50,130,3]);  //base
translate([0,30,0]) //vertical
    cube([3,100,18]);
translate([0,127,0]) //Horizontal
    cube([59,3,18]);
translate([56,43,0])
    cube([3,29.5,17.65]);
/*
translate([-50,44,0]) //rama
    cube([70,5,3]);
translate([-30,44,0]) //rama    
    cube([5,83,3]);
translate([-15,44,0]) //rama    
    cube([5,83,3]);
translate([-30,125,0]) //rama   
    cube([70,5,3]);
translate([-30,100,0]) //rama   
    cube([70,5,3]);
translate([-30,70,0]) //rama   
    cube([70,5,3]);
*/
//Cilindro
translate([13,76,0])
    cylinder(d=4.6,h=7,$fn=500);
}

translate([59,-0.01,-1.01]) //cubo1
    cube([30,131,4.1]); 
translate([-0.01,-0.01,-1.01]) //cubo2
    cube([59.1,15.1,4.1]); 

//rajas I:
translate([15.35,90.2,-1.01]) //I
    cube([29.8,3.2,7]);
translate([15.35,39.6,-1.01]) //I
    cube([29.8,3.2,7]);
translate([28.65,39.61,-1.01])    
    cube([3.25,50.6,7]); // I

//Cilindros:
translate([1.21,45,10.5])
rotate([0,90,0])
union(){
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

/*
translate([-50,49,0])
    cube([13.7,2,3]);
*/


}