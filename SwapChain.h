#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include <algorithm>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"

class SwapChain
{
public:
	VkSwapchainKHR			   m_swapChain;
	std::vector<VkImage>	   m_swapChainImages;
	VkFormat				   m_swapChainImageFormat;
	VkExtent2D				   m_swapChainExtent;
	std::vector<VkImageView>   m_swapChainImageViews;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;
	VkImage					   m_colorImage;
	VkDeviceMemory			   m_colorImageMemory;
	VkImageView				   m_colorImageView;
	VkImage					   m_depthImage;
	VkDeviceMemory			   m_depthImageMemory;
	VkImageView				   m_depthImageView;

	static auto chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window) -> VkExtent2D;
	static auto chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) -> VkSurfaceFormatKHR;
	static auto chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) -> VkPresentModeKHR;

	void recreateSwapChain(GLFWwindow& window, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, VkSurfaceKHR& surface, VkRenderPass& renderPass, SwapChain& swapChain);
	void createSwapChain(PhysicalDevice& physicalDevice, LogicalDevice logicalDevice, VkSurfaceKHR& surface, GLFWwindow& window);
	void createImageViews(LogicalDevice& logicalDevice);
	auto createImageView(LogicalDevice& logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) -> VkImageView;
	void createColorResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain);
	void createDepthResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain);
	void createImage(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void createFramebuffers(LogicalDevice& logicalDevice, VkRenderPass& renderPass);
	
	static auto findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, PhysicalDevice& physicalDevice) -> VkFormat;
	static auto findMemoryType(PhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t;
	static auto findDepthFormat(PhysicalDevice& physicalDevice) -> VkFormat;

	
	void cleanupSwapChain(VkDevice logicalDevice);

	// Allows us to use the class like a pointer
	auto operator*() -> VkSwapchainKHR& { return m_swapChain; }

}; // class SwapChain