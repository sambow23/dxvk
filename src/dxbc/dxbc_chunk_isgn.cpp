#include "dxbc_chunk_isgn.h"

namespace dxvk {
  
  DxbcIsgn::DxbcIsgn(DxbcReader reader, DxbcTag tag) {
    uint32_t elementCount = reader.readu32();
    reader.skip(sizeof(uint32_t));
    
    std::array<DxbcScalarType, 4> componentTypes = {
      DxbcScalarType::Uint32, DxbcScalarType::Uint32,
      DxbcScalarType::Sint32, DxbcScalarType::Float32,
    };
    
    for (uint32_t i = 0; i < elementCount; i++) {
      DxbcSgnEntry entry;
      entry.streamId        = tag == "OSG5" ? reader.readu32() : 0;
      entry.semanticName    = reader.clone(reader.readu32()).readString();
      entry.semanticIndex   = reader.readu32();
      entry.systemValue     = static_cast<DxbcSystemValue>(reader.readu32());
      entry.componentType   = componentTypes.at(reader.readu32());
      entry.registerId      = reader.readu32();
      entry.componentMask   = bit::extract(reader.readu32(), 0, 3);

      m_entries.push_back(entry);
    }
  }
  
  
  DxbcIsgn::~DxbcIsgn() {
    
  }
  
  
  const DxbcSgnEntry* DxbcIsgn::findByRegister(uint32_t registerId) const {
    for (auto e = this->begin(); e != this->end(); e++) {
      if (e->registerId == registerId)
        return &(*e);
    }
    
    return nullptr;
  }
  
  
  const DxbcSgnEntry* DxbcIsgn::find(
    const std::string& semanticName,
          uint32_t     semanticIndex) const {
    for (auto e = this->begin(); e != this->end(); e++) {
      if (e->semanticIndex == semanticIndex
       && compareSemanticNames(semanticName, e->semanticName))
        return &(*e);
    }
    
    return nullptr;
  }
  
  
  bool DxbcIsgn::compareSemanticNames(
    const std::string& a, const std::string& b) const {
    if (a.size() != b.size())
      return false;
    
    for (size_t i = 0; i < a.size(); i++) {
      if (std::toupper(a[i]) != std::toupper(b[i]))
        return false;
    }
    
    return true;
  }
  
}