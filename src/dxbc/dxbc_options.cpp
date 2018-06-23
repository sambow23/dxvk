#include <unordered_map>

#include "dxbc_options.h"

namespace dxvk {
  
  const static std::unordered_map<std::string, DxbcOptions> g_dxbcAppOptions = {{
    
  }};
  
  
  DxbcOptions getDxbcAppOptions(const std::string& appName) {
    auto appOptions = g_dxbcAppOptions.find(appName);
    
    return appOptions != g_dxbcAppOptions.end()
      ? appOptions->second
      : DxbcOptions();
  }
  
  
  DxbcOptions getDxbcDeviceOptions(const Rc<DxvkDevice>& device) {
    DxbcOptions flags;
    
    VkPhysicalDeviceFeatures devFeatures = device->features();
    
    const DxvkGpuVendor vendor = static_cast<DxvkGpuVendor>(devProps.vendorID);
    
    if (vendor == DxvkGpuVendor::Nvidia) {
      flags.set(
        DxbcOption::AddExtraDrefCoordComponent,
        DxbcOption::UseSimpleMinMaxClamp);
    }
    
    if (devFeatures.shaderStorageImageReadWithoutFormat)
      flags.set(DxbcOption::UseStorageImageReadWithoutFormat);
    
    flags.set(DxbcOption::DeferKill);
    return flags;
  }
  
}