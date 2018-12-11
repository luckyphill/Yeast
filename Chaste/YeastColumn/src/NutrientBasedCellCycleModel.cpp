/*

Created by Phill for the YeastColumn project

*/

#include "NutrientBasedCellCycleModel.hpp"

NutrientBasedCellCycleModel::NutrientBasedCellCycleModel()
    : mBirthTime(SimulationTime::Instance()->GetTime()),
      mReadyToDivide(false),
      mDimension(UNSIGNED_UNSET)
{
}

NutrientBasedCellCycleModel::~NutrientBasedCellCycleModel()
{
}

NutrientBasedCellCycleModel::NutrientBasedCellCycleModel(const NutrientBasedCellCycleModel& rModel)
    : mBirthTime(rModel.mBirthTime),
      mReadyToDivide(rModel.mReadyToDivide),
      mDimension(rModel.mDimension)
{
}

void NutrientBasedCellCycleModel::Initialise()
{
}

void NutrientBasedCellCycleModel::InitialiseDaughterCell()
{
}

void NutrientBasedCellCycleModel::SetCell(CellPtr pCell)
{
    mpCell = pCell;
}

CellPtr NutrientBasedCellCycleModel::GetCell()
{
    assert(mpCell != nullptr);
    return mpCell;
}

void NutrientBasedCellCycleModel::SetBirthTime(double birthTime)
{
    mBirthTime = birthTime;
}

double NutrientBasedCellCycleModel::GetBirthTime() const
{
    return mBirthTime;
}

double NutrientBasedCellCycleModel::GetAge()
{
    return SimulationTime::Instance()->GetTime() - mBirthTime;
}

void NutrientBasedCellCycleModel::ResetForDivision()
{
    assert(mReadyToDivide);
    mReadyToDivide = false;
    mBirthTime = SimulationTime::Instance()->GetTime();
}

void NutrientBasedCellCycleModel::SetDimension(unsigned dimension)
{
    if (dimension != 1 && dimension !=2 && dimension != 3 && dimension != UNSIGNED_UNSET)
    {
        EXCEPTION("Dimension must be 1, 2, 3 or UNSIGNED_UNSET");
    }
    mDimension = dimension;
}

unsigned NutrientBasedCellCycleModel::GetDimension() const
{
    return mDimension;
}


void NutrientBasedCellCycleModel::OutputCellCycleModelInfo(out_stream& rParamsFile)
{
    std::string cell_cycle_model_type = GetIdentifier();

    *rParamsFile << "\t\t<" << cell_cycle_model_type << ">\n";
    OutputCellCycleModelParameters(rParamsFile);
    *rParamsFile << "\t\t</" << cell_cycle_model_type << ">\n";
}

void NutrientBasedCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
 // None to output
}


NutrientBasedCellCycleModel* NutrientBasedCellCycleModel::CreateCellCycleModel()
{
    return new NutrientBasedCellCycleModel(*this);
}

bool NutrientBasedCellCycleModel::ReadyToDivide()
{
    // Random number
    // Wnt concentration
    // If random number < WntConcentration and age > 1 then divide
    if (RandomNumberGenerator::Instance()->ranf() > WntConcentration<2>::Instance()->GetWntLevel(mpCell) && mpCell->GetAge() > 1 )
    {
        return true;
    }

    return false;
}