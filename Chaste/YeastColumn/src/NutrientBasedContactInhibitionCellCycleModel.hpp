/*

Created by Phill for the YeastColumn project

*/

#ifndef NutrientBasedContactInhibitionCellCycleModel_HPP_
#define NutrientBasedContactInhibitionCellCycleModel_HPP_

#include "AbstractSimpleCellCycleModel.hpp"
#include "RandomNumberGenerator.hpp"
#include "WntConcentration.hpp"
#include "NutrientBasedCellCycleModel.hpp"

/**
 * A stochastic cell-cycle model where cells divide with probability dependent on the nutrient concentration
 *
 * 
 */
class NutrientBasedContactInhibitionCellCycleModel : public NutrientBasedCellCycleModel
{
private:

	double mQuiescentFraction = 0.88;
	double mEquilibriumVolume = M_PI * 0.5 * 0.5;

protected:

    NutrientBasedContactInhibitionCellCycleModel(const NutrientBasedContactInhibitionCellCycleModel& rModel);

public:

    /**
     * Constructor - just a default, mBirthTime is set in the AbstractCellCycleModel class.
     */
    NutrientBasedContactInhibitionCellCycleModel();

    //virtual ~NutrientBasedContactInhibitionCellCycleModel();

    AbstractCellCycleModel* CreateCellCycleModel();

    // Overridden method
    virtual bool ReadyToDivide();

    virtual void OutputCellCycleModelParameters(out_stream& rParamsFile);

    void SetQuiessentFraction(double fraction);

    void SetEquilibriumVolume(double volume);

    };

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(NutrientBasedContactInhibitionCellCycleModel)

#endif /*NutrientBasedContactInhibitionCellCycleModel_HPP_*/