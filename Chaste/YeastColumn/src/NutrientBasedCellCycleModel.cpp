/*

Created by Phill for the YeastColumn project

*/

#include "NutrientBasedCellCycleModel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "Debug.hpp"

NutrientBasedCellCycleModel::NutrientBasedCellCycleModel()
    : AbstractSimpleCellCycleModel(),
    mDt(DOUBLE_UNSET)
{
}

NutrientBasedCellCycleModel::NutrientBasedCellCycleModel(const NutrientBasedCellCycleModel& rModel)
   : AbstractSimpleCellCycleModel(rModel),
   mDt(rModel.mDt)
{

}

AbstractCellCycleModel* NutrientBasedCellCycleModel::CreateCellCycleModel()
{
    return new NutrientBasedCellCycleModel(*this);
}

void NutrientBasedCellCycleModel::SetCellCycleDuration()
{
    // Not used
    mCellCycleDuration = DBL_MAX;
}

bool NutrientBasedCellCycleModel::ReadyToDivide()
{
    // Random number
    // Wnt concentration
    // If random number < WntConcentration and age > 1 then divide
    
    if (RandomNumberGenerator::Instance()->ranf() < GetSimulationDt() * WntConcentration<2>::Instance()->GetWntLevel(mpCell) && mpCell->GetAge() > 1 )
    {   
        mReadyToDivide = true;
    } else 
    {
        mReadyToDivide = false;
    }

    return mReadyToDivide;
}

double NutrientBasedCellCycleModel::GetSimulationDt()
{
    assert(mDt != DOUBLE_UNSET);
    return mDt;
}

void NutrientBasedCellCycleModel::SetSimulationDt(double dt)
{
    mDt = dt;
}

double NutrientBasedCellCycleModel::GetAverageTransitCellCycleTime()
{
    // Not used
    return 1;
}

double NutrientBasedCellCycleModel::GetAverageStemCellCycleTime()
{
    // Not used
    return 1;
}

void NutrientBasedCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    AbstractSimpleCellCycleModel::OutputCellCycleModelParameters(rParamsFile);

}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(NutrientBasedCellCycleModel)
