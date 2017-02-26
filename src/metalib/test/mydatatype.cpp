
#include "MyDataType.h"
#include <qdebug.h>

//CIM_DATATYPE_IMPL(MyDataType)

MetaObject MyDataType::staticMetaObject = {
	Q_NULLPTR, QByteArray("\0"), QByteArray("\0")
	, QByteArray("\0"), cim_static_metacall, Q_NULLPTR, Q_NULLPTR,
    true//CIM_META_REGISTER(MyDataType)
};

const MetaObject *MyDataType::metaObject() const
{
	return &staticMetaObject;
}

void *MyDataType::cim_metacast(const char *_clname)
{
	if (!_clname) return Q_NULLPTR;
	if (!strcmp(_clname, metaObject()->className()))
		return static_cast<void*>(const_cast<MyDataType*>(this));
	return MyDataType::cim_metacast(_clname);
}

int MyDataType::cim_metacall(MetaObject::Call _c, int _id, void **_a)
{
	if (strcmp(metaObject()->className(), "MyDataType") == 0)
		return  _id = -1;

	_id = MyDataType::cim_metacall(_c, _id, _a);
	if (_id < 0)
		return _id;

	return _id;
}

void MyDataType::cim_static_metacall(void *_o, MetaObject::Call _c, int _id, void **_a)
{
	if (_c == MetaObject::CreateInstance) {
		switch (_id) {
		case 0: 
		{
			QVariant _r;
			InstanceType &type = *reinterpret_cast<InstanceType*>(_a[1]);
			QByteArray &param = (*reinterpret_cast<QByteArray*>(_a[2]));
			if (HeapInstance == type)
				_r.setValue(new MyDataType(param));
			else if (StackInstance == type)
				_r.setValue(MyDataType(param));
			else if (PointerInstance == type)
				_r.setValue(reinterpret_cast<MyDataType*>(param.toULongLong()));

			*reinterpret_cast<QVariant*>(_a[0]) = _r;
		} break;
		default: break;
		}
	}
	else if (_c == MetaObject::RetrieveObjData) {
		if (_a[0])
		{
			QVariant &var = (*reinterpret_cast<QVariant*>(_a[1]));
			QVariant _r;
			if (var.canConvert<MyDataType*>())
				_r.setValue(var.value<MyDataType*>()->values());
			else if (var.canConvert<MyDataType>())
				_r.setValue(var.value<MyDataType>().values());

			*reinterpret_cast<QVariant*>(_a[0]) = _r;
		}
	}

	Q_UNUSED(_o);
}

MyDataType::MyDataType(const QByteArray &values)
{
	setValues(values);
}

void MyDataType::setValues(const QByteArray &var)
{
	if (var.count('\0') != 6)
		return;

	int n;
	setMultiplier(UnitMul(atoi(constData_Bi(var, n = 0))));
	setValue(atof(constData_Bi(var, ++n)));
	value1 = atof(constData_Bi(var, ++n));
	value2 = atoi(constData_Bi(var, ++n));
	value3 = bool(atoi(constData_Bi(var, ++n)));
	value4 = QByteArray(constData_Bi(var, ++n));
}

QByteArray MyDataType::values() const
{
	QByteArray v;

    v ooon(multiplier())
        ooon(value())
        ooon(value1 < EPSILON ? 0.0f : value1)
        ooon(value2 < EPSILON ? 0.0f : value2)
        ooon(value3)
        ooo(value4);

	return v;
}
