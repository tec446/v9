#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <unordered_map>
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>
#include <array>
#include <fstream>
#include <set>
#include <optional>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <algorithm>

#include "vulkanConfig.h"
#include "Instance.h"
#include "DebugMessenger.h"
#include "Surface.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SwapChain.h"
#include "Window.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "DescriptorSet.h"
#include "CommandPool.h"
#include "TextureImage.h"
#include "TextureSampler.h"
#include "ModelLoader.h"
#include "UniformBuffers.h"
#include "SyncObjects.h"



class HelloTriangleApplication 
{
public:
	void run() {
		m_window.initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	// Wrappers
	Instance		 m_instance{};
	DebugMessenger	 m_debugMessenger{};
	Surface			 m_surface{};
	PhysicalDevice	 m_physicalDevice{};
	LogicalDevice	 m_logicalDevice{};
	SwapChain		 m_swapChain{};
	Window 			 m_window{};
	RenderPass		 m_renderPass{};
	DescriptorSet	 m_descriptorSets{};
	GraphicsPipeline m_graphicsPipeline{};
	CommandPool      m_commandPool{};
	TextureImage	 m_textureImage{};
	TextureSampler   m_textureSampler{};
	ModelLoader		 m_modelLoader{};
	UniformBuffers   m_uniformBuffers{};
	SyncObjects      m_syncObjects{};
	// Wrappers






	bool framebufferResized = false;

	void initVulkan() {
		m_instance.createInstance(m_debugMessenger);
		m_debugMessenger.setupDebugMessenger(m_instance);
		m_surface.createSurface(m_instance, m_window);
		m_physicalDevice.pickPhysicalDevice(m_instance, m_swapChain, m_surface);
		m_logicalDevice.createLogicalDevice(m_physicalDevice, m_instance, m_surface);
		m_swapChain.createSwapChain(m_physicalDevice, m_logicalDevice, m_surface, m_window);
		m_swapChain.createImageViews(m_logicalDevice);
		m_renderPass.createRenderPass(m_physicalDevice, m_logicalDevice, m_swapChain);
		m_descriptorSets.createDescriptorSetLayout(m_logicalDevice);
		m_graphicsPipeline.createGraphicsPipeline(m_physicalDevice, m_logicalDevice, m_descriptorSets, m_renderPass);
		m_commandPool.createCommandPool(m_physicalDevice, m_logicalDevice, m_surface);
		m_swapChain.createColorResources(m_physicalDevice, m_logicalDevice, m_swapChain);
		m_swapChain.createDepthResources(m_physicalDevice, m_logicalDevice, m_swapChain);
		m_swapChain.createFramebuffers(m_logicalDevice, m_renderPass);
		m_textureImage.createTextureImage();
		m_textureSampler.createTextureSampler();
		m_modelLoader.loadModel();
		m_uniformBuffers.createVertexBuffer(m_logicalDevice);
		m_uniformBuffers.createIndexBuffer();
		m_uniformBuffers.createUniformBuffers(m_logicalDevice);
		m_descriptorSets.createDescriptorPool(m_logicalDevice);
		m_descriptorSets.createDescriptorSets(m_logicalDevice, m_uniformBuffers, m_textureImage);
		m_commandPool.createCommandBuffers(m_logicalDevice, m_descriptorSets);
		m_syncObjects.createSyncObjects(*m_logicalDevice);

	} // initVulkan()

	void mainLoop()
	{
		while (!glfwWindowShouldClose(*m_window))
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(*m_logicalDevice);
	}

	void cleanup()
	{
		m_swapChain.cleanupSwapChain();

		vkDestroySampler(*m_logicalDevice, textureSampler, nullptr);
		vkDestroyImageView(*m_logicalDevice, textureImageView, nullptr);
		vkDestroyImage(*m_logicalDevice, textureImage, nullptr);
		vkFreeMemory(*m_logicalDevice, textureImageMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroyBuffer(*m_logicalDevice, uniformBuffers[i], nullptr);
			vkFreeMemory(*m_logicalDevice, uniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(*m_logicalDevice, descriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(*m_logicalDevice, descriptorSetLayout, nullptr);

		vkDestroyBuffer(*m_logicalDevice, indexBuffer, nullptr);
		vkFreeMemory(*m_logicalDevice, indexBufferMemory, nullptr);

		vkDestroyBuffer(*m_logicalDevice, vertexBuffer, nullptr);
		vkFreeMemory(*m_logicalDevice, vertexBufferMemory, nullptr);

		vkDestroyPipeline(*m_logicalDevice, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(*m_logicalDevice, pipelineLayout, nullptr);

		vkDestroyRenderPass(*m_logicalDevice, renderPass, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(*m_logicalDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(*m_logicalDevice, renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(*m_logicalDevice, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(*m_logicalDevice, commandPool, nullptr);

		vkDestroyDevice(*m_logicalDevice, nullptr);

		if (enableValidationLayers)
		{
			m_debugMessenger.DestroyDebugUtilsMessengerEXT(*m_instance, *m_debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(*m_instance, *m_surface, nullptr);
		vkDestroyInstance(*m_instance, nullptr);

		glfwDestroyWindow(*m_window);

		glfwTerminate();
	}

	bool hasStencilComponent(VkFormat format)
	{ return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }



	void drawFrame() {
		vkWaitForFences(*m_logicalDevice, 1, &m_commandPool.m_inFlightFences[m_commandPool.m_currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(*m_logicalDevice, *m_swapChain, UINT64_MAX,
			m_commandPool.m_imageAvailableSemaphores[m_commandPool.m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			m_swapChain.recreateSwapChain(m_window, m_physicalDevice, m_logicalDevice, m_surface, m_renderPass, m_swapChain);
			return;
		} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		// Only reset fence when submitting work
		vkResetFences(*m_logicalDevice, 1, &m_commandPool.m_inFlightFences[m_commandPool.m_currentFrame]);

		m_uniformBuffers.vkUpdateUniformBuffer(m_commandPool.m_currentFrame);

		vkResetCommandBuffer(m_commandPool.m_commandBuffers[m_commandPool.m_currentFrame], 0);
		m_commandPool.recordCommandBuffer(m_commandPool.m_commandBuffers[m_commandPool.m_currentFrame], imageIndex, m_renderPass, m_swapChain, m_graphicsPipeline, m_descriptorSets);
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_commandPool.m_imageAvailableSemaphores[m_commandPool.m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandPool.m_commandBuffers[m_commandPool.m_currentFrame];
		VkSemaphore signalSemaphores[] = { m_commandPool.m_renderFinishedSemaphores[m_commandPool.m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { *m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		if (vkQueueSubmit(m_logicalDevice.m_graphicsQueue, 1, &submitInfo, m_commandPool.m_inFlightFences[m_commandPool.m_currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		result = vkQueuePresentKHR(m_logicalDevice.m_presentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			m_swapChain.recreateSwapChain(m_window, m_physicalDevice, m_logicalDevice, m_surface, m_renderPass, m_swapChain);
		} else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}

		m_commandPool.m_currentFrame = (m_commandPool.m_currentFrame + 1) % m_descriptorSets.MAX_FRAMES_IN_FLIGHT;
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	} // framebufferResizeCallback()

}; // HelloTriangleApplication

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	
	return EXIT_SUCCESS;
}