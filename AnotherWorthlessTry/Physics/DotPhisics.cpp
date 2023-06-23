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

void DotPhysics::SetSceneBorder(const DirectX::XMFLOAT3& sceneBorder_)
{
    this->sceneBorder = sceneBorder_;
}

//std::vector<Point> DotPhysics::CalculateScene(std::vector<Point> points){}

bool DotPhysics::ObjectCollision(const Point& point1, const Point& point2)
{
    return point1.radius > Distance(DistanceVector(point1.pointPos, point2.pointPos));
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

Point DotPhysics::Move(const Point& point, float num)
{
    return Point{ 
        point.radius,
        DirectX::XMFLOAT3{ 
            point.pointPos.x + num * point.velosity.x * this->dt,
            point.pointPos.y + num * point.velosity.y * this->dt,
            point.pointPos.z + num * point.velosity.z * this->dt
        },
        point.velosity,
        point.acceleration,
        point.moveState,
        point.createGravityState

    };
}

Point DotPhysics::Accelerate(const Point& point, float num)
{
    return gAccelerationState ? GAccelerate(PAccelerate(point, num), num) : PAccelerate(point, num);
}

Point DotPhysics::GAccelerate(const Point& point, float num) {
    return Point{
           point.radius,
           point.pointPos,
           DirectX::XMFLOAT3{
               point.velosity.x + num * gAcceleration.x * this->dt,
               point.velosity.y + num * gAcceleration.y * this->dt,
               point.velosity.z + num * gAcceleration.z * this->dt
           },
           point.acceleration,
           point.moveState,
           point.createGravityState
    };
}

Point DotPhysics::PAccelerate(const Point& point, float num)
{
    if (airResistanceState) {
        return Point{
            point.radius,
            point.pointPos,
            DirectX::XMFLOAT3{
                point.velosity.x + num * point.acceleration.x * airResistanceDicrement * point.radius / 100.0f * this->dt,
                point.velosity.y + num * point.acceleration.y * airResistanceDicrement * point.radius / 100.0f * this->dt,
                point.velosity.z + num * point.acceleration.z * airResistanceDicrement * point.radius / 100.0f * this->dt
            },
            point.acceleration,
            point.moveState,
            point.createGravityState
        };
    }
    else {
        return Point{
            point.radius,
            point.pointPos,
            DirectX::XMFLOAT3{
               point.velosity.x + num * point.acceleration.x * this->dt,
               point.velosity.y + num * point.acceleration.y * this->dt,
               point.velosity.z + num * point.acceleration.z * this->dt
            },
            point.acceleration,
            point.moveState,
            point.createGravityState
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
            point1.acceleration,
            point1.moveState,
            point1.createGravityState
        },
        Point{
            point2.radius,
            point2.pointPos,
            v1,
            point2.acceleration,
            point2.moveState,
            point2.createGravityState
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
            point.acceleration,
            point.moveState,
            point.createGravityState
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
            point.acceleration,
            point.moveState,
            point.createGravityState
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
            point.acceleration,
            point.moveState,
            point.createGravityState
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
            point.acceleration,
            point.moveState,
            point.createGravityState
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
            point.acceleration,
            point.moveState,
            point.createGravityState
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
            point.acceleration,
            point.moveState,
            point.createGravityState
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
            point.acceleration,
            point.moveState,
            point.createGravityState
        );
    }
}

DirectX::XMFLOAT3  DotPhysics::DistanceVector(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2) {
    return DirectX::XMFLOAT3{
        pos2.x - pos1.x,
        pos2.y - pos1.y,
        pos2.z - pos1.z
    };
}

float DotPhysics::Distance(const DirectX::XMFLOAT3& distanceVector)
{
    return powf(
        powf(distanceVector.x, 2.0f) +
        powf(distanceVector.y, 2.0f) +
        powf(distanceVector.z, 2.0f), 0.5f
    );
}


DirectX::XMFLOAT3 DotPhysics::BounceDicrement(DirectX::XMFLOAT3 velosity)
{
    float vX = velosity.x - velosity.x * bounceDicrement / 100.0f;
    float vY = velosity.y - velosity.y * bounceDicrement / 100.0f;
    float vZ = velosity.z - velosity.z * bounceDicrement / 100.0f;
    
    return DirectX::XMFLOAT3{vX, vY, vZ};
}

DirectX::XMFLOAT3 DotPhysics::CalcAcceleration(const Point& point1, const Point& point2, float multiplier)
{
    DirectX::XMFLOAT3 acc = DistanceVector(point1.pointPos, point2.pointPos);
    float accPower = Distance(acc);
    

    return DirectX::XMFLOAT3 {
        acc.x* point1.radius* multiplier / powf(accPower, 3.0f),
        acc.y* point1.radius* multiplier / powf(accPower, 3.0f),
        acc.z* point1.radius* multiplier / powf(accPower, 3.0f)
    };
}

void DotPhysics::SetStates(bool gAccelerationState_, bool dicrimentState_, bool airResistanceState_)
{
    this->gAccelerationState = gAccelerationState_;
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
