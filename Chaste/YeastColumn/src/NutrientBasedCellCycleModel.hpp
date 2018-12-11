/*

Created by Phill for the YeastColumn project

*/

#ifndef NutrientBasedCellCycleModel_HPP_
#define NutrientBasedCellCycleModel_HPP_

#include "ChasteSerialization.hpp"
#include "ClassIsAbstract.hpp"
#include "Identifiable.hpp"

#include <boost/serialization/base_object.hpp>

#include <vector>

#include "OutputFileHandler.hpp"
#include "CellCyclePhases.hpp"
#include "SimulationTime.hpp"
#include "Cell.hpp"

#include "WntConcentration.hpp"
#include "RandomNumberGenerator.hpp"

class Cell; // Circular definition (cells need to know about cycle models and vice-versa)
typedef boost::shared_ptr<Cell> CellPtr;

/**
 * The NutrientBasedCellCycleModel contains basic information to all cell-cycle models.
 * It handles assignment of birth time, and associates the model with a Cell.
 *
 * Cell-cycle models are noncopyable since cells are noncopyable.
 */
class NutrientBasedCellCycleModel : public Identifiable
{
private:

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Archive the object and its member variables.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        // Make sure the SimulationTime singleton gets saved too
        SerializableSingleton<SimulationTime>* p_time_wrapper = SimulationTime::Instance()->GetSerializationWrapper();
        archive & p_time_wrapper;

        // DO NOT archive & mpCell; -- The CellCycleModel is only ever archived from the Cell
        // which knows this and it is handled in the load_construct of Cell.
        archive & mBirthTime;
        archive & mReadyToDivide;
        archive & mDimension;
    }

    /**
     * Prevent copy-assignment of this class, or its subclasses.
     * Note that we do not define this method, therefore statements like "NutrientBasedCellCycleModel new = old;" will not compile.
     * We do not inherit from boost::noncopyable because we *do* define a protected copy-constructor, for use by CreateCellCycleModel.
     *
     * @return the new cell cycle model.
     */
    NutrientBasedCellCycleModel& operator=(const NutrientBasedCellCycleModel&);

protected:

    /** The cell that this model is associated with. */
    CellPtr mpCell;

    /**
     * The time that the cell began to split from its parent
     * (i.e. @b beginning of M phase NOT the end)
     */
    double mBirthTime;

    /**
     * Whether the cell is currently ready to undergo division.
     */
    bool mReadyToDivide;

    /**
     * Spatial dimension being used in simulation (defaults to 0, set with SetDimension).
     */
    unsigned mDimension;

    /**
     * Protected copy-constructor for use by CreateCellCycleModel.
     * The only way for external code to create a copy of a cell cycle model
     * is by calling that method, to ensure that a model of the correct subclass is created.
     * This copy-constructor helps subclasses to ensure that all member variables are correctly copied when this happens.
     *
     * This method is called by child classes to set member variables for a daughter cell upon cell division.
     * Note that the parent cell cycle model will have had ResetForDivision() called just before CreateCellCycleModel() is called,
     * so performing an exact copy of the parent is suitable behaviour. Any daughter-cell-specific initialisation
     * can be done in InitialiseDaughterCell().
     *
     * @param rModel the cell cycle model to copy.
     */
    NutrientBasedCellCycleModel(const NutrientBasedCellCycleModel& rModel);

