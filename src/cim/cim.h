#ifndef CIM_H
#define CIM_H

#include <QObject>

#include "cim_global.h"

class CIM_EXPORT Cim : public QObject
{
    Q_OBJECT
    Q_ENUMS(CIM_TYPE)
    Q_ENUMS(CIM_GEO_TYPE)
    Q_ENUMS(CIM_PSR_TYPE)

public:
    enum CIM_TYPE{
        TYPE_GEO,
        TYPE_PSR
    };

    enum CIM_GEO_TYPE{
        GeoRegion,					// 地理区域
        SubGeoRegion					// 控制区域
    };

    enum CIM_PSR_TYPE{ //定义资源类型
        Substation,					// 变电站
        Plant,						// 发电厂
        Line,						// 线路
        VoltageLevel,				// 电压等级
        Bay,						// 间隔
        Terminal,					// 电气端口

        ACLineSegment,				// 交流线段
        DCLineSegment,				// 直流线段
        LoadBreakSwitch,			// 负荷开关
        Recloser,					// 重合闸
        Breaker,					// 断路器（开关）
        Sectionaliser,				//
        Jumper,						// 发电厂
        Fuse,						// 发电厂
        GroundDisconnector,			// 接地刀
        Disconnector,				// 隔离开关（刀闸）
        TransformerWinding,         // 绕组
        PowerTransformer,           // 变压器

        BusbarSection,				// 母线
        Junction,					// 0阻抗电气连接
        AsynchronousMachine,		// 异步机
        SynchronousMachine,			// 同步机

        PSRTYPE_CNT
    };


    explicit Cim(QObject *parent = 0);
    ~Cim();
};

#endif // CIM_H
