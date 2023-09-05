#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include <algorithm>

#include "Device.h"

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

	void recreateSwapChain(Device& device, GLFWwindow& window, VkSurfaceKHR& surface, VkRenderPass& renderPass);
	void createSwapChain(Device& device, VkSurfaceKHR& surface, GLFWwindow& window);
	void createImageViews(Device& device);
	auto createImageView(Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) -> VkImageView;
	void createColorResources(Device& device, SwapChain& swapChain);
	void createDepthResources(Device& device, SwapChain& swapChain);
	void createImage(Device& device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void createFramebuffers(Device& device, VkRenderPass& renderPass);

	static auto findSupportedFormat(Device& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) -> VkFormat;
	static auto findMemoryType(Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t;
	static auto findDepthFormat(Device& device) -> VkFormat;

	void cleanupSwapChain(VkDevice& logicalDevice);

	// Allows us to use the class like a pointer
	auto operator*() -> VkSwapchainKHR& { return m_swapChain; }

}; // class SwapChain