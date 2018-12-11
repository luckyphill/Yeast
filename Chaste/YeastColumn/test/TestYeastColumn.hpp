// Standard includes for tests
#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "CheckpointArchiveTypes.hpp"
#include "SmartPointers.hpp"
#include "CommandLineArguments.hpp"

// Simulators and output
#include "OffLatticeSimulation.hpp" //Simulates the evolution of the population

// Forces
#include "GeneralisedLinearSpringForce.hpp"

#include "NodesOnlyMesh.hpp"
#include "CellsGenerator.hpp"

#include "NodeBasedCellPopulation.hpp"

#include "NutrientBasedCellCycleModel.hpp"

#include "WntConcentration.hpp"

// Make a boundary condition at the base - maybe at the sides?

// Misc
#include "FakePetscSetup.hpp"
#include "Debug.hpp"


class TestYeastColumn : public AbstractCellBasedTestSuite
{
	public:
	void TestColumnFixedNutrientGradient() throw(Exception)
	{



		// Make the Wnt concentration for tracking cell position so division can be turned off
		WntConcentration<2>* p_wnt = WntConcentration<2>::Instance();
		p_wnt->SetType(LINEAR);
		p_wnt->SetCryptLength(20);
		p_wnt->SetWntConcentrationParameter(20); // Scales the distance to a fraction

	
		std::vector<Node<2>*> nodes;
		std::vector<unsigned> transit_nodes;
		std::vector<unsigned> location_indices;

		unsigned node_counter = 0;

		double dt = 0.01;

		double end_time = 10;
		
		double sampling_multiple = 10;

		double maxInteractionRadius = 2.0;

		unsigned n = 5;

		double x;
		double y;

		for(unsigned i=1; i<=n; i++)
		{
			y = 0;
			x = double(i);
			Node<2>* single_node_2 =  new Node<2>(node_counter,  false,  x, y);
			nodes.push_back(single_node_2);
			transit_nodes.push_back(node_counter);
			location_indices.push_back(node_counter);
			node_counter++;
		}

		NodesOnlyMesh<2> mesh;
		mesh.ConstructNodesWithoutMesh(nodes, maxInteractionRadius);

		std::vector<CellPtr> cells;

		MAKE_PTR(TransitCellProliferativeType, p_trans_type);
		MAKE_PTR(WildTypeCellMutationState, p_state);

		for(unsigned i=1; i<=n; i++)
		{
			NutrientBasedCellCycleModel* p_cycle_model = new NutrientBasedCellCycleModel();
			
			CellPtr p_cell(new Cell(p_state, p_cycle_model));
			p_cell->SetCellProliferativeType(p_trans_type);

			cells.push_back(p_cell);
		}

		NodeBasedCellPopulation<2> cell_population(mesh, cells, location_indices);
		p_wnt->SetCellPopulation(cell_population);


		OffLatticeSimulation simulator(cell_population);

		simulator.SetOutputDirectory('TestColumnFixedNutrientGradient');

		simulator.SetEndTime(end_time);
		simulator.SetDt(dt);
		simulator.SetSamplingTimestepMultiple(sampling_multiple);


		MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);

		simulator.AddForce(p_force);

		simulator.Solve();
		WntConcentration<2>::Destroy();


	};

};