#include "Scene.h"

Scene::Scene()
{
	Point p;
	p.radius = 1;
	DirectX::XMFLOAT3 pos {1.0f, 1.0f, 1.0f};
	p.pointPos = pos;
	p.velosity = this->DEFAULT_VELOSITY;
	p.acceleration = this->DEFAULT_ACCELERATION;

	this->points.push_back(p);

	pos = { 3.1f, 3.1f, 3.0f };
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

void Scene::createPoint(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velosity, DirectX::XMFLOAT3 acceleration)
{
	Point p;
	p.pointPos = pos;
	p.velosity = velosity;
	p.acceleration = acceleration;

	this->points.push_back(p);
}
