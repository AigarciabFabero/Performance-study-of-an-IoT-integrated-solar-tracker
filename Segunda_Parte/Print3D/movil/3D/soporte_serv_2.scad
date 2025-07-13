//soporte servo 2
//
rotate([0,0,90])
difference(){
union(){
difference(){
//cubo base
translate([-13.15,-22.25,0]){
    cube([26.3,47.5,4]);
}
//cilindros interior:
translate([0,0,-1]){
cilindro(0,11.15,0,3.1,8,0);}
translate([0,11.15,1.51]){
cylinder(r1=1.7,r2=3.2,h=2.5,$fn=500);}

cilindro(0,11.15,3.5,6.3,0.5);

translate([0,0,-1]){
cilindro(0,-8.85,0,3.1,8,0);}
translate([0,-8.85,1.51]){
cylinder(r1=1.55,r2=3.1,h=2.5,$fn=500);}

//cilindro esquina para que pasen cables:
translate([-13,34.5,-0.1]){
cilindro(0,-10,0,9.5,10,0);}

}

//Barras con tornillos: 

//barra lateral izq
translate([-13.15,-22.25,4]){
    cube([4.5,44.5,4]);}
    
//barra lateral derecha   
translate([8.65,-22.25,4]){
    cube([4.5,44.5,4]);}
}
//Cilindro más proximo al eje del motor
cilindro(-6,13.72,-20,1.8,40,90);
//mas alejado
cilindro(-6,-14.05,-20,1.8,40,90);
}    
//Paredes del Cojinete:
translate([22.25,-13.15,0])
cube([3,26.3,29.5]);
translate([-25.25,-13.15,4])
cube([3,26.3,25.5]);

module cilindro(x,y,z,diametro,altura,rotacion){
rotate([0,rotacion,0])    
    translate([x,y,z])
        cylinder(d=diametro,h=altura,$fn=1000);
}

