#!env python3
import unittest as unit
import toghap
import json 

class TestToghap(unit.TestCase):

    # Test sequences:
    def test_dynamicInitialization(self):
        dynamic= self.dynamic_initNetwork()
        self.dynamic_visitDependencies(dynamic)
        self.dynamic_initProbas(dynamic)
  
    def dynamic_initNetwork(self):
        diceDomain= range(1,7)
        actionDomain= ["roll", "keep"]
        stateVariables= { "H": range(3), "D1": diceDomain,
            "D2": diceDomain, "D3": diceDomain }
        actionVariables= { "A1": actionDomain, "A2": actionDomain, "A3": actionDomain  }
        transitionalVariables= { "ID1": diceDomain, "ID2": diceDomain, "ID3": diceDomain }

        dynamic= wd.Dynamic(stateVariables, actionVariables, transitionalVariables)

        self.assertEqual( str( dynamic.__class__ ), "<class 'wanda.dynamic.Dynamic'>" )

        self.assertEqual( dynamic.stateDimention(), 4 )
        self.assertEqual( dynamic.actionDimention(), 3 )
        self.assertEqual( dynamic.transitionalDimention(), 3 )
        self.assertEqual( dynamic.overallDimention(), 14 )

        self.assertEqual( dynamic.overallSize(), 725594112 )
        self.assertEqual( dynamic.varNames, ["H", "D1", "D2", "D3", "A1", "A2", "A3", "ID1", "ID2", "ID3", "H'", "D1'", "D2'", "D3'"] )

        #dynamic.setDependency( "ID1", ["D1", "A1"] )
        dynamic.node("ID1").setParents( ["D1", "A1"] )
        dynamic.node("ID2").setParents( ["D2", "A2"] )
        dynamic.node("ID3").setParents( ["D3", "A3"] )
        dynamic.node("H'").setParents( ["H", "A1", "A2", "A3"] )
        dynamic.node( "D1'" ).setParents( ["ID1", "ID2", "ID3"] )
        dynamic.node( "D2'" ).setParents( ["ID1", "ID2", "ID3"] )
        dynamic.node( "D3'" ).setParents( ["ID1", "ID2", "ID3"] )

        self.assertEqual( dynamic.networkAsDico(), { "H":[], "D1":[], "D2":[], "D3":[], "A1":[], "A2":[], "A3":[],
                                                    "ID1":["D1", "A1"], "ID2":["D2", "A2"], "ID3":["D3", "A3"],
                                                    "H'":["H", "A1", "A2", "A3"], "D1'":["ID1", "ID2", "ID3"], "D2'":["ID1", "ID2", "ID3"], "D3'":["ID1", "ID2", "ID3"] } )
        return dynamic

    def dynamic_visitDependencies(self, dynamic):
        self.assertEqual( dynamic.nodeIdFromName("D1'"), 11 )
        dep=  wd.core.WdDynamic_dependency(dynamic.model, 11)
        space= wd.core.WdDependency_parentSpace( dep )
        dim= wd.core.WdSpace_dimention(space)
        self.assertEqual( dim, 3 )
        state= wd.uintArray( dim, 0 )
        wd.core.WdSpace_state0(space, state)
        for d3 in range(6) :
            for d2 in range(6) :
                for d1 in range(6) :
                    stList= [ state[i] for i in range(dim) ]
                    self.assertEqual( stList, [d1, d2, d3] )
                    wd.core.WdSpace_nextState(space, state)
        self.assertEqual( wd.core.WdSpace_isEndState(space, state), 1 )

        config= []
        for d3 in range(1, 7) :
            for d2 in range(1, 7) :
                for d1 in range(1, 7) :
                    config.append( [ d1, d2, d3 ] )
        i= 0
        space= dynamic.node("D1'").parentSpace()
        for conf in space :
            self.assertEqual( conf, config[i] )
            i+= 1

        config= []
        for a2 in ["roll", "keep"] :
            for d2 in range(1, 7) :
                    config.append( [ d2, a2 ] )
        i= 0
        for conf in dynamic.node("ID2").parentSpace() :
            self.assertEqual( conf, config[i] )
            i+= 1

    def dynamic_initProbas(self, dynamic):
        for d, a in dynamic.node("ID1").parentSpace().tuples() :
            if a == 'roll' :
                dynamic.node("ID1").setDistribution( [d, a], { (i+1):(1/6) for i in range(6)}  )
            else :
                dynamic.node("ID1").setDistribution( [d, a], { d: 1.0 }  )

        distrib= dynamic.node("ID1").conditionalDistributionDico()
        reference= { '1-roll': {1: 1/6, 2: 1/6, 3: 1/6, 4: 1/6, 5: 1/6, 6: 1/6},
            '2-roll': {1: 1/6, 2: 1/6, 3: 1/6, 4: 1/6, 5: 1/6, 6: 1/6},
            '3-roll': {1: 1/6, 2: 1/6, 3: 1/6, 4: 1/6, 5: 1/6, 6: 1/6},
            '4-roll': {1: 1/6, 2: 1/6, 3: 1/6, 4: 1/6, 5: 1/6, 6: 1/6},
            '5-roll': {1: 1/6, 2: 1/6, 3: 1/6, 4: 1/6, 5: 1/6, 6: 1/6},
            '6-roll': {1: 1/6, 2: 1/6, 3: 1/6, 4: 1/6, 5: 1/6, 6: 1/6},
            '1-keep': {1: 1.0}, '2-keep': {2: 1.0}, '3-keep': {3: 1.0}, '4-keep': {4: 1.0}, '5-keep': {5: 1.0}, '6-keep': {6: 1.0} 
        }
        self.assertEqual( distrib, reference )
        
    # Test sequences:
    def test_dynamicTransition(self):
        dynamic= self.game421_initialize()
        # Tester quelque transition....
        self.assertEqual( dynamic.distributionDicoFrom( [2, 6, 4, 1], ['roll', 'keep', 'keep'] ),
            { '1-4-1-1': 1/6, '1-4-2-1': 1/6, '1-4-3-1': 1/6, '1-4-4-1': 1/6, '1-5-4-1': 1/6, '1-6-4-1': 1/6 }
        )
        self.assertEqual( dynamic.distributionDicoFrom( [2, 2, 2, 2], ['keep', 'keep', 'keep'] ), { '0-2-2-2': 1.0 } )
        self.assertEqual( dynamic.distributionDicoFrom( [1, 6, 4, 1], ['roll', 'roll', 'keep'] ),
            {'0-1-1-1': 1/36, '0-2-1-1': 2/36, '0-3-1-1': 2/36,
             '0-4-1-1': 2/36, '0-5-1-1': 2/36, '0-6-1-1': 2/36,
             '0-2-2-1': 1/36, '0-3-2-1': 2/36, '0-4-2-1': 2/36,
             '0-5-2-1': 2/36, '0-6-2-1': 2/36, '0-3-3-1': 1/36,
             '0-4-3-1': 2/36, '0-5-3-1': 2/36, '0-6-3-1': 2/36,
             '0-4-4-1': 1/36, '0-5-4-1': 2/36, '0-6-4-1': 2/36,
             '0-5-5-1': 1/36, '0-6-5-1': 2/36, '0-6-6-1': 1/36}
        )
        self.assertEqual( dynamic.distributionDicoFrom( [0, 6, 4, 1], ['roll', 'keep', 'keep'] ),
            { '0-4-1-1': 1/6, '0-4-2-1': 1/6, '0-4-3-1': 1/6, '0-4-4-1': 1/6, '0-5-4-1': 1/6, '0-6-4-1': 1/6 }
        )

    def game421_initialize(self):
        diceDomain= range(1,7)
        actionDomain= ["roll", "keep"]
        stateVariables= { "H": range(3), "D1": diceDomain,
            "D2": diceDomain, "D3": diceDomain }
        actionVariables= { "A1": actionDomain, "A2": actionDomain, "A3": actionDomain  }
        transitionalVariables= { "ID1": diceDomain, "ID2": diceDomain, "ID3": diceDomain }

        dynamic= wd.Dynamic(stateVariables, actionVariables, transitionalVariables)

        dynamic.node("ID1").setParents( ["D1", "A1"] )
        dynamic.node("ID2").setParents( ["D2", "A2"] )
        dynamic.node("ID3").setParents( ["D3", "A3"] )

        for var in ["ID1", "ID2", "ID3"]:
            node= dynamic.node(var)
            for d, a in node.parentSpace().tuples() :
                if a == 'roll' :
                    node.setDistribution( [d, a], { (i+1):(1/6) for i in range(6)}  )
                else :
                    node.setDistribution( [d, a], { d: 1.0 }  )

        node= dynamic.node("H'")
        node.setParents( ["H", "A1", "A2", "A3"] )
        for h, a1, a2, a3 in node.parentSpace().tuples() :
            if h < 2 or ( a1 == 'keep' and a2 == 'keep' and a3 == 'keep' ) :
                node.setDistribution( [h, a1, a2, a3], { 0: 1.0 }  )
            else :
                node.setDistribution( [h, a1, a2, a3], { (h-1): 1.0 }  )

        node= dynamic.node("D1'")
        node.setParents( ["ID1", "ID2", "ID3"] )
        for d1, d2, d3 in node.parentSpace() :
            hand= [d1, d2, d3]
            hand.sort()
            node.setDistribution( [d1, d2, d3], { hand[2]: 1.0 }  )

        node= dynamic.node("D2'")
        node.setParents( ["ID1", "ID2", "ID3"] )
        for d1, d2, d3 in node.parentSpace() :
            hand= [d1, d2, d3]
            hand.sort()
            node.setDistribution( [d1, d2, d3], { hand[1]: 1.0 }  )

        node= dynamic.node("D3'")
        node.setParents( ["ID1", "ID2", "ID3"] )
        for d1, d2, d3 in node.parentSpace() :
            hand= [d1, d2, d3]
            hand.sort()
            node.setDistribution( [d1, d2, d3], { hand[0]: 1.0 }  )

        return dynamic

if __name__ == '__main__':
    unit.main()
