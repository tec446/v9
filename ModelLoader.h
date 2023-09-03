#pragma once

#include <vulkan/vulkan.h>
#include <tiny_obj_loader.h>
#include <stdexcept>
#include <unordered_map>

#include "Types.h"
#include "TempMagicValues.h"

namespace ModelLoader
{
	void loadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

}; // namespace ModelLoader