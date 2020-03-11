/**
 * @file mb_encoder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

namespace i264 {

class MacroblockEncoder {
 public:
  void MacroblockEncoder(const SliceEncoder& slice_encoder);
  void Encode(Macroblock& macroblock);
  void Code(Macroblock& macroblock);

 private:
  const SliceEncoder& slice_encoder_;
};
}  // namespace i264
