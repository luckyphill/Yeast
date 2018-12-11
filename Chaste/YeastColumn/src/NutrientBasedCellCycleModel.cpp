/*

Created by Phill for the YeastColumn project

*/

#include "NutrientBasedCellCycleModel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "Debug.hpp"

NutrientBasedCellCycleModel::NutrientBasedCellCycleModel()
    : AbstractSimpleCellCycleModel()
{
}

NutrientBasedCellCycleModel::NutrientBasedCellCycleModel(const NutrientBasedCellCycleModel& rModel)
   : AbstractSimpleCellCycleModel(rModel)
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
    PRINT_VARIABLE(0.1 * WntConcentration<2>::Instance()->GetWntLevel(mpCell))
    PRINT_VARIABLE(mpCell->GetCellId())
    if (RandomNumberGenerator::Instance()->ranf() < 0.1 * WntConcentration<2>::Instance()->GetWntLevel(mpCell) && mpCell->GetAge() > 1 )
    {   
        TRACE("Yes divide")
        mReadyToDivide = true;
    } else 
    {
        TRACE("No divide")
    }

    return mReadyToDivide;
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
