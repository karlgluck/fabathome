#ifndef NMCCOM_H
#define NMCCOM_H

typedef unsigned char byte;

typedef struct _NMCMOD {
	byte	modtype;		//module type
    byte	modver;			//module version number
	byte	statusitems;	//definition of items to be returned
	byte	stat;  			//status byte
    byte	groupaddr;		//current group address
    BOOL	groupleader;	//TRUE if group leader
    void *	p;				//pointer to specific module's data structure
    } NMCMOD;

#define MAXSIOERROR 2

//Define PIC baud rate divisors
#define	PB19200		63
#define	PB57600		20
#define	PB115200	10
#define	PB230400	5

//Module type definitions:
#define	SERVOMODTYPE	0
#define	ADCMODTYPE		1
#define	IOMODTYPE		2
#define	STEPMODTYPE		3
//The following must be created for each new module type:
//		data structure XXXMOD
//		Initializer function NewXXXMod
//		Status reading function GetXXXStat
//		NMCInit and SendNmcCmd must be modified to include calls
//			to the two functions above

#define CKSUM_ERROR		0x02	//Checksum error bit in status byte

//--------------------- ADC Module specific stuff --------------------------
typedef struct _ADCMOD {
    //******  Move all this data to the NMCMOD structure *******
	short int ad0;	//definition of items to be returned
	short int ad1;
	short int ad2;
	short int ad3;
	short int ad4;
	short int ad5;
    } ADCMOD;


#define MAXNUMMOD	33

//Load functions from DLL.
static int (*NmcInit)(char*,unsigned int) = reinterpret_cast<int(*)(char*,unsigned int)>(DLL::loadFunction("NMCLIB04.dll","NmcInit"));  
static void (*NmcShutdown)(void) = reinterpret_cast<void(*)(void)>(DLL::loadFunction("NMCLIB04.dll","NmcShutdown"));  
static BOOL (*NmcSetGroupAddr)(byte,byte,BOOL) = reinterpret_cast<BOOL(*)(byte,byte,BOOL)>(DLL::loadFunction("NMCLIB04.dll","NmcSetGroupAddr"));
static BOOL (*NmcDefineStatus)(byte,byte) = reinterpret_cast<BOOL(*)(byte,byte)>(DLL::loadFunction("NMCLIB04.dll","NmcDefineStatus"));
static BOOL (*NmcNoOp)(byte) = reinterpret_cast<BOOL(*)(byte)>(DLL::loadFunction("NMCLIB04.dll","NmcNoOp"));
static BOOL (*NmcHardReset)(byte) = reinterpret_cast<BOOL(*)(byte)>(DLL::loadFunction("NMCLIB04.dll","NmcHardReset"));
static byte (*NmcGetStat)(byte) = reinterpret_cast<byte(*)(byte)>(DLL::loadFunction("NMCLIB04.dll","NmcGetStat"));
static byte (*NmcGetModType)(byte) = reinterpret_cast<byte(*)(byte)>(DLL::loadFunction("NMCLIB04.dll","NmcGetModType"));
static void (*ErrorPrinting)(int) = reinterpret_cast<void(*)(int)>(DLL::loadFunction("NMCLIB04.dll","ErrorPrinting"));


#endif //ndef NMCCOM_H
