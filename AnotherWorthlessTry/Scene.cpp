#include "Scene.h"

Scene::Scene()
{
	Point p;
	//p.radius = 1;
	//p.pointPos = DirectX::XMFLOAT3{ 18.9f, 1.1f, 1.1f };
	//p.velosity = DirectX::XMFLOAT3{ 0.0032f, 0.0f, 0.0f };
	//p.acceleration = DirectX::XMFLOAT3{ 0.0f, -0.001f, 0.0f };

	//this->points.push_back(p);

	p.radius = 4;
	p.pointPos = DirectX::XMFLOAT3{ 15.9f, 1.1f, 1.1f };
	p.velosity = DirectX::XMFLOAT3{ -1.0f, 0.0f, 0.0f };
	p.acceleration = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f };

	this->points.push_back(p);

	/*
	p.radius = 1;
	p.pointPos = DirectX::XMFLOAT3{ 4.1f, 18.9f, 4.1f };
	p.velosity = DirectX::XMFLOAT3{ 0.0f, 0.0032f, 0.0f };
	p.acceleration = DirectX::XMFLOAT3{ 0.0f, -0.001f, 0.0 };

	this->points.push_back(p);

	p.radius = 1;
	p.pointPos = DirectX::XMFLOAT3{ 8.1f, 8.1f, 18.9f };
	p.velosity = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0032f };
	p.acceleration = DirectX::XMFLOAT3{ 0.0f, -0.001f, 0.0f };

	this->points.push_back(p);*/
}

std::vector<Point> Scene::GetPoints() const
{
	return this->points;
}

DirectX::XMFLOAT3 Scene::GetPointOfGod() const
{
	return this->pointOfGod;
}

DirectX::XMFLOAT3 Scene::GetSceneBorder() const
{
	return this->sceneBorder;
}

bool Scene::IsMoveable(size_t index)
{
	return this->points.at(index).moveState;
}

bool Scene::IsCreateGravity(size_t index)
{
	return this->points.at(index).createGravityState;
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

void Scene::SetPointOfGod(const DirectX::XMFLOAT3& newPointOfGod)
{
	this->pointOfGod = newPointOfGod;
}

void Scene::SetSceneBorder(const DirectX::XMFLOAT3& newSceneBorder)
{
	this->sceneBorder = newSceneBorder;
}

void Scene::CreatePoint()
{
	Point p;
	p.radius = 1;
	DirectX::XMFLOAT3 pos {};
	p.pointPos = this->pointOfGod;
	p.velosity = this->DEFAULT_VELOSITY;
	p.acceleration = this->DEFAULT_ACCELERATION;

	this->points.push_back(p);
}

void Scene::CreatePoint(const DirectX::XMFLOAT3& pos)
{
	Point p;
	p.pointPos = pos;
	p.velosity = this->ZERO_VELOSITY;
	p.acceleration = this->DEFAULT_ACCELERATION;

	this->points.push_back(p);
}

void Scene::CreatePoint(float radius, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& velosity, const DirectX::XMFLOAT3& acceleration, bool moveState, bool createGravityState)
{
	Point p;
	p.radius = radius;
	p.pointPos = pos;
	p.velosity = velosity;
	p.acceleration = acceleration;

	p.moveState = moveState;
	p.createGravityState = createGravityState;

	this->points.push_back(p);
}

void Scene::DestroyAllPoint() {
	this->points.clear();
}