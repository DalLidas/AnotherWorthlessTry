#include "DotPhisics.h"

void DotPhysics::Initialize(DirectX::XMFLOAT3 sceneBorder, bool bounceDicrimentState, bool airResistanceState)
{
    this->sceneBorder = sceneBorder;
    this->bounceDicrimentState = false;
    this->airResistanceState = false;
}

void DotPhysics::SetDeltaTime(float dt)
{
    this->dt = dt;
}

void DotPhysics::CalculateScene(std::vector<Point> points)
{
    Point bufPoint{};

    for (auto point1 = points.begin(); point1 != points.end(); ++point1) {
        if (BorderCollision(*point1)) {
            BounceFromBorder(*point1);
        }
        else {
            for (auto point2 = points.begin(); point2 != points.end(); ++point2) {
                if (point1 != point2) {
                    bufPoint = Move(*point1);
                    if (ObjectCollision(*point1, *point2)) {
                        BounceFromObject(*point1, *point2);
                    }
                    else {
                       
                    }

                }
            }
        }
        

    }
}

bool DotPhysics::ObjectCollision(const Point& point1, const Point& point2)
{
    return false;
}

bool DotPhysics::BorderCollision(const Point& point)
{
    return false;
}

Point DotPhysics::Move(const Point& point)
{
    return Point{ 
        point.radius,
        DirectX::XMFLOAT3{ 
            point.pointPos.x + point.velosity.x * this->dt,
            point.pointPos.y + point.velosity.y * this->dt,
            point.pointPos.z + point.velosity.z * this->dt
        },
        point.velosity,
        point.acceleration 
    };
}

void DotPhysics::Accelerate(Point& point)
{
    if (airResistanceState) {
        point.velosity = DirectX::XMFLOAT3{
            point.velosity.x + point.velosity.x * airResistanceDicrement * this->dt,
            point.velosity.y + point.velosity.y * airResistanceDicrement * this->dt,
            point.velosity.z + point.velosity.z * airResistanceDicrement * this->dt
        };
    }
    else {
        point.velosity = DirectX::XMFLOAT3{ 
            point.velosity.x + point.velosity.x * this->dt,
            point.velosity.y + point.velosity.y * this->dt,
            point.velosity.z + point.velosity.z * this->dt 
        };
    }
        
}

void DotPhysics::BounceFromObject(Point& point1, Point& point2)
{

}

void DotPhysics::BounceFromBorder(Point& point)
{

}


void DotPhysics::SetStates(bool dicrimentState, bool airResistanceState)
{
    this->bounceDicrimentState = dicrimentState;
    this->airResistanceState = airResistanceState;
}

void DotPhysics::SetBounceDicrement(float newBounceDicrement)
{
    this->bounceDicrement = newBounceDicrement;
}

void DotPhysics::SetAirResistanceDicrement(float newAirResistanceDicrement)
{
    this->airResistanceDicrement = newAirResistanceDicrement;
}


