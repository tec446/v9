#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "Types.h"

struct Coordinates3D
{
	int16_t x, y, z;
}; // struct Coordinates

struct TextureObject
{
	// per unique texture
	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

}; // class TextureObject

// Contains renderable objects
class RenderObjects
{
private:
	// one per unique object
	std::vector<VkDescriptorSet> m_descriptorSets{};
	// one per object
	std::vector<std::vector<Vertex>>   m_vertices{};
	std::vector<std::vector<uint32_t>> m_indices{};
	// one per unique object
	std::vector<VkBuffer>		m_vertexBuffer{};
	std::vector<VkDeviceMemory> m_vertexBufferMemory{};
	// one per unique object
	std::vector<VkBuffer>		m_indexBuffer{};
	std::vector<VkDeviceMemory> m_indexBufferMemory{};
	// one per unique object
	std::vector<VkBuffer>		m_uniformBuffers{};
	std::vector<VkDeviceMemory> m_uniformBuffersMemory{};
	std::vector<void*>			m_uniformBuffersMapped{};
	// one per unique texture
	std::vector<VkImage>		m_textureImage{};
	std::vector<VkDeviceMemory> m_textureImageMemory{};
	std::vector<VkImageView>	m_textureImageView{};
	std::vector<VkSampler>		m_textureSampler{};

	// which indices are free for reuse
	std::vector<uint16_t>	 m_freeIndices{};
	// stores object data as indices to the above vectors
	std::vector<uint16_t>	 m_objectIndices;
	// stores object names for debugging
	std::vector<std::string> m_objectNames;

public:
	bool createRenderObject(
		VkDescriptorSet		  descriptorSet,
		VkDescriptorSetLayout descriptorSetLayout,
		std::vector<Vertex>	  vertices,
		std::vector<uint32_t> indices,
		VkBuffer			  vertexBuffer,
		VkDeviceMemory		  vertexBufferMemory,
		VkBuffer			  indexBuffer,
		VkDeviceMemory		  indexBufferMemory,
		VkBuffer			  uniformBuffer,
		VkDeviceMemory		  uniformBufferMemory,
		void*				  uniformBufferMapped,
		VkImage				  textureImage,
		VkDeviceMemory		  textureImageMemory,
		VkImageView			  textureImageView,
		VkSampler			  textureSampler,
		std::string			  objectName
	);

}; // class RenderObjects

// Contains instances of renderable objects with references to RenderObjects
class RenderObjectInstances
{
	// used to access objectIndices in RenderObjects
	std::vector<uint16_t> m_referenceIndices;
	// used to determine object position in world
	// may be used for multiple objects through nested vectors
	std::vector<std::vector<Coordinates3D>> m_coordinates;

}; // class RenderObjectInstances
