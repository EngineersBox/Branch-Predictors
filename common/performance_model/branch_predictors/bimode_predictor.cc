#include "simulator.h"
#include "bimode_predictor.h"

#include <cmath>
#include <bitset>

static inline UInt32 calculateBitMask(UInt32 size) {
   return (1 << (UInt32)(std::log2((size * 8) / 2))) - 1;
}

BiModePredictor::BiModePredictor(String name, core_id_t core_id, UInt32 size, float phtRatio, UInt32 bhrSize):
   BranchPredictor(name, core_id),
   takenPHT((size * (1 - phtRatio)) / 2),
   untakenPHT((size * (1 - phtRatio)) / 2),
   choicePHT((size * phtRatio) / 2),
   addressMask(calculateBitMask((size * phtRatio) / 2)),
   bhr(0),
   bhrMask(calculateBitMask(bhrSize)) {
}

UInt32 BiModePredictor::calculateDirectionIndex(IntPtr ip) {
   return ((this->bhr & this->bhrMask) ^ (ip & this->addressMask)) % this->takenPHT.size();
}

UInt32 BiModePredictor::calculateChoiceIndex(IntPtr ip) {
   return (ip & this->addressMask) % this->choicePHT.size();
}

BiModePredictor::DualSaturatingPredictor BiModePredictor::getChoiceEntry(IntPtr ip) {
   return this->choicePHT[calculateChoiceIndex(ip)];
}

bool BiModePredictor::predict(bool indirect, IntPtr ip, IntPtr target) {
   BiModePredictor::DualSaturatingPredictor choiceEntry = this->getChoiceEntry(ip);
   BiModePredictor::DualSaturatingPredictor directionEntry;
   if (choiceEntry.predict()) {
      directionEntry = this->takenPHT[calculateDirectionIndex(ip)];
   } else {
      directionEntry = this->untakenPHT[calculateDirectionIndex(ip)];
   }
   return directionEntry.predict();
}

void BiModePredictor::update(bool predicted, bool actual, bool indirect, IntPtr ip, IntPtr target) {
   updateCounters(predicted, actual);
   BiModePredictor::DualSaturatingPredictor choiceEntry = this->getChoiceEntry(ip);
   BiModePredictor::DualSaturatingPredictor directionEntry;
   if (choiceEntry.predict()) {
      this->takenPHT[calculateDirectionIndex(ip)].update(actual);
   } else {
      this->untakenPHT[calculateDirectionIndex(ip)].update(actual);
   }
   this->bhr = (this->bhr << 1) | actual;
}
