#include "DotPhisics.h"

void DotPhysics::Initialize(DirectX::XMFLOAT3 sceneBorder, bool bounceDicrimentState, bool airResistanceState)
{
    this->sceneBorder = sceneBorder;
    this->bounceDicrimentState = bounceDicrimentState;
    this->airResistanceState = airResistanceState;
}

void DotPhysics::SetDeltaTime(float dt)
{
    this->dt = dt;
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
    if (point.pointPos.x - point.radius < 0) return botX;
    if (point.pointPos.y + point.radius > sceneBorder.y) return topY;
    if (point.pointPos.y - point.radius < 0) return botY;
    if (point.pointPos.z + point.radius > sceneBorder.z) return topZ;
    if (point.pointPos.z - point.radius < 0) return botZ;
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
                point.velosity.x + point.acceleration.x * airResistanceDicrement * this->dt,
                point.velosity.y + point.acceleration.y * airResistanceDicrement * this->dt,
                point.velosity.z + point.acceleration.z * airResistanceDicrement * this->dt
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
    
    /*DirectX::XMFLOAT3 avarageVelosity{
        (point1.velosity.x + point2.velosity.x) / 2,
        (point1.velosity.y + point2.velosity.y) / 2,
        (point1.velosity.z + point2.velosity.z) / 2
    };*/

    if (bounceDicrimentState) {
        return std::pair<Point, Point>(
            Point{
               point1.radius,
               point1.pointPos,
               DirectX::XMFLOAT3 {
                    point2.velosity.x,
                    point2.velosity.y,
                    point2.velosity.z,
                },
               point1.acceleration
            },
            Point{
               point2.radius,
               point2.pointPos,
               DirectX::XMFLOAT3{
                    point1.velosity.x,
                    point1.velosity.y,
                    point1.velosity.z,
               },
               point2.acceleration
            });
    }
    else {
        return std::pair<Point, Point>(
            Point{
               point1.radius,
               point1.pointPos,
               DirectX::XMFLOAT3 {
                    point2.velosity.x,
                    point2.velosity.y,
                    point2.velosity.z,
                },
               point1.acceleration
            },
            Point{
               point2.radius,
               point2.pointPos,
               DirectX::XMFLOAT3{
                    point1.velosity.x,
                    point1.velosity.y,
                    point1.velosity.z,
               },
               point2.acceleration
            });
    }
    
}

Point DotPhysics::BounceFromBorder(const Point& point, int borderSide)
{
    if (borderSide == topX || borderSide == botX) {
        return Point(
            point.radius,
            point.pointPos,
            DirectX::XMFLOAT3{
                -1.0f * point.velosity.x,
                point.velosity.y,
                point.velosity.z,
            },
            point.acceleration
        );
    }
    else if (borderSide == topY || borderSide == botY) {
        return Point(
            point.radius,
            point.pointPos,
            DirectX::XMFLOAT3{
                point.velosity.x,
                -1.0f * point.velosity.y,
                point.velosity.z,
        },
            point.acceleration
        );
    }
    else if (borderSide == topZ || borderSide == botZ) {
        return Point(
            point.radius,
            point.pointPos,
            DirectX::XMFLOAT3{
                point.velosity.x,
                point.velosity.y,
                -1.0f * point.velosity.z,
        },
            point.acceleration
        );
    }
    else {
        return Point(
            point.radius,
            point.pointPos,
            point.velosity,
            point.acceleration
        );
    }
    
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


