#include "GroundEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

GroundEntity::GroundEntity(Mesh* _modelMesh1, Mesh* _modelMesh2) :
position(0.0f, 0.0f, 0.0f),
scale(1.0f, 1.0f, 1.0f)
{
	modelMesh[0] = _modelMesh1;
	modelMesh[1] = _modelMesh2;
}

GroundEntity::~GroundEntity()
{
}

void GroundEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GroundEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(400.0f, 400.0f, 400.0f);

	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			if (((x * 9 + z) % 2) == 0)
				RenderHelper::RenderMesh(modelMesh[0]);
			else
				RenderHelper::RenderMesh(modelMesh[1]);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

GroundEntity* Create::Ground(const std::string& _meshName1, const std::string& _meshName2)
{
	Mesh* modelMesh1 = MeshBuilder::GetInstance()->GetMesh(_meshName1);
	if (modelMesh1 == nullptr)
		return nullptr;

	Mesh* modelMesh2 = MeshBuilder::GetInstance()->GetMesh(_meshName2);
	if (modelMesh2 == nullptr)
		return nullptr;

	GroundEntity* result = new GroundEntity(modelMesh1, modelMesh2);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}