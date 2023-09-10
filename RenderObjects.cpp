#include "RenderObjects.h"

bool RenderObjects::createRenderObject(
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
) {

	m_descriptorSets.push_back(descriptorSet);
	m_vertices.push_back(vertices);
	m_indices.push_back(indices);
	m_vertexBuffer.push_back(vertexBuffer);
	m_vertexBufferMemory.push_back(vertexBufferMemory);
	m_indexBuffer.push_back(indexBuffer);
	m_indexBufferMemory.push_back(indexBufferMemory);
	m_uniformBuffers.push_back(uniformBuffer);
	m_uniformBuffersMemory.push_back(uniformBufferMemory);
	m_uniformBuffersMapped.push_back(uniformBufferMapped);
	m_textureImage.push_back(textureImage);
	m_textureImageMemory.push_back(textureImageMemory);
	m_textureImageView.push_back(textureImageView);
	m_textureSampler.push_back(textureSampler);

	m_objectIndices.push_back(m_objectIndices.size());
	m_objectNames.push_back(objectName);

	// TODO: check if object already exists first

	return true;
}
