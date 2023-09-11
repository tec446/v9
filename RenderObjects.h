#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "Types.h"
#include "Device.h"
#include "Buffer.h"


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
public:
	// one per unique object
	// 2D vector to allow for multiple frames in flight
	// should be resized to frames in flight at some point
	std::vector<std::vector<VkDescriptorSet>> m_descriptorSets{};
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
	std::vector<uint16_t>	 m_objectIndices{};
	// stores object names for debugging
	std::vector<std::string> m_objectNames{};

	bool createRenderObject(
		std::vector<VkDescriptorSet> descriptorSets,
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


	void transitionImageLayout(Device& device, CommandPool& commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void generateMipmaps(Device& device, CommandPool& commandPool, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void createImage(Device& device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void createTextureImage(Device& device, CommandPool& commandPool, SwapChain& swapChain, std::string texturePath, uint32_t mipLevels, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
	auto createTextureImageView(Device& device, SwapChain& swapChain, uint32_t mipLevels, VkImage& textureImage) -> VkImageView;
	auto createImageView(Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) -> VkImageView;
	void createTextureSampler(Device& device, uint32_t mipLevels, VkSampler& textureSampler);
	void createUniformBuffers(Device& device, int maxFramesInFlight);
	void createVertexBuffer(Device& device, CommandPool& commandPool);
	void createIndexBuffer(Device& device, CommandPool& commandPool); 
	void createDescriptorSets(VkDevice& logicalDevice, std::vector<VkBuffer>& uniformBuffers, VkImageView& textureImageView, VkSampler& textureSampler, std::vector<VkDescriptorSet>& descriptorSets, uint32_t maxFramesInFlight, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool);
	
	static auto findMemoryType(Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t;

}; // class RenderObjects

// Contains instances of renderable objects with references to RenderObjects
class RenderObjectInstances
{
public:
	// used to access objectIndices in RenderObjects
	std::vector<uint16_t> m_referenceIndices{};
	// used to determine object position in world
	// may be used for multiple objects through nested vectors
	std::vector<std::vector<Coordinates3D>> m_coordinates{};

	void createRenderObjectInstance(
		uint16_t referenceIndex,
		Coordinates3D coordinates
	);

}; // class RenderObjectInstances
