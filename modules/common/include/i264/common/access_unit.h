/**
 * @file access_unit.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_ACCESS_UNIT_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_ACCESS_UNIT_H_

#include "i264/common/types.h"

namespace i264 {

struct NALUnit {
  NalUnitType type;

  bool IsSlice() const;
  bool IsSei() const;
  bool IsVcl() const;
  bool ISNonVcl() const;
};

struct OutputNALUnit : public NALUnit {
  BitStream bit_stream;
};

class AccessUnit {
 public:
  void Push(const OutputNALUnit& nal);
  void Push(OutputNALUnit&& nal);
  size_t Sze() const;

  // for supporting iterator access
  std::list<OutputNALUnit>::iterator begin();
  std::list<OutputNALUnit>::iterator end();
  std::list<OutputNALUnit>::const_iterator begin() const;
  std::list<OutputNALUnit>::const_iterator end() const;

 private:
  std::list<OutputNALUnit> nals_;
};
}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_ACCESS_UNIT_H_
