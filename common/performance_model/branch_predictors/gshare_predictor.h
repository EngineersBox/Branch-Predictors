#ifndef GSHARE_PREDICTOR_H
#define GSHARE_PREDICTOR_H

#include "branch_predictor.h"

#include <vector>

#include "global_predictor.h"
#include "saturating_predictor.h"

class GsharePredictor : public BranchPredictor
{
public:
   GsharePredictor(String name, core_id_t core_id, UInt32 size);

   virtual bool predict(bool indirect, IntPtr ip, IntPtr target) override;

   virtual void update(bool predicted, bool actual, bool indirect, IntPtr ip, IntPtr target) override;
   
private:
   using DualSaturatingPredictor = SaturatingPredictor<2>;
   std::vector<DualSaturatingPredictor> pht;
   UInt32 bhr;
   UInt32 mask;

   inline DualSaturatingPredictor getPHTEntry(IntPtr ip);
};

#endif
