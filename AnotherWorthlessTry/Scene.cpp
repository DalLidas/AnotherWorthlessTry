#include "Scene.h"

Scene::Scene()
{
	Point p;
	p.radius = 1;
	DirectX::XMFLOAT3 pos {0.0f, 0.0f, 0.0f};
	p.pointPos = pos;
	p.velosity = this->DEFAULT_VELOSITY;
	p.acceleration = this->DEFAULT_ACCELERATION;

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

void Scene::SetPoints(std::vector<Point> newPoints)
{
	this->points.clear();
	this->points = newPoints;
}

void Scene::SetPoint(Point newPoint, size_t index)
{
	this->points.at(index) = newPoint;
}

void Scene::SetSceneBorder(DirectX::XMFLOAT3 newSceneBorder)
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

void Scene::createPoint(DirectX::XMFLOAT3 pos)
{
	Point p;
	p.pointPos = pos;
	p.velosity = this->ZERO_VELOSITY;
	p.acceleration = this->ZERO_ACCELERATION;

	this->points.push_back(p);
}

void Scene::createPoint(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velosity, DirectX::XMFLOAT3 acceleration)
{
	Point p;
	p.pointPos = pos;
	p.velosity = velosity;
	p.acceleration = acceleration;

	this->points.push_back(p);
}
