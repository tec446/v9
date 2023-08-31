#pragma once

#include <vulkan/vulkan.h>
#include <tiny_obj_loader.h>
#include <stdexcept>
#include <unordered_map>

#include "GraphicsPipeline.h"
#include "CommandPool.h"

class ModelLoader
{
public:
	const std::string MODEL_PATH = "viking_room.obj";
	const std::string TEXTURE_PATH = "viking_room.png";


	void loadModel(CommandPool& commandPool);

}; // class ModelLoader