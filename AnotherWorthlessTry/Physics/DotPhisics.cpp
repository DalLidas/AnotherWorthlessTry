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

//std::vector<Point> DotPhysics::CalculateScene(std::vector<Point> points)
//{
//    Point bufPoint{};
//   
//    for (size_t i = 0; i < points.size(); ++i) {
//        bufPoint = Move(points.at(i));
//
//        if (BorderCollision(bufPoint)) {
//            BounceFromBorder(points.at(i));
//        }
//        else {
//            for (size_t j = 0; j < points.size(); ++j) {
//                if (i != j) {
//                    if (ObjectCollision(bufPoint, points.at(j))) {
//                        BounceFromObject(points.at(i), points.at(j));
//                    }
//                   
//                }
//            }
//        }
//        
//        points.at(i) = bufPoint;
//        points.at(i) = Accelerate(bufPoint);
//    }
//
//    return points;
//}

bool DotPhysics::ObjectCollision(const Point& point1, const Point& point2)
{
    return point1.radius > powf( 
        powf(point1.pointPos.x - point2.pointPos.x, 2.0f) +
        powf(point1.pointPos.y - point2.pointPos.y, 2.0f) +
        powf(point1.pointPos.z - point2.pointPos.z, 2.0f), 0.5f);

}

bool DotPhysics::BorderCollision(const Point& point)
{
    return false;
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

    DirectX::XMFLOAT3 p1Velosity {
        point2.velosity.x,
        point2.velosity.y,
        point2.velosity.z,
    };

    DirectX::XMFLOAT3 p2Velosity {
        point1.velosity.x,
        point1.velosity.y,
        point1.velosity.z,
    };

    if (bounceDicrimentState) {
        return std::pair<Point, Point>(
            Point{
               point1.radius,
               point1.pointPos,
               p1Velosity,
               point1.acceleration
            },
            Point{
               point2.radius,
               point2.pointPos,
               p2Velosity,
               point2.acceleration
            });
    }
    else {
        return std::pair<Point, Point>(
            Point{
               point1.radius,
               point1.pointPos,
               p1Velosity,
               point1.acceleration
            },
            Point{
               point2.radius,
               point2.pointPos,
               p2Velosity,
               point2.acceleration
            });
    }
    
}

Point DotPhysics::BounceFromBorder(const Point& point)
{
    return Point();
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


