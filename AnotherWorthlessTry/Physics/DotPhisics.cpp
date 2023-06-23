#include "DotPhisics.h"

void DotPhysics::Initialize(DirectX::XMFLOAT3 sceneBorder_, bool bounceDicrimentState_, bool airResistanceState_)
{
    this->sceneBorder = sceneBorder_;
    this->bounceDicrimentState = bounceDicrimentState_;
    this->airResistanceState = airResistanceState_;
}

void DotPhysics::SetDeltaTime(float dt_)
{
    this->dt = dt_;
}

//std::vector<Point> DotPhysics::CalculateScene(std::vector<Point> points){}

bool DotPhysics::ObjectCollision(const Point& point1, const Point& point2)
{
    return point1.radius > powf( 
        powf(point1.pointPos.x - point2.pointPos.x, 2.0f) +
        powf(point1.pointPos.y - point2.pointPos.y, 2.0f) +
        powf(point1.pointPos.z - point2.pointPos.z, 2.0f), 0.5f);
}

int DotPhysics::BorderCollision(const Point& point)
{
    if (point.pointPos.x + point.radius > sceneBorder.x) return topX;
    else if (point.pointPos.x - point.radius < 0) return botX;
    else if (point.pointPos.y + point.radius > sceneBorder.y) return topY;
    else if (point.pointPos.y - point.radius < 0) return botY;
    else if (point.pointPos.z + point.radius > sceneBorder.z) return topZ;
    else if (point.pointPos.z - point.radius < 0) return botZ;
    else return 0;
}

Point DotPhysics::Move(const Point& point, int num)
{
    return Point{ 
        point.radius,
        DirectX::XMFLOAT3{ 
            point.pointPos.x + num * point.velosity.x * this->dt,
            point.pointPos.y + num * point.velosity.y * this->dt,
            point.pointPos.z + num * point.velosity.z * this->dt
        },
        point.velosity,
        point.acceleration 
    };
}

Point DotPhysics::Accelerate(const Point& point)
{
    if (airResistanceState) {
        return Point{
            point.radius,
            point.pointPos,
            DirectX::XMFLOAT3{
                point.velosity.x + point.acceleration.x * airResistanceDicrement * point.radius / 100.0f * this->dt,
                point.velosity.y + point.acceleration.y * airResistanceDicrement * point.radius / 100.0f * this->dt,
                point.velosity.z + point.acceleration.z * airResistanceDicrement * point.radius / 100.0f * this->dt
            },
            point.acceleration
        };
    }
    else {
        return Point{
           point.radius,
           point.pointPos,
           DirectX::XMFLOAT3{
               point.velosity.x + point.acceleration.x * this->dt,
               point.velosity.y + point.acceleration.y * this->dt,
               point.velosity.z + point.acceleration.z * this->dt
           },
           point.acceleration
        };
    }
        
}

std::pair<Point, Point> DotPhysics::BounceFromObject(const Point& point1, const Point& point2)
{
    DirectX::XMFLOAT3 v1 = point1.velosity;
    if (bounceDicrimentState)  v1 = BounceDicrement(point1.velosity);

    DirectX::XMFLOAT3 v2 = point2.velosity;
    if (bounceDicrimentState) v2 = BounceDicrement(point2.velosity);


    return std::pair<Point, Point>(
        Point{
            point1.radius,
            DirectX::XMFLOAT3 {
                point1.pointPos.x + (point1.pointPos.x - point2.pointPos.x) / (point1.radius + point2.radius),
                point1.pointPos.y + (point1.pointPos.y - point2.pointPos.y) / (point1.radius + point2.radius),
                point1.pointPos.z + (point1.pointPos.z - point2.pointPos.z) / (point1.radius + point2.radius),
            },
            v2,
            point1.acceleration
        },
        Point{
            point2.radius,
            point2.pointPos,
            v1,
            point2.acceleration
        });
    
}

Point DotPhysics::BounceFromBorder(const Point& point, int borderSide)
{
    DirectX::XMFLOAT3 v = point.velosity;
    if(bounceDicrimentState) v =  BounceDicrement(point.velosity);


    float bonusCord = point.radius + 0.1f;
    switch (borderSide)
    {
    case(topX):
        return Point(
            point.radius,
            DirectX::XMFLOAT3{
                sceneBorder.x - bonusCord,
                point.pointPos.y,
                point.pointPos.z
            },
            DirectX::XMFLOAT3{
                -1.0f * v.x,
                v.y,
                v.z,
            },
            point.acceleration
        );

    case(botX):
        return Point(
            point.radius,
            DirectX::XMFLOAT3{
                bonusCord,
                point.pointPos.y,
                point.pointPos.z
            },
            DirectX::XMFLOAT3{
                -1.0f * v.x,
                v.y,
                v.z,
            },
            point.acceleration
        );
        
    case(topY):
        return Point(
            point.radius,
            DirectX::XMFLOAT3{
                point.pointPos.x,
                sceneBorder.y - bonusCord,
                point.pointPos.z
            },
            DirectX::XMFLOAT3{
                v.x,
                -1.0f * v.y,
                v.z,
            },
            point.acceleration
        );

    case(botY):
        return Point(
            point.radius,
            DirectX::XMFLOAT3{
                point.pointPos.x,
                bonusCord,
                point.pointPos.z
            },
            DirectX::XMFLOAT3{
                v.x,
                -1.0f * v.y,
                v.z,
        },
            point.acceleration
        );

    case(topZ):
        return Point(
            point.radius,
            DirectX::XMFLOAT3{
                point.pointPos.x,
                point.pointPos.y,
                sceneBorder.z - bonusCord
            },
            DirectX::XMFLOAT3{
                v.x,
                v.y,
                -1.0f * v.z,
            },
            point.acceleration
        );

    case(botZ):
        return Point(
            point.radius,
            DirectX::XMFLOAT3{
                point.pointPos.x,
                point.pointPos.y,
                bonusCord
            },
            DirectX::XMFLOAT3{
                v.x,
                v.y,
                -1.0f * v.z,
            },
            point.acceleration
        );

    default:
        return Point(
            point.radius,
            point.pointPos,
            DirectX::XMFLOAT3{
                v.x,
                v.y,
                v.z,
            },
            point.acceleration
        );
    }
}

DirectX::XMFLOAT3 DotPhysics::BounceDicrement(DirectX::XMFLOAT3 velosity)
{
    float vX = velosity.x - velosity.x * bounceDicrement / 100.0f;
    float vY = velosity.y - velosity.y * bounceDicrement / 100.0f;
    float vZ = velosity.z - velosity.z * bounceDicrement / 100.0f;
    
    return DirectX::XMFLOAT3{vX, vY, vZ};
}

void DotPhysics::SetStates(bool dicrimentState_, bool airResistanceState_)
{
    this->bounceDicrimentState = dicrimentState_;
    this->airResistanceState = airResistanceState_;
}

void DotPhysics::SetBounceDicrement(float newBounceDicrement_)
{
    this->bounceDicrement = newBounceDicrement_;
}

void DotPhysics::SetAirResistanceDicrement(float newAirResistanceDicrement_)
{
    this->airResistanceDicrement = newAirResistanceDicrement_;
}
