#include "simulator.h"
#include "gshare_predictor.h"

#include <cmath>

GsharePredictor::GsharePredictor(String name, core_id_t core_id, UInt32 size):
   BranchPredictor(name, core_id),
   pht((size * 8) / 2),
   bhr(0),
   mask((1 << (UInt32)(std::log2((size * 8) / 2))) - 1) {}

GsharePredictor::DualSaturatingPredictor GsharePredictor::getPHTEntry(IntPtr ip) {
   return this->pht[(this->bhr & this->mask) ^ (ip & this->mask)];
}

bool GsharePredictor::predict(bool indirect, IntPtr ip, IntPtr target) {
   GsharePredictor::DualSaturatingPredictor predictor = this->getPHTEntry(ip);
   return predictor.predict();
}

void GsharePredictor::update(bool predicted, bool actual, bool indirect, IntPtr ip, IntPtr target) {
   updateCounters(predicted, actual);
   GsharePredictor::DualSaturatingPredictor predictor = this->getPHTEntry(ip);
   predictor.update(actual);
   this->bhr = (this->bhr < 1) | actual;
}
