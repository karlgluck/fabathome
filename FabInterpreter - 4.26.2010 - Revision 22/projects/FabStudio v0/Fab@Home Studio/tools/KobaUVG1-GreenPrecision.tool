// This is a tool file for a 10ml syringe with Green Precision tip with Koba UVG-1 normal viscosity UV cure resin
// The Xylotex amplifier generates 1/8 steps by default, so adjust distance per step here

//////////////////FREQUENTLY CHANGED PARAMETERS//////////////////////////////////////////////////////

DESCRIPTION 	10cc Syringe (EFD 5111LL-B), Green (0.84mm ID) precision tip (EFD 5125TT-B), Koba UVG-1 normal viscosity UV cure resin
TIPDIAM		0.84				//[mm], internal diameter of syringe needle/tip
PATHWIDTH	0.86				//[mm] width of stream of material deposited
PATHHEIGHT	0.77				//[mm] height of layers of material deposited
PUSHOUT		.035	  			//[seconds] early dispensing to start flow quickly
SUCKBACK	0.045				//[seconds] reverse plunger motion to stop flow quickly.  0.05588 Max 
SUCKBACKDELAY	0.045
DEPOSITIONRATE  0.0035				//(mm plunger motion per mm of path length)
CLEARANCE	2.0				//[mm] clearance between tip and last layer when traversing
PATHSPEED	1.0				//[mm/s] speed along paths when depositing material
PAUSEPATHS	20				//number of paths after which to trigger an automatic pause
MINPATHLENGTH   0.25				//[MM] minumum path legnth

//////////////////RARELY CHANGED PARAMETERS//////////////////////////////////////////////////////////

TOOLGEOM	New1SyringeTool.STL 		//name of the stl file describing the tool body geometry
TOOLOFFSET	200 -220 -151			//dx dy dz graphics offset of tool body
TOOLCOLOR	0.8 0.1 0.0 0.5  		//R G B Alpha in range 0-1 (alpha=transparency)
BARRELGEOM	New1Syringe10ccBarrel.STL  	//name of the stl file describing the syringe barrel geometry
BARRELOFFSET	0 0 0				//dx dy dz graphics offset of syringe barrel
BARRELCOLOR	0.2 0.2 0.2 0.5			//R G B Alpha in range 0-1 (alpha=transparency)
BARRELDIAM	15.8				//[mm] internal diameter of syringe barrel
TIPGEOM		New1SyringeAmberNeedle.STL	//name of the stl file describing the nozzle/needle geometry
TIPOFFSET	0 0 0				//dx dy dz graphics offset of syringe nozzle/needle
TIPCOLOR	1 0 0 0.9			//R G B Alpha in range 0-1 (alpha=transparency)
TIPPOINT	99,-318,-60			//x,y,z tip location in geometry file coordinates
PLUNGERGEOM	New1Syringe10ccPlunger.STL	//name of the stl file describing the syringe plunger geometry
PLUNGEROFFSET	0 0 80				//dx dy dz graphics offset of syringe plunger
PLUNGERCOLOR	0 1 0 0.5			//R G B Alpha in range 0-1 (alpha=transparency)
DIRECTION	0 0 -1				//orientation of graphical display of motion of plunger
VOLUME		10				//[mL] volume of syringe barrel
MOTOR		3 -1 0.000396875		//amplifier axis #, axis direction(+/-1), mm per (1/8) step
RANGE		0 83 0 				//[mm] start end home of graphical range of motion for plunger
BACKFILL	2 				//[mm] maximum depth allowed for backfill deposition (NOT YET IMPLEMENTED)
LIMITSWITCH	0 0 				//+direction -direction (0 -> limit switch not connected; 1 if it is)
INCREMENT	0.1 				//[mm] default distance to move plunger in manual jog