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
#include "NutrientBasedContactInhibitionCellCycleModel.hpp"

#include "WntConcentration.hpp"

// Make a boundary condition at the base - maybe at the sides?
#include "PlaneBoundaryCondition.hpp"

// Modifiers
#include "VolumeTrackingModifier.hpp"

// Misc
#include "FakePetscSetup.hpp"
#include "Debug.hpp"


class TestYeastColumn : public AbstractCellBasedTestSuite
{
	public:
	void xTestColumnFixedNutrientGradient() throw(Exception)
	{



		// Make the Wnt concentration for tracking cell position so division can be turned off
		WntConcentration<2>* p_wnt = WntConcentration<2>::Instance();
		p_wnt->SetType(LINEAR);
		p_wnt->SetCryptLength(10);
		p_wnt->SetWntConcentrationParameter(1); // Scales the distance to a fraction

	
		std::vector<Node<2>*> nodes;
		std::vector<unsigned> transit_nodes;
		std::vector<unsigned> location_indices;

		unsigned node_counter = 0;

		double dt = 0.001;

		double end_time = 100;
		
		double sampling_multiple = 1;

		double maxInteractionRadius = 2.0;

		unsigned n = 3;

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
			p_cycle_model->SetSimulationDt(dt);
			
			CellPtr p_cell(new Cell(p_state, p_cycle_model));
			p_cell->SetCellProliferativeType(p_trans_type);

			cells.push_back(p_cell);
		}

		NodeBasedCellPopulation<2> cell_population(mesh, cells, location_indices);
		p_wnt->SetCellPopulation(cell_population);


		OffLatticeSimulation<2> simulator(cell_population);

		simulator.SetOutputDirectory("TestColumnFixedNutrientGradient");

		simulator.SetEndTime(end_time);
		simulator.SetDt(dt);
		simulator.SetSamplingTimestepMultiple(sampling_multiple);


		MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
		p_force->SetMeinekeSpringStiffness(100);

		simulator.AddForce(p_force);

		c_vector<double,2> point1;
		c_vector<double,2> normal1;
		point1(0) = 0;
		point1(1) = 0;

		normal1(0) = 0;
		normal1(1) = -1;

		MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc1, (&cell_population,point1,normal1));
		p_bc1->SetUseJiggledNodesOnPlane(true);
		simulator.AddCellPopulationBoundaryCondition(p_bc1);

		c_vector<double,2> point2;
		c_vector<double,2> normal2;
		point2(0) = 0;
		point2(1) = 0;

		normal2(0) = -1;
		normal2(1) = 0;

		MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc2, (&cell_population,point2,normal2));
		p_bc2->SetUseJiggledNodesOnPlane(true);
		simulator.AddCellPopulationBoundaryCondition(p_bc2);

		c_vector<double,2> point3;
		c_vector<double,2> normal3;
		point3(0) = n;
		point3(1) = 0;

		normal3(0) = 1;
		normal3(1) = 0;

		MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc3, (&cell_population,point3,normal3));
		p_bc3->SetUseJiggledNodesOnPlane(true);
		simulator.AddCellPopulationBoundaryCondition(p_bc3);

		MAKE_PTR(VolumeTrackingModifier<2>, p_mod);
		simulator.AddSimulationModifier(p_mod);


		simulator.Solve();
		WntConcentration<2>::Destroy();


	};

	void TestColumnFixedNutrientGradientContactInhibition() throw(Exception)
	{



		// Make the Wnt concentration for tracking cell position so division can be turned off
		WntConcentration<2>* p_wnt = WntConcentration<2>::Instance();
		p_wnt->SetType(LINEAR);
		p_wnt->SetCryptLength(10);
		p_wnt->SetWntConcentrationParameter(1); // Scales the distance to a fraction

	
		std::vector<Node<2>*> nodes;
		std::vector<unsigned> transit_nodes;
		std::vector<unsigned> location_indices;

		unsigned node_counter = 0;

		double dt = 0.001;

		double end_time = 100;
		
		double sampling_multiple = 1;

		double maxInteractionRadius = 2.0;

		unsigned n = 3;

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
			NutrientBasedContactInhibitionCellCycleModel* p_cycle_model = new NutrientBasedContactInhibitionCellCycleModel();
			p_cycle_model->SetSimulationDt(dt);
			
			CellPtr p_cell(new Cell(p_state, p_cycle_model));
			p_cell->SetCellProliferativeType(p_trans_type);

			cells.push_back(p_cell);
		}

		NodeBasedCellPopulation<2> cell_population(mesh, cells, location_indices);
		p_wnt->SetCellPopulation(cell_population);


		OffLatticeSimulation<2> simulator(cell_population);

		simulator.SetOutputDirectory("TestColumnFixedNutrientGradientContactInhibition");

		simulator.SetEndTime(end_time);
		simulator.SetDt(dt);
		simulator.SetSamplingTimestepMultiple(sampling_multiple);


		MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
		p_force->SetMeinekeSpringStiffness(100);

		simulator.AddForce(p_force);

		c_vector<double,2> point1;
		c_vector<double,2> normal1;
		point1(0) = 0;
		point1(1) = 0;

		normal1(0) = 0;
		normal1(1) = -1;

		MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc1, (&cell_population,point1,normal1));
		p_bc1->SetUseJiggledNodesOnPlane(true);
		simulator.AddCellPopulationBoundaryCondition(p_bc1);

		c_vector<double,2> point2;
		c_vector<double,2> normal2;
		point2(0) = 0;
		point2(1) = 0;

		normal2(0) = -1;
		normal2(1) = 0;

		MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc2, (&cell_population,point2,normal2));
		p_bc2->SetUseJiggledNodesOnPlane(true);
		simulator.AddCellPopulationBoundaryCondition(p_bc2);

		c_vector<double,2> point3;
		c_vector<double,2> normal3;
		point3(0) = n;
		point3(1) = 0;

		normal3(0) = 1;
		normal3(1) = 0;

		MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_bc3, (&cell_population,point3,normal3));
		p_bc3->SetUseJiggledNodesOnPlane(true);
		simulator.AddCellPopulationBoundaryCondition(p_bc3);

		MAKE_PTR(VolumeTrackingModifier<2>, p_mod);
		simulator.AddSimulationModifier(p_mod);


		simulator.Solve();
		WntConcentration<2>::Destroy();


	};

};