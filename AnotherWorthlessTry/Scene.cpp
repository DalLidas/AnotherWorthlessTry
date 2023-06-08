#include "Scene.h"

Scene::Scene()
{
	Point p;
	p.radius = 2;
	p.pointPos = DirectX::XMFLOAT3{ 4.0f, 3.0f, 4.0f };
	p.velosity = DirectX::XMFLOAT3{ 0.0032f, -0.0016f, 0.0016f };
	p.acceleration = DirectX::XMFLOAT3{ 0.000000f, 0.0f, 0.0f };

	this->points.push_back(p);

	
	p.radius = 2;
	p.pointPos = DirectX::XMFLOAT3{ 4.0f, 10.0f, 12.0f };
	p.velosity = DirectX::XMFLOAT3{ -0.0004f, 0.0016f, 0.0005f };
	p.acceleration = DirectX::XMFLOAT3{ 0.000000f, 0.0f, 0.0 };

	this->points.push_back(p);

	p.radius = 2;
	p.pointPos = DirectX::XMFLOAT3{ 5.0f, 5.0f, 10.0f };
	p.velosity = DirectX::XMFLOAT3{ 0.0004f, 0.0004f, 0.0016f };
	p.acceleration = DirectX::XMFLOAT3{ 0.000000f, 0.0f, 0.0f };

	this->points.push_back(p);
	
}

std::vector<Point> Scene::GetPoints() const
{
	return this->points;
}

DirectX::XMFLOAT3 Scene::GetSceneBorder() const
{
	return this->sceneBorder;
}

void Scene::SetPoints(const std::vector<Point>& newPoints)
{
	this->points.clear();
	this->points = newPoints;
}

void Scene::SetPoint(const Point& newPoint, size_t index)
{
	this->points.at(index) = newPoint;
}

void Scene::SetSceneBorder(const DirectX::XMFLOAT3& newSceneBorder)
{
	this->sceneBorder = newSceneBorder;
}

void Scene::createPoint()
{
	Point p;
	p.radius = 1;
	DirectX::XMFLOAT3 pos {};
	p.pointPos = this->pointOfGod;
	p.velosity = this->DEFAULT_VELOSITY;
	p.acceleration = this->DEFAULT_ACCELERATION;

	this->points.push_back(p);
}

void Scene::createPoint(const DirectX::XMFLOAT3& pos)
{
	Point p;
	p.pointPos = pos;
	p.velosity = this->ZERO_VELOSITY;
	p.acceleration = this->ZERO_ACCELERATION;

	this->points.push_back(p);
}

void Scene::createPoint(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& velosity, const DirectX::XMFLOAT3& acceleration)
{
	Point p;
	p.pointPos = pos;
	p.velosity = velosity;
	p.acceleration = acceleration;

	this->points.push_back(p);
}
