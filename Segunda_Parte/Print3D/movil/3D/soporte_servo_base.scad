//soporte servo base
difference(){
union(){
//cubo principal:
difference(){
    cubo(-2,-3,0,46.5,34.3,8.4);
    cubo(-0.01,2.8,-0.01,44.52,22.7,10);
}
//cubo que contiene tornillos:
difference(){
    cubo(-10,-10,0,54.5,48.5,3);
    cubo(-0.01,2.8,-0.01,44.52,22.7,10);
    //cilindros lamina inferior:
    cilindro(-6,14.5,-0.01,2.5,5); 
    cilindro(22,-6,-0.01,2.5,5); 
    cilindro(22,34.3,-0.01,2.5,5); 
    }
}   
    cubo(0,2.8,-0.1,44.51,22.7,6.9);
    //cilindros lamina superior 1.6mm
    cilindro(7.53,1.4,-0.1,1.6,10);
    cilindro(7.53,26.9,-0.1,1.6,10);
    cilindro(35.35,1.4,-0.1,1.6,10);
    cilindro(35.35,26.9,-0.1,1.6,10);
    //cilindros lamina superior 3mm
    cilindro(7.53,1.4,-0.1,4,4);
    cilindro(7.53,26.9,-0.1,4,4);
    cilindro(35.35,1.4,-0.1,4,4);
    cilindro(35.35,26.9,-0.1,4,4);
}

module cubo(x,y,z,dx,dy,dz){
translate([x,y,z])
    cube([dx,dy,dz]);
}
module cilindro(x,y,z,diam,alt){
    translate([x,y,z])
        cylinder(d=diam,h=alt,$fn=1000);
}
