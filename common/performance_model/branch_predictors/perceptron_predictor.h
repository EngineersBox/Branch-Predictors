#ifndef PERCEPTRON_PREDICTOR_H
#define PERCEPTRON_PREDICTOR_H

#include "branch_predictor.h"
#include "dyn_int.h"

#include <vector>

#define B 6

class PerceptronPredictor: public BranchPredictor {
public:

   PerceptronPredictor(String name, core_id_t core_id, UInt32 size, UInt32 perceptrons, UInt32 bhrSize, UInt32 threshold);

   virtual bool predict(bool indirect, IntPtr ip, IntPtr target) override;

   virtual void update(bool predicted, bool actual, bool indirect, IntPtr ip, IntPtr target) override;

private:
   std::vector<std::vector<Int<B>>> weights;
   std::vector<Int<2>> bhr;
   Int<B> yOut;

   UInt32 threshold;

   inline UInt32 calculateWeightTableIndex(IntPtr ip);

};

#endif
