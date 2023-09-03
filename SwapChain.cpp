#include "SwapChain.h"

void SwapChain::createSwapChain
(
	PhysicalDevice& physicalDevice, 
	LogicalDevice logicalDevice, 
	VkSurfaceKHR& surface, 
	GLFWwindow& window
)
{
	SwapChainSupportDetails swapChainSupport = physicalDevice.querySwapChainSupport(*physicalDevice, surface);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);


	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 &&
		imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	QueueFamilyIndices indices = physicalDevice.findQueueFamilies(*physicalDevice, surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	if (vkCreateSwapchainKHR(*logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(*logicalDevice, m_swapChain, &imageCount, nullptr);
	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(*logicalDevice, m_swapChain, &imageCount, m_swapChainImages.data());

	m_swapChainImageFormat = surfaceFormat.format;
	m_swapChainExtent = extent;

} // void createSwapChain()

auto SwapChain::chooseSwapExtent
(
	const VkSurfaceCapabilitiesKHR& capabilities, 
	GLFWwindow& window
) -> VkExtent2D
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(&window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height);
		return actualExtent;

	}
} // VkExtent2D chooseSwapExtent()

auto SwapChain::chooseSwapSurfaceFormat(
	const std::vector<VkSurfaceFormatKHR>& availableFormats
) -> VkSurfaceFormatKHR
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
} // chooseSwapSurfaceFormat()

auto SwapChain::chooseSwapPresentMode(
	const std::vector <VkPresentModeKHR>& availablePresentModes
) -> VkPresentModeKHR 
{
	return VK_PRESENT_MODE_FIFO_KHR;
} // chooseSwapPresentMode()

void SwapChain::recreateSwapChain(
	GLFWwindow& window, 
	PhysicalDevice& physicalDevice, 
	LogicalDevice& logicalDevice, 
	VkSurfaceKHR& surface, 
	VkRenderPass& renderPass, 
	SwapChain& swapChain
)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(&window, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(&window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(*logicalDevice);

	cleanupSwapChain(*logicalDevice);

	createSwapChain(physicalDevice, logicalDevice, surface, window);
	createImageViews(logicalDevice);
	createColorResources(physicalDevice, logicalDevice, swapChain);
	createDepthResources(physicalDevice, logicalDevice, swapChain);
	createFramebuffers(logicalDevice, renderPass);

} // recreateSwapChain()

void SwapChain::cleanupSwapChain(
	VkDevice logicalDevice
)
{
	vkDestroyImageView(logicalDevice, m_colorImageView, nullptr);
	vkDestroyImage(logicalDevice, m_colorImage, nullptr);
	vkFreeMemory(logicalDevice, m_colorImageMemory, nullptr);

	vkDestroyImageView(logicalDevice, m_depthImageView, nullptr);
	vkDestroyImage(logicalDevice, m_depthImage, nullptr);
	vkFreeMemory(logicalDevice, m_depthImageMemory, nullptr);

	for (size_t i = 0; i < m_swapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(logicalDevice, m_swapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
	{
		vkDestroyImageView(logicalDevice, m_swapChainImageViews[i], nullptr);
	}
	vkDestroySwapchainKHR(logicalDevice, m_swapChain, nullptr);
} // cleanupSwapChain()

void SwapChain::createImageViews(
	LogicalDevice& logicalDevice
)
{
	m_swapChainImageViews.resize(m_swapChainImages.size());
	for (size_t i = 0; i < m_swapChainImages.size(); i++)
	{
		m_swapChainImageViews[i] = createImageView(logicalDevice, m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
} // createImageViews()

auto SwapChain::createImageView(
	LogicalDevice& logicalDevice,
	VkImage image, 
	VkFormat format, 
	VkImageAspectFlags aspectFlags, 
	uint32_t mipLevels
) -> VkImageView
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(*logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
} // createImageView()

void SwapChain::createColorResources(
	PhysicalDevice& physicalDevice, 
	LogicalDevice& logicalDevice, 
	SwapChain& swapChain
)
{
	VkFormat colorFormat = m_swapChainImageFormat;

	createImage
	(
		physicalDevice,
		logicalDevice,
		m_swapChainExtent.width, 
		m_swapChainExtent.height, 
		1, 
		physicalDevice.m_msaaSamples, 
		colorFormat, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		swapChain.m_colorImage,
		swapChain.m_colorImageMemory
	);

	m_colorImageView = createImageView(logicalDevice, swapChain.m_colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

} // createColorResources()

void SwapChain::createDepthResources(
	PhysicalDevice& physicalDevice, 
	LogicalDevice& logicalDevice, 
	SwapChain& swapChain)
{
	VkFormat depthFormat = findDepthFormat(physicalDevice);

	createImage
	(
		physicalDevice,
		logicalDevice,
		m_swapChainExtent.width, 
		m_swapChainExtent.height, 
		1, 
		physicalDevice.m_msaaSamples, 
		depthFormat, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		swapChain.m_depthImage,
		swapChain.m_depthImageMemory
	);

	m_depthImageView = createImageView(logicalDevice, swapChain.m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
} // createDepthResources()

void SwapChain::createImage(
	PhysicalDevice& physicalDevice,
	LogicalDevice& logicalDevice,
	uint32_t width, 
	uint32_t height, 
	uint32_t mipLevels, 
	VkSampleCountFlagBits numSamples, 
	VkFormat format, 
	VkImageTiling tiling, 
	VkImageUsageFlags usage, 
	VkMemoryPropertyFlags properties, 
	VkImage& image, 
	VkDeviceMemory& imageMemory
)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSamples;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(*logicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{throw std::runtime_error("failed to create image!");}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(*logicalDevice, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(*logicalDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{throw std::runtime_error("failed to allocate image memory!");}

	vkBindImageMemory(*logicalDevice, image, imageMemory, 0);
} // createImage()

auto SwapChain::findDepthFormat(
	PhysicalDevice& physicalDevice
) -> VkFormat
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
		physicalDevice
	);
} // findDepthFormat()	

auto SwapChain::findSupportedFormat(
	const std::vector<VkFormat>& candidates, 
	VkImageTiling tiling, 
	VkFormatFeatureFlags features, 
	PhysicalDevice& physicalDevice
) -> VkFormat
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(*physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{ return format; }
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{ return format; }
	}

	throw std::runtime_error("failed to find supported format!");
} // findSupportedFormat()

 auto SwapChain::findMemoryType(
	 PhysicalDevice& physicalDevice, 
	 uint32_t typeFilter, 
	 VkMemoryPropertyFlags properties
 ) -> uint32_t
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(*physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");

} // findMemoryType()

 void SwapChain::createFramebuffers(
	 LogicalDevice& logicalDevice, 
	 VkRenderPass& renderPass
 )
 {
	 m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

	 for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
	 {
		 std::array<VkImageView, 3> attachments = {
			 m_colorImageView,
			 m_depthImageView,
			 m_swapChainImageViews[i]
		 };

		 VkFramebufferCreateInfo framebufferInfo{};
		 framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		 framebufferInfo.renderPass = renderPass;
		 framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		 framebufferInfo.pAttachments = attachments.data();
		 framebufferInfo.width = m_swapChainExtent.width;
		 framebufferInfo.height = m_swapChainExtent.height;
		 framebufferInfo.layers = 1;

		 if (vkCreateFramebuffer(*logicalDevice, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
		 {
			 throw std::runtime_error("failed to create framebuffer!");
		 }
	 }
 } // createFramebuffers
