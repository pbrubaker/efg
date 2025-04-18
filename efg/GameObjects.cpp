#include "GameObjects.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

void Mesh::draw(GfxContext const& gfx, GfxProgram const& program)
{
    gfxProgramSetParameter(gfx, program, "transform", modelMatrix);
	gfxProgramSetParameter(gfx, program, "MaterialBuffer", material.cBuffer);
    gfxProgramSetParameter(gfx, program, "useDiffuseMap", material.useDiffuseMap);
    gfxProgramSetParameter(gfx, program, "useSpecMap", material.useSpecMap);

    if (material.useDiffuseMap)
    {
        gfxProgramSetParameter(gfx, program, "diffuseMap", material.diffuseMap);
    }
	if (material.useSpecMap)
	{
        gfxProgramSetParameter(gfx, program, "specularMap", material.specularMap);
	}


    gfxProgramSetParameter(gfx, program, "instances", instanceBuffer);
    gfxCommandBindIndexBuffer(gfx, indexBuffer);
    gfxCommandBindVertexBuffer(gfx, vertexBuffer);
    gfxCommandDrawIndexed(gfx, (uint32_t)indices.size(), instances);

}

void Mesh::gui()
{
	update |= ImGui::InputFloat3("Position", &position[0], "%.3f");
	update |= ImGui::InputFloat3("Rotation", &rotation[0], "%.3f");
	update |= ImGui::InputFloat3("Scale", &scale[0], "%.3f");
	ImGui::Separator();
	ImGui::InputFloat3("Ambient", &material.cBuffer.properties.ambient[0]);
	ImGui::Separator();
	ImGui::InputFloat3("Diffuse", &material.cBuffer.properties.diffuse[0]);
	ImGui::Separator();
	ImGui::InputFloat3("Specular", &material.cBuffer.properties.specular[0]);
	ImGui::Separator();
	ImGui::InputFloat("Shininess", &material.cBuffer.properties.shininess);
}

void Mesh::destroy(GfxContext const& gfx)
{
	gfxDestroyBuffer(gfx, indexBuffer);
	gfxDestroyBuffer(gfx, vertexBuffer);
	gfxDestroyTexture(gfx, material.diffuseMap);
	gfxDestroyTexture(gfx, material.specularMap);
	gfxDestroyBuffer(gfx, instanceBuffer);
}

void Instanced::draw(GfxContext const& gfx, GfxProgram const& program)
{
	gfxProgramSetParameter(gfx, program, "MaterialBuffer", material);
}

void Instanced::gui()
{
	ImGui::InputFloat3("Position", &position[0], "%.3f");
	ImGui::InputFloat3("Rotation", &rotation[0], "%.3f");
	ImGui::InputFloat3("Scale", &scale[0], "%.3f");
}

void Instanced::destroy(GfxContext const& gfx)
{

}

glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::vec3 rotationRadians = glm::radians(rotation);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(rotationRadians.y, rotationRadians.x, rotationRadians.z);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);
	return translationMatrix * rotationMatrix * scalingMatrix;
}
