/**
 * @file distortion.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-18
 *
 * @copyright Copyright (c) 2020
 *
 */

namespace i264 {

class Distortion {
 public:
  template <int BLOCK_WIDTH, int BLOCK_HEIGHT>
  static uint32_t ComputeBlockSad(
      const Array2D<BLOCK_WIDTH, BLOCK_HEIGHT>& current_block,
      const Array2D<BLOCK_WIDTH, BLOCK_HEIGHT>& reference_block,
      int lambda = 1);
};

template <int BLOCK_WIDTH, int BLOCK_HEIGHT>
uint32_t Distortion::uint32_t ComputeBlockSad(
    const Array2D<BLOCK_WIDTH, BLOCK_HEIGHT>& current_block,
    const Array2D<BLOCK_WIDTH, BLOCK_HEIGHT>& reference_block, int lambda = 1) {
  uint32_t sad = 0;
  for (int i = 0; i < BLOCK_HEIGHT; i++) {
    for (int j = 0; j < BLOCK_WIDTH; j++) {
      sad += static_cast<uint32_t>(
          std::abs(current_block[i][j] - reference_block[i][j]))
    }
  }
}

}  // namespace i264
