#include "NutrientBasedContactInhibitionCellCycleModel.hpp"
#include "Debug.hpp"

NutrientBasedContactInhibitionCellCycleModel::NutrientBasedContactInhibitionCellCycleModel()
    : NutrientBasedCellCycleModel()
{
	mDt = DOUBLE_UNSET;
}

NutrientBasedContactInhibitionCellCycleModel::NutrientBasedContactInhibitionCellCycleModel(const NutrientBasedContactInhibitionCellCycleModel& rModel)
   : NutrientBasedCellCycleModel(rModel)
{
	mDt = rModel.mDt;
}

AbstractCellCycleModel* NutrientBasedContactInhibitionCellCycleModel::CreateCellCycleModel()
{
    return new NutrientBasedContactInhibitionCellCycleModel(*this);
}

bool NutrientBasedContactInhibitionCellCycleModel::ReadyToDivide()
{

	double cell_volume = mpCell->GetCellData()->GetItem("volume");
	double quiescent_volume = mEquilibriumVolume * mQuiescentFraction;

	if (cell_volume > quiescent_volume && RandomNumberGenerator::Instance()->ranf() < GetSimulationDt() * WntConcentration<2>::Instance()->GetWntLevel(mpCell) && mpCell->GetAge() > 1 )
    {   
        mReadyToDivide = true;
    } else 
    {
        mReadyToDivide = false;
    }

    return mReadyToDivide;
}

void NutrientBasedContactInhibitionCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
	AbstractSimpleCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

void NutrientBasedContactInhibitionCellCycleModel::SetQuiessentFraction(double fraction)
{
	mQuiescentFraction = fraction;
}

void NutrientBasedContactInhibitionCellCycleModel::SetEquilibriumVolume(double volume)
{
	mEquilibriumVolume = volume;
}