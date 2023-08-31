#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include <algorithm>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "Window.h"
#include "RenderPass.h"

class SwapChain
{
public:
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	}; // struct SwapChainSupportDetails

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


	auto chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window& window) -> VkExtent2D;
	auto chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) -> VkSurfaceFormatKHR;
	auto chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) -> VkPresentModeKHR;
	
	void createSwapChain(PhysicalDevice& physicalDevice, LogicalDevice logicalDevice, Surface& surface, Window& window);
	void recreateSwapChain(Window& window, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Surface& surface, RenderPass& renderPass, SwapChain& swapChain);
	void createImageViews(LogicalDevice& logicalDevice);
	auto createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, LogicalDevice& logicalDevice) -> VkImageView;
	void createColorResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain);
	void createDepthResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	void createFramebuffers(LogicalDevice& logicalDevice, RenderPass& renderPass);
	
	auto querySwapChainSupport(VkPhysicalDevice device, Surface surface) -> SwapChain::SwapChainSupportDetails;
	auto findDepthFormat(PhysicalDevice& physicalDevice) -> VkFormat;
	auto findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, PhysicalDevice& physicalDevice) -> VkFormat;
	auto findMemoryType(PhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t;
	
	void cleanupSwapChain(VkDevice logicalDevice);

	// Allows us to use the class like a pointer
	auto operator*() -> VkSwapchainKHR& { return m_swapChain; }

}; // class SwapChain