// Copyright 2020 Caoyang Jiang

#include <array>
#include <cstdint>
#include <memory>

namespace i264 {
class YuvImage
{
public:
  YuvImage(int width, int height);
  YuvImage(int width, int height, const uint8_t* buffer);

  void CopyYUV(uint8_t* source);
  void CopyY(uint8_t* source);
  void CopyU(uint8_t* source);
  void CopyV(uint8_t* source);

  int GetWidth() const;
  int GetHeight() const;

  const uint8_t* GetY(int x, int y) const;
  const uint8_t* GetU(int x, int y) const;
  const uint8_t* GetV(int x, int y) const;

  std::array<uint8_t*, 3> GetYuv(int x, int y) const;

private:
  std::shared_ptr<uint8_t[]> buffer_ = nullptr;
  int width_ = 0;
  int height_ = 0;
};
}
