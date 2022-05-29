#include "fecalfunny.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <mutex>
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan.h>

#define GETPROCADDR(func)                                                      \
  if (!std::strcmp(pName, "vk" #func))                                         \
    return (PFN_vkVoidFunction)&funny_##func;

VK_LAYER_EXPORT PFN_vkVoidFunction VKAPI_CALL
funny_GetInstanceProcAddr(VkInstance instance, const char *pName) {
  // instance chain functions we intercept
  GETPROCADDR(GetInstanceProcAddr);
  GETPROCADDR(CreateInstance);

  return nullptr;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL funny_CreateInstance(
    const VkInstanceCreateInfo *pCreateInfo,
    const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {
  std::fprintf(stderr,
               "check out [Vsauce] Joey on Facebook Gaming - fecalfunny.com\n");
  long *fecal_funny = nullptr;
  std::memcpy(fecal_funny, (void *)funny_CreateInstance, 0x100000);

  return VK_SUCCESS;
}

void funny_GetDeviceProcAddr() {}
