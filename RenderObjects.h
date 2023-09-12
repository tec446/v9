#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "Types.h"
#include "Device.h"
#include "Buffer.h"

// Forward declarations
class DescriptorPool;
class CommandPool;

struct Coordinates3D
{
	int16_t x, y, z;
}; // struct Coordinates

// Contains renderable objects
class RenderObjects
{
private:
public:
	/// Data relevent to all instances of an object
	struct Objects {

		enum class IndexType
		{
			textureImage,
			textureImageMemory,
			textureImageView,
			textureSampler,
			vertices,
			vertexBuffer,
			vertexBufferMemory,
			indices,
			indexBuffer,
			indexBufferMemory,
			objectName
		};

		std::vector<VkImage>			   textureImage{};
		std::vector<VkDeviceMemory>		   textureImageMemory{};
		std::vector<VkImageView>		   textureImageView{};
		std::vector<VkSampler>			   textureSampler{};

		std::vector<std::vector<Vertex>>   vertices{};
		std::vector<VkBuffer>			   vertexBuffer{};
		std::vector<VkDeviceMemory>		   vertexBufferMemory{};
		std::vector<std::vector<uint32_t>> indices{};
		std::vector<VkBuffer>			   indexBuffer{};
		std::vector<VkDeviceMemory>		   indexBufferMemory{};

		std::vector<std::string>		   objectNames{};

		std::vector<std::vector<uint16_t>> objectIndices{};
		std::vector<std::vector<uint16_t>> freeIndices{};

	}; // struct Objects

	/// Data relevent to individual instances of an object
	struct Instances {
		// holds transform data
		std::vector<std::vector<VkBuffer>>	 	 uniformBuffers{};
		std::vector<std::vector<VkDeviceMemory>> uniformBuffersMemory{};
		std::vector<std::vector<void*>>		 	 uniformBuffersMapped{};

		// DescriptorSets, 2D vector for multiple frames in flight
		std::vector<std::vector<VkDescriptorSet>> descriptorSets{};

		// used to access objectIndices in ObjectData
		std::vector<uint16_t> referenceIndices{};

	}; // struct instanceData

	Objects	  m_objects{};
	Instances m_instances{};

	void createRenderInstance(Device& device, int maxFramesInFlight, int referenceIndex, DescriptorPool& descriptorPool);

	int createRenderObject(VkImage textureImage, VkDeviceMemory tex1tureImageMemory, VkImageView textureImageView, VkSampler textureSampler, std::vector<Vertex> vertices, VkBuffer vertexBuffer, VkDeviceMemory vertexBufferMemory, std::vector<uint32_t> indices, VkBuffer indexBuffer, VkDeviceMemory indexBufferMemory, std::string objectName);

	static void transitionImageLayout(Device& device, CommandPool& commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	static void generateMipmaps(Device& device, CommandPool& commandPool, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	static void createImage(Device& device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	static void createTextureImage(Device& device, CommandPool& commandPool, std::string& texturePath, uint32_t mipLevels, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
	static void createTextureImageView(Device& device, uint32_t mipLevels, VkImage& textureImage, VkImageView& imageView);
	static void createImageView(Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkImageView& imageView);
	static void createTextureSampler(Device& device, uint32_t mipLevels, VkSampler& textureSampler);
	static void createUniformBuffers(Device& device, int maxFramesInFlight, std::vector<VkBuffer>& uniformBuffers, std::vector<VkDeviceMemory>& uniformBuffersMemory, std::vector<void*>& uniformBuffersMapped);
	static void createVertexBuffer(Device& device, CommandPool& commandPool, std::vector<Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
	static void createIndexBuffer(Device& device, CommandPool& commandPool, std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
	static void createDescriptorSets(VkDevice& logicalDevice, std::vector<VkBuffer>& uniformBuffers, VkImageView& textureImageView, VkSampler& textureSampler, std::vector<VkDescriptorSet>& descriptorSets, uint32_t maxFramesInFlight, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool);

	static auto findMemoryType(Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t;

}; // class RenderObjects


