#ifndef BIMODE_PREDICTOR_H
#define BIMODE_PREDICTOR_H

#include "branch_predictor.h"
#include "saturating_predictor.h"
#include "global_predictor.h"

#include <vector>

class BiModePredictor : public BranchPredictor {
public:
   BiModePredictor(String name, core_id_t core_id, UInt32 size, float choiceRatio, UInt32 bhrSize);

   virtual bool predict(bool indirect, IntPtr ip, IntPtr target) override;

   virtual void update(bool predicted, bool actual, bool indirect, IntPtr ip, IntPtr target) override;

private:
   using DualSaturatingPredictor = SaturatingPredictor<2>;

   std::vector<DualSaturatingPredictor> takenPHT;
   std::vector<DualSaturatingPredictor> untakenPHT;
   std::vector<DualSaturatingPredictor> choicePHT;
   UInt32 addressMask;
   UInt32 bhr;
   UInt32 bhrMask;

   UInt32 calculateDirectionIndex(IntPtr ip);
   UInt32 calculateChoiceIndex(IntPtr ip);

   DualSaturatingPredictor getChoiceEntry(IntPtr ip);
};

#endif
