//Estructura completa:

//Base:

import("base.stl");

//Soporte_servo_base:

translate([-11.13,-14.15,2])
    color("green")
    import("soporte_servo_base.stl");

//servomotor:

translate([-11.13,-11.35,5.9])
    color("blue")
    import("servo.stl");
    
//Soportes:
translate([30,50,2])
    import("soportes.stl");
translate([-30,50,2])
    import("soportes.stl");
translate([30,-50,2])
    import("soportes.stl");
translate([-30,-50,2])
    import("soportes.stl");
    
//rele_exterior:
translate([0,0,36.8])
    import("cojinete_exterior.stl");
rotate([0,180,0])   
    translate([0,0,-47.8])
        import("cojinete_exterior.stl");

//rele:

translate([0,0,42.3])
color("red")
    import("cojinete.stl");
    
//Soporte rele interno:

translate([0,0,36.8])
    color("orange")
    import("cojinete_interior_inf.stl");
    
translate([0,0,42.8])
    color("purple")
    import("cojinete_interior_sup.stl");
  
//Soporte servo horizontal:
translate([11.13,0,48.8])
    color("grey")
    import("soporte_serv_2.stl");

// servo superior horizontal:

translate([0,0,55.6])
    color("blue")
    import("servo_horizontal.stl");

//Union U:

translate([0,-1.7,56.95])
color("brown")
    import("U.stl");
    
//Cojinete U

rotate([90,0,0])
translate([0,66.95,-25.1])
color("red")
import("cojinete_U.stl");

//Cilindro:

rotate([0,90,0])
translate([-66.95,0,43.5])
color("pink")
import("cilindro.stl");

//Soporte panel:
translate([116.5,0,97])
rotate([90,0,90])
color("orange")
import("soporte_panel.stl");