public:

    /**
     * Sets up a new NutrientBasedCellCycleModel, gives it a birth time of the
     * present simulation time (which is overwritten by some subclasses)
     */
    NutrientBasedCellCycleModel();

    /**
     * Base class with virtual methods needs a virtual destructor. The destructor
     * does not delete mpCell. Instead, the cell takes responsibility for deleting
     * the cell-cycle model when it is destroyed.
     */
    virtual ~NutrientBasedCellCycleModel();

    /**
     * Gives the cell-cycle model a pointer to its host cell.
     *
     * Some cell-cycle models pass this pointer to other classes,
     * which use this information to determine other information based upon the location
     * of the cell (e.g. the Wnt concentration at this location).
     *
     * @param pCell pointer to the cell
     */
    void SetCell(CellPtr pCell);

    /**
     * Initialise the cell-cycle model at the start of a simulation.
     *
     * This method will be called precisely once per cell set up in the initial
     * cell population. It is not called on cell division; use ResetForDivision(),
     * CreateCellCycleModel() and InitialiseDaughterCell() for that.
     *
     * By the time this is called, a CellPopulation will have been set up, so the model
     * can know where its cell is located in space. If relevant to the simulation,
     * any singletons will also have been initialised.
     */
    virtual void Initialise();

    /**
     * Initialise the new daughter cell's cycle model after a cell division.
     *
     * This is called by Cell::Divide once the new cell object
     * has been fully created, to perform any initialisation of the
     * cell cycle which requires access to the cell.
     *
     * Note that much initialisation can be performed using the
     * combination of ResetForDivision() (called on the parent prior to
     * division) and CreateCellCycleModel() (called on the reset
     * parent to create the new cell-cycle model object).
     */
    virtual void InitialiseDaughterCell();

    /**
     * @return The cell which plays host to this cell-cycle model.
     */
    CellPtr GetCell();

    /**
     * Set the cell's time of birth (usually not required as it should be inside
     * the individual cell-cycle-model-constructor, but useful for tests).
     *
     * @param birthTime the simulation time at this cell's birth.
     *
     * (This function is overridden in  ODE Based Cell Cycle Models).
     */
    virtual void SetBirthTime(double birthTime);

    /**
     * Set the spatial dimension.
     *
     * @param dimension
     */
    void SetDimension(unsigned dimension);

    /**
     * @return the dimension this cell-cycle model thinks the simulation is in.
     */
    unsigned GetDimension() const;

    /**
     * @return the time at which the cell was born.
     */
    double GetBirthTime() const;

    /**
     * @return the cell's age.
     */
    double GetAge();

    /**
     * @return whether the cell is ready to divide.
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * The intention is that this method is called precisely once at
     * each timestep of the simulation. However this does not appear
     * to always be the case at present, and so it can cope with more
     * unusual usage patterns.
     */
    virtual bool ReadyToDivide();

    /**
     * Each cell-cycle model must be able to be reset 'after' a cell division.
     *
     * Actually, this method is called from Cell::Divide() to
     * reset the cell cycle just before the daughter cell is created.
     * CreateCellCycleModel() can then clone our state to generate a
     * cell-cycle model instance for the daughter cell.
     */
    virtual void ResetForDivision();

    /**
     * Builder method to create new instances of the cell-cycle model.
     * Each concrete subclass must implement this method to create an
     * instance of that subclass.
     *
     * This method is called by Cell::Divide() to create a cell
     * cycle model for the daughter cell.  Note that the parent cell
     * cycle model will have had ResetForDivision() called just before
     * CreateCellCycleModel() is called, so performing an exact copy of the
     * parent is suitable behaviour. Any daughter-cell-specific initialisation
     * can be done in InitialiseDaughterCell().
     *
     * Copy constructors are used to set all the member variables in the appropriate classes.
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @return new cell-cycle model
     */
    virtual NutrientBasedCellCycleModel* CreateCellCycleModel();

    /**
     * Outputs cell-cycle model used in the simulation to file and then calls
     * OutputCellCyclemodelParameters to output all relevant parameters.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputCellCycleModelInfo(out_stream& rParamsFile);

    /**
     * Outputs cell cycle model parameters to file.
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    virtual void OutputCellCycleModelParameters(out_stream& rParamsFile);
};

CLASS_IS_ABSTRACT(NutrientBasedCellCycleModel)

#endif /*NutrientBasedCellCycleModel_HPP_*/
