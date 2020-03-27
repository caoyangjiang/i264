/**
 * @file cabac_coder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-26
 *
 * @copyright Copyright (c) 2020
 *
 */

namespace i264 {
class CabacCoder {
 public:
  void CodeMbType(const Macroblock& macroblock, BitStreamWriter& bitstream);
  void CodeMbQpDelta(const Macroblock& macroblock, BitStreamWriter& bitstream);
  void CodeIntraChromaPredMode(const Macroblock& macroblock,
                               BitStreamWriter& bitstream);
  void CodeCodedBlockFlag(const Macroblock& macroblock,
                          BitStreamWriter& writer);
  void CodeSignificantCoeffFlag(const Macroblock& macroblock,
                                BitStreamWriter& writer);
  void CodeLastSignificantCoeffFlag(const Macroblock& macroblock,
                                    BitStreamWriter& writer);
  void CodeCoeffAbsLevelMinus1(const Macroblock& macroblock,
                               BitStreamWriter& writer);
  void CoedCoeffSignFlag(const Macroblock& macroblock, BitStreamWriter& writer);
};

}  // namespace i264
