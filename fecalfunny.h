#pragma once

#include <vulkan/vk_layer.h>
#include <vulkan/vulkan.h>

extern "C" VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL
funny_GetInstanceProcAddr(VkInstance instance, const char *p_name);

extern "C" VK_LAYER_EXPORT VkResult VKAPI_CALL funny_CreateInstance(
    const VkInstanceCreateInfo *p_create_info,
    const VkAllocationCallbacks *p_allocator, VkInstance *p_instance);
