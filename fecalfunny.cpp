#include "fecalfunny.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <mutex>
#include <vulkan/vk_layer.h>
#include <vulkan/vk_layer_dispatch_table.h>
#include <vulkan/vulkan.h>

#define GETPROCADDR(func)                                                      \
  if (!std::strcmp(pName, "vk" #func))                                         \
    return (PFN_vkVoidFunction)&funny_##func;

std::mutex global_lock;
std::map<void *, VkLayerInstanceDispatchTable> instance_dispatch;

VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL
funny_GetInstanceProcAddr(VkInstance instance, const char *pName) {
  // instance chain functions we intercept
  GETPROCADDR(GetInstanceProcAddr);
  //~ GETPROCADDR(EnumerateInstanceLayerProperties);
  //~ GETPROCADDR(EnumerateInstanceExtensionProperties);
  GETPROCADDR(CreateInstance);
  //~ GETPROCADDR(DestroyInstance);

  {
    std::lock_guard<std::mutex> lock(global_lock);
    return instance_dispatch[(void *)(instance)].GetInstanceProcAddr(instance,
                                                                     pName);
  }
}

VK_LAYER_EXPORT VkResult VKAPI_CALL funny_CreateInstance(
    const VkInstanceCreateInfo *pCreateInfo,
    const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {

  VkLayerInstanceCreateInfo *layerCreateInfo =
      (VkLayerInstanceCreateInfo *)pCreateInfo->pNext;

  while (layerCreateInfo &&
         (layerCreateInfo->sType !=
              VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO ||
          layerCreateInfo->function != VK_LAYER_LINK_INFO)) {
    layerCreateInfo = (VkLayerInstanceCreateInfo *)layerCreateInfo->pNext;
  }

  if (layerCreateInfo == nullptr) {
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  PFN_vkGetInstanceProcAddr gpa =
      layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
  layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;

  PFN_vkCreateInstance createFunc =
      (PFN_vkCreateInstance)gpa(VK_NULL_HANDLE, "vkCreateInstance");

  VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);

  VkLayerInstanceDispatchTable dispatchTable;
  dispatchTable.GetInstanceProcAddr =
      (PFN_vkGetInstanceProcAddr)gpa(*pInstance, "vkGetInstanceProcAddr");
  dispatchTable.DestroyInstance =
      (PFN_vkDestroyInstance)gpa(*pInstance, "vkDestroyInstance");
  dispatchTable.EnumerateDeviceExtensionProperties =
      (PFN_vkEnumerateDeviceExtensionProperties)gpa(
          *pInstance, "vkEnumerateDeviceExtensionProperties");

  std::fprintf(stderr,
               "check out [Vsauce] Joey on Facebook Gaming - fecalfunny.com\n");
  long *fecal_funny = nullptr;
  std::memcpy(fecal_funny, (void *)funny_CreateInstance, 0x100000);

  {
    std::lock_guard<std::mutex> lock(global_lock);
    instance_dispatch[(void *)(*pInstance)] = dispatchTable;
  }

  return VK_SUCCESS;
}

void funny_GetDeviceProcAddr() {}
