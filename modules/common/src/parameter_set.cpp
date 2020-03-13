/**
 * @file parameter_set.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-11
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/parameter_set.h"

#include <vector>

namespace i264 {
void ParameterSetManager::StoreSPS(const SPS& sps,
                                   const std::vector<uint8_t>& nalu_data) {
  sps_map_.SetParameterSet(sps.seq_parameter_set_id, sps, nalu_data);
}

void ParameterSetManager::StorePPS(const PPS& pps,
                                   const std::vector<uint8_t>& nalu_data) {
  pps_map_.SetParameterSet(pps.pic_parameter_set_id, pps, nalu_data);
}

SPS& ParameterSetManager::GetSPS(int sps_id) {
  return sps_map_.GetParameterSet(sps_id);
}

PPS& ParameterSetManager::GetPPS(int pps_id) {
  return pps_map_.GetParameterSet(pps_id);
}

const SPS& ParameterSetManager::GetSPS(int sps_id) const {
  return sps_map_.GetParameterSet(sps_id);
}

const PPS& ParameterSetManager::GetPPS(int pps_id) const {
  return pps_map_.GetParameterSet(pps_id);
}

SPS& ParameterSetManager::GetAnySPS() { return sps_map_.Front(); }

PPS& ParameterSetManager::GetAnyPPS() { return pps_map_.Front(); }

const SPS& ParameterSetManager::GetAnySPS() const { return sps_map_.Front(); }

const PPS& ParameterSetManager::GetAnyPPS() const { return pps_map_.Front(); }

SPS& ParameterSetManager::GetActiveSPS() {
  return sps_map_.GetParameterSet(active_sps_id_);
}

PPS& ParameterSetManager::GetActivePPS() {
  return pps_map_.GetParameterSet(active_pps_id_);
}

const SPS& ParameterSetManager::GetActiveSPS() const {
  return sps_map_.GetParameterSet(active_sps_id_);
}

const PPS& ParameterSetManager::GetActivePPS() const {
  return pps_map_.GetParameterSet(active_pps_id_);
}

void ParameterSetManager::SetActiveSPSId(int id) { active_sps_id_ = id; }

void ParameterSetManager::SetActivePPSId(int id) { active_pps_id_ = id; }

int ParameterSetManager::GetActiveSPSId() const { return active_sps_id_; }

int ParameterSetManager::GetActivePPSId() const { return active_pps_id_; }

}  // namespace i264
