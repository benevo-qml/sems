///////////////////////////////////////////////////////////
//  Voltage.h
//  Implementation of the Class Voltage
//  Created on:      13-Nov-2015 13:16:24
//  Original author: benev
///////////////////////////////////////////////////////////

#if !defined(EA_019705E2_8178_473e_884C_8AA7D084C044__INCLUDED_)
#define EA_019705E2_8178_473e_884C_8AA7D084C044__INCLUDED_

#include "cim_global.h"
#include "core/o3objectdefs.h"

class TestObject;

/**
 * Electrical voltage.
 */
class MyDataType
{
	CIM_DATATYPE(MyDataType)

public:
	enum UnitMul
	{
		/**
		* Kilo 10**3.
		*/
		KILO,
		/**
		* Mega 10**6.
		*/
		MEGA,
		/**
		* Giga 10**9.
		*/
		GIGA
	};
	//CIM_ENUM(UnitMul)

	enum UnitSyml
	{
		/**
		* Apparent power in volt ampere.
		*/
		iVA,
		/**
		* Active power in watt.
		*/
		iW,
		/**
		* Reactive power in volt ampere reactive.
		*/
		iVAr,
		/**
		* Apparent energy in volt ampere hours.
		*/
		iVAh,
		/**
		* Real energy in what hours.
		*/
		iWh,
		/**
		* Reactive energy in volt ampere reactive hours.
		*/
		iVArh,
		/**
		* Voltage in volt.
		*/
		iV,
		/**
		* Resistance in ohm.
		*/
		iohm,
		/**
		* Current in ampere.
		*/
		iA,
	};
	//CIM_ENUM(UnitSyml)

	MyDataType() {}

	CIM_PROPERTY(public, UnitMul, m_multiplier, READ, multiplier, WRITTE, setMultiplier)
	CIM_PROPERTY(public, UnitSyml, m_unit, READ, unit, WRITTE, setUnit)
	CIM_PROPERTY(public, float, m_value, READ, value, WRITTE, setValue)

private:
	float value1;
	int value2;
	bool value3;
	QByteArray value4;

};
typedef MyDataType* MyDataTypePtr;
Q_DECLARE_METATYPE(MyDataType)
Q_DECLARE_METATYPE(MyDataType*)

#endif // !defined(EA_019705E2_8178_473e_884C_8AA7D084C044__INCLUDED_)
