#include "perceptron_predictor.h"

#include "simulator.h"

#include <string>
#include <cmath>
#include <stdexcept>
#include <memory>
#include <algorithm>

PerceptronPredictor::PerceptronPredictor(String name, core_id_t core_id, UInt32 size, UInt32 perceptrons, UInt32 bhrSize, UInt32 threshold):
   BranchPredictor(name, core_id),
   weights(perceptrons),
   bhr(bhrSize),
   yOut(),
   threshold(threshold) {
      this->yOut.data = 0;
      UInt32 projectedSize = (B) * (bhrSize + 1) * perceptrons;
      if ((projectedSize/8) > size) {
         throw std::runtime_error("Size is too large for budget");
      }
      this->bhr = std::vector<Int<2>>(projectedSize, Int<2>(-1));
      this->weights = std::vector<std::vector<Int<B>>>(perceptrons, std::vector<Int<B>>(bhrSize + 1));
}

inline UInt32 PerceptronPredictor::calculateWeightTableIndex(IntPtr ip) {
   return ip % this->weights.size();
}

bool PerceptronPredictor::predict(bool indirect, IntPtr ip, IntPtr target) {
   std::vector<Int<B>> weightVector = this->weights[this->calculateWeightTableIndex(ip)];
   this->yOut.data = weightVector[0].data;
   for (UInt32 i = 1; i < weightVector.size(); i++) {
      this->yOut += weightVector[i] * this->bhr[i - 1];
   }
   return this->yOut.data >> B == 0;
}

void PerceptronPredictor::update(bool predicted, bool actual, bool indirect, IntPtr ip, IntPtr target) {
   updateCounters(predicted, actual);
   if (predicted == actual && std::abs(this->yOut.data) > this->threshold) {
      return;
   }
   UInt32 index = this->calculateWeightTableIndex(ip);
   this->weights[index][0] += (SInt32) (actual ? 1 : -1);
   for (UInt32 i = 1; i < this->weights[index].size(); i++) {
      this->weights[index][i] += (this->bhr[i - 1] * (actual ? 1 : -1));
   }
   this->bhr.pop_back();
   this->bhr.insert(this->bhr.begin(), Int<2> bhrEntry(actual ? 1 : -1));
}
