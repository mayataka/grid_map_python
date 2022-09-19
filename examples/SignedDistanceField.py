from grid_map import GridMap, SignedDistanceField
import numpy as np

def testGetDistance():
    map = GridMap(layers=['layer'])
    map.setGeometry(length=np.array([1.0, 2.0]), resolution=0.1, position=np.array([0.15, 0.25]))
    map["layer"] = np.full(map["layer"].shape, 1.0)
    map.setValueAt(layer="layer", index=[3, 4], value=2.0)
    map.setValueAt(layer="layer", index=[3, 5], value=2.0)
    map.setValueAt(layer="layer", index=[3, 6], value=2.0)
    map.setValueAt(layer="layer", index=[4, 4], value=2.0)
    map.setValueAt(layer="layer", index=[4, 5], value=2.0)
    map.setValueAt(layer="layer", index=[4, 6], value=2.0)
    map.setValueAt(layer="layer", index=[5, 4], value=2.0)
    map.setValueAt(layer="layer", index=[5, 5], value=2.0)
    map.setValueAt(layer="layer", index=[5, 6], value=2.0)
    map.setValueAt(layer="layer", index=[6, 4], value=2.0)
    map.setValueAt(layer="layer", index=[6, 5], value=2.0)
    map.setValueAt(layer="layer", index=[6, 6], value=2.0)
    map.setValueAt(layer="layer", index=[7, 4], value=2.0)
    map.setValueAt(layer="layer", index=[7, 5], value=2.0)
    map.setValueAt(layer="layer", index=[7, 6], value=2.0)

    sdf = SignedDistanceField()
    sdf.calculateSignedDistanceField(gridMap=map, layer="layer", heightClearance=1.5)

    _, pos = map.getPosition(index=[5, 5])
    x, y = pos[0], pos[1]
    for z in [0.0, 0.5, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 10.0]:
        print('at (x, y, ' + str(z) + '): ', sdf.getDistanceAt(position=np.array([x, y, z])))

    _, pos = map.getPosition(index=[5, 2])
    x, y = pos[0], pos[1]
    for z in [0.0, 0.5, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 10.0]:
        print('at (x, y, ' + str(z) + '): ', sdf.getDistanceAt(position=np.array([x, y, z])))

def testGetDistanceGradient():
    map = GridMap(layers=['layer'])
    map.setGeometry(length=np.array([1.0, 2.0]), resolution=0.1, position=np.array([0.15, 0.25]))
    map["layer"] = np.full(map["layer"].shape, 1.0)
    map.setValueAt(layer="layer", index=[3, 4], value=2.0)
    map.setValueAt(layer="layer", index=[3, 5], value=2.0)
    map.setValueAt(layer="layer", index=[3, 6], value=2.0)
    map.setValueAt(layer="layer", index=[4, 4], value=2.0)
    map.setValueAt(layer="layer", index=[4, 5], value=2.0)
    map.setValueAt(layer="layer", index=[4, 6], value=2.0)
    map.setValueAt(layer="layer", index=[5, 4], value=2.0)
    map.setValueAt(layer="layer", index=[5, 5], value=2.0)
    map.setValueAt(layer="layer", index=[5, 6], value=2.0)
    map.setValueAt(layer="layer", index=[6, 4], value=2.0)
    map.setValueAt(layer="layer", index=[6, 5], value=2.0)
    map.setValueAt(layer="layer", index=[6, 6], value=2.0)
    map.setValueAt(layer="layer", index=[7, 4], value=2.0)
    map.setValueAt(layer="layer", index=[7, 5], value=2.0)
    map.setValueAt(layer="layer", index=[7, 6], value=2.0)

    sdf = SignedDistanceField()
    sdf.calculateSignedDistanceField(gridMap=map, layer="layer", heightClearance=1.5)

    _, pos = map.getPosition(index=[5, 6])
    x, y = pos[0], pos[1]
    for z in [0.0, 1.0, 2.0, 2.2, 10.0]:
        print('at (x, y, ' + str(z) + '): ', sdf.getDistanceGradientAt(position=np.array([x, y, z])))

    _, pos = map.getPosition(index=[2, 2])
    x, y = pos[0], pos[1]
    for z in [0.0, 1.0, 2.0, 2.2, 10.0]:
        print('at (x, y, ' + str(z) + '): ', sdf.getDistanceGradientAt(position=np.array([x, y, z])))


def testGetInterpolatedDistance():
    map = GridMap(layers=['layer'])
    map.setGeometry(length=np.array([1.0, 2.0]), resolution=0.1, position=np.array([0.15, 0.25]))
    map["layer"] = np.full(map["layer"].shape, 1.0)
    map.setValueAt(layer="layer", index=[3, 3], value=2.0)
    map.setValueAt(layer="layer", index=[3, 4], value=2.0)
    map.setValueAt(layer="layer", index=[3, 5], value=2.0)
    map.setValueAt(layer="layer", index=[3, 6], value=2.0)
    map.setValueAt(layer="layer", index=[3, 7], value=2.0)
    map.setValueAt(layer="layer", index=[4, 3], value=2.0)
    map.setValueAt(layer="layer", index=[4, 4], value=2.0)
    map.setValueAt(layer="layer", index=[4, 5], value=2.0)
    map.setValueAt(layer="layer", index=[4, 6], value=2.0)
    map.setValueAt(layer="layer", index=[4, 7], value=2.0)
    map.setValueAt(layer="layer", index=[5, 3], value=2.0)
    map.setValueAt(layer="layer", index=[5, 4], value=2.0)
    map.setValueAt(layer="layer", index=[5, 5], value=2.0)
    map.setValueAt(layer="layer", index=[5, 6], value=2.0)
    map.setValueAt(layer="layer", index=[5, 7], value=2.0)
    map.setValueAt(layer="layer", index=[6, 3], value=2.0)
    map.setValueAt(layer="layer", index=[6, 4], value=2.0)
    map.setValueAt(layer="layer", index=[6, 5], value=2.0)
    map.setValueAt(layer="layer", index=[6, 6], value=2.0)
    map.setValueAt(layer="layer", index=[6, 7], value=2.0)
    map.setValueAt(layer="layer", index=[7, 3], value=2.0)
    map.setValueAt(layer="layer", index=[7, 4], value=2.0)
    map.setValueAt(layer="layer", index=[7, 5], value=2.0)
    map.setValueAt(layer="layer", index=[7, 6], value=2.0)
    map.setValueAt(layer="layer", index=[7, 7], value=2.0)

    sdf = SignedDistanceField()
    sdf.calculateSignedDistanceField(gridMap=map, layer="layer", heightClearance=1.5)

    _, pos = map.getPosition(index=[5, 5])
    x, y = pos[0], pos[1]
    for z in [0.0, 0.5, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 10.0]:
        print('at (x, y, ' + str(z) + '): ', sdf.getInterpolatedDistanceAt(position=np.array([x, y, z])))

    _, pos = map.getPosition(index=[5, 10])
    x, y = pos[0], pos[1]
    for z in [0.0, 0.5, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 10.0]:
        print('at (x, y, ' + str(z) + '): ', sdf.getInterpolatedDistanceAt(position=np.array([x, y, z])))

    pointCloud = sdf.convertToPointCloud()
    for i in range(pointCloud.size()):
        if i % 100 == 0:
            print('PointCloud[' + str(i) + ']: ', pointCloud[i])

testGetDistance()
testGetDistanceGradient()
testGetInterpolatedDistance()