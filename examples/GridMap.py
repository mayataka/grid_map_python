from grid_map import GridMap, InterpolationMethods
import numpy as np

def testClipToMap():
    #
    # Test 8 points outside of map.
    # 
    #  A  B  C
    #   +---+
    #   |   |         X
    #  D|   |E        ^
    #   |   |         |
    #   +---+     Y<--+
    #  F  G  H
    # 
    #  Note: Position to index alignment is a half open interval.
    #        An example position of 0.5 is assigned to the upper index.
    #        The interval in the current example is: 
    #        Position: [...)[0.485 ... 0.5)[0.5 ... 0.505)[...)
    #        Index:      8          9           10          11
    #
    map = GridMap(layers=['types'])
    map.setGeometry(length=np.array([1.0, 1.0]), resolution=0.05, position=np.array([0.0, 0.0]))

    print('=============== Point A ===============')
    outsidePosition = np.array([1.0, 1.0])
    closestInsidePosition = map.getClosestPositionInMap(outsidePosition)
    isInside, insideIndex = map.getIndex(closestInsidePosition)
    print('isInside: ', isInside, ', insideIndex: ', insideIndex, ', closestInsidePosition: ', closestInsidePosition)

    print('\n=============== Point B ===============')
    outsidePosition = np.array([1.0, 0.0])
    closestInsidePosition = map.getClosestPositionInMap(outsidePosition)
    isInside, insideIndex = map.getIndex(closestInsidePosition)
    print('isInside: ', isInside, ', insideIndex: ', insideIndex, ', closestInsidePosition: ', closestInsidePosition)

    print('\n=============== Point C ===============')
    outsidePosition = np.array([1.0, -1.0])
    closestInsidePosition = map.getClosestPositionInMap(outsidePosition)
    isInside, insideIndex = map.getIndex(closestInsidePosition)
    print('isInside: ', isInside, ', insideIndex: ', insideIndex, ', closestInsidePosition: ', closestInsidePosition)

    print('\n=============== Point D ===============')
    outsidePosition = np.array([0.0, 1.0])
    closestInsidePosition = map.getClosestPositionInMap(outsidePosition)
    isInside, insideIndex = map.getIndex(closestInsidePosition)
    print('isInside: ', isInside, ', insideIndex: ', insideIndex, ', closestInsidePosition: ', closestInsidePosition)

    print('\n=============== Point E ===============')
    outsidePosition = np.array([0.0, -1.0])
    closestInsidePosition = map.getClosestPositionInMap(outsidePosition)
    isInside, insideIndex = map.getIndex(closestInsidePosition)
    print('isInside: ', isInside, ', insideIndex: ', insideIndex, ', closestInsidePosition: ', closestInsidePosition)

    print('\n=============== Point F ===============')
    outsidePosition = np.array([-1.0, 1.0])
    closestInsidePosition = map.getClosestPositionInMap(outsidePosition)
    isInside, insideIndex = map.getIndex(closestInsidePosition)
    print('isInside: ', isInside, ', insideIndex: ', insideIndex, ', closestInsidePosition: ', closestInsidePosition)

    print('\n=============== Point G ===============')
    outsidePosition = np.array([-1.0, 0.0])
    closestInsidePosition = map.getClosestPositionInMap(outsidePosition)
    isInside, insideIndex = map.getIndex(closestInsidePosition)
    print('isInside: ', isInside, ', insideIndex: ', insideIndex, ', closestInsidePosition: ', closestInsidePosition)


def testExtendMapAligned():
    map1 = GridMap()
    map1.setGeometry(length=np.array([5.1, 5.1]), resolution=1.0, position=np.array([0.0, 0.0])) # buffer size is [5, 5]
    map1.add("zero", 0.0)
    map1.add("one", 1.0)
    map1.setBasicLayers(map1.getLayers())

    map2 = GridMap()
    map2.setGeometry(length=np.array([3.1, 3.1]), resolution=1.0, position=np.array([2.0, 2.0])) # buffer size is [3, 3]
    map2.add("one", 1.1)
    map2.add("two",  2.0)
    map2.setBasicLayers(map1.getLayers())

    map1.addDataFrom(map2, True, True, True)

    print('\n=============== Test extend ===============')
    print(map1.exists("two"))
    print(map1.isInside([3.0, 3.0]))
    print('length: ', map1.getLength())
    print('position: ', map1.getPosition())
    print('getValueAt("one", [2, 2]): ', map1.getValueAt(layer='one', index=np.array([2, 2])))
    print('getValueAt("one", [-2, -2]): ', map1.getValueAt(layer='one', index=np.array([-2, -2])))
    print('getValueAt("zero", [0, 0]): ', map1.getValueAt(layer='zero', index=np.array([0, 0])))

def testNearestNeighbor():
    map = GridMap(layers=["types"])
    map.setGeometry(length=np.array([3.0, 3.0]), resolution=1.0, position=np.array([0.0, 0.0])) 
    map.setValueAt(layer="types", index=[0, 0], value=0.5)
    map.setValueAt(layer="types", index=[0, 1], value=3.8)
    map.setValueAt(layer="types", index=[0, 2], value=2.0)
    map.setValueAt(layer="types", index=[1, 0], value=2.1)
    map.setValueAt(layer="types", index=[1, 1], value=1.0)
    map.setValueAt(layer="types", index=[1, 2], value=2.0)
    map.setValueAt(layer="types", index=[2, 0], value=1.0)
    map.setValueAt(layer="types", index=[2, 1], value=2.0)
    map.setValueAt(layer="types", index=[2, 2], value=2.0)

    print('\n=============== Test NearestNeighbor ===============')
    value = map.getValueAtPosition(layer="types", position=np.array([1.35, -0.4]))
    print('value: ', value)
    value = map.getValueAtPosition(layer="types", position=np.array([-0.3, 0.0]))
    print('value: ', value)

def testLinearInterpolated():
    map = GridMap(layers=["types"])
    map.setGeometry(length=np.array([3.0, 3.0]), resolution=1.0, position=np.array([0.0, 0.0])) 
    map.setValueAt(layer="types", index=[0, 0], value=0.5)
    map.setValueAt(layer="types", index=[0, 1], value=3.8)
    map.setValueAt(layer="types", index=[0, 2], value=2.0)
    map.setValueAt(layer="types", index=[1, 0], value=2.1)
    map.setValueAt(layer="types", index=[1, 1], value=1.0)
    map.setValueAt(layer="types", index=[1, 2], value=2.0)
    map.setValueAt(layer="types", index=[2, 0], value=1.0)
    map.setValueAt(layer="types", index=[2, 1], value=2.0)
    map.setValueAt(layer="types", index=[2, 2], value=2.0)

    print('\n=============== Test LinearInterpolated ===============')
    value = map.getValueAtPosition(layer="types", position=np.array([-0.5, -1.2]), interpolationMethods=InterpolationMethods.INTER_LINEAR)
    print('value: ', value)
    value = map.getValueAtPosition(layer="types", position=np.array([-0.5, 0]), interpolationMethods=InterpolationMethods.INTER_LINEAR)
    print('value: ', value)
    value = map.getValueAtPosition(layer="types", position=np.array([0.69, 0.38]), interpolationMethods=InterpolationMethods.INTER_LINEAR)
    print('value: ', value)


testClipToMap()
testExtendMapAligned()
testNearestNeighbor()
testLinearInterpolated()