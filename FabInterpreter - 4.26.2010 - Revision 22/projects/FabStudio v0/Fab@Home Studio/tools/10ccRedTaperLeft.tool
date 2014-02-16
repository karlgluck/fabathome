// This is a tool file for the 2-Syringe Tool, left 10ml syringe with Red Taper tip
; written by Evan Malone, April 11, 2007

DESCRIPTION 	Left 10cc Syringe (EFD 5111LL-B), Red (0.25mm ID) taper tip (EFD 5125TT-B)
PATHWIDTH	0.4 mm
PATHHEIGHT	0.3 mm
TOOLGEOM	New2SyringeTool.STL
TOOLOFFSET	200 -220 -151	 // dx dy dz offset of graphics display of entire tool
TOOLCOLOR	0.2 0.2 0.2 0.5  // R G B Alpha in range 0-1 (alpha=transparency)
BARRELGEOM	New2Syringe10ccBarrelLeft.STL  
BARRELOFFSET	0 0 -10  
BARRELCOLOR	0.2 0.2 0.2 0.5  // R G B Alpha in range 0-1 (alpha=transparency)
BARRELDIAM	15.8		//internal diameter of syringe barrel, mm
TIPGEOM		New2SyringeAmberNeedleLeft.STL  
TIPOFFSET	0 0 0
TIPCOLOR	1 0 0 0.9  // R G B Alpha in range 0-1 (alpha=transparency)
TIPDIAM		0.25		//internal diameter of syringe needle/tip, mm
TIPPOINT	99,-332.478,-60  //(x,y,z tip location in geometry file coordinates)
PLUNGERGEOM	New2Syringe10ccPlungerLeft.STL  
PLUNGEROFFSET	0 0 80 //(in geometry file coordinates)
PLUNGERCOLOR	0 1 0 0.5  // R G B Alpha in range 0-1 (alpha=transparency)
DIRECTION	0 0 -1	//orientation of graphical display of motion of plunger
VOLUME		10 cc
MOTOR		3 -1 0.000396875 //amp axis #, axis direction(+/-1), mm per step 
PUSHOUT		0.22 sec // early dispensing to start flow quickly
SUCKBACK	0.21 sec // reverse plunger motion to stop flow quickly 
RANGE		0 80 0 // [mm] start end home
DEPOSITIONRATE  0.00068 //(mm plunger motion per mm of path length)
BACKFILL	12 mm
LIMITSWITCH	0 0 //+direction -direction (0 -> limit switch not connected; 1 if it is)
INCREMENT	0.1 //distance to move plunger in manual jog
CLEARANCE	3.0 //clearance between tip and last layer when traversing
PATHSPEED	10.0	//speed along paths during deposition, mm/s

; note: The units "mm" and "cc" are labels only, they are not read by the software
; For EFD parts see http://www.efd-inc.com/catalogs/EFD-ULTRA-Catalog.pdf
; For Gauge diamaters see http://www.graphicproducts.com/supplies/wire_gauge.html
